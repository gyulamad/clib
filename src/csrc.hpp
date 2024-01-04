#pragma once 

#include <string>
#include <vector>

#include "../clib/files.hpp"
#include "../clib/vectors.hpp"
#include "../clib/time.hpp"

using namespace std;
using namespace clib;

void csrc_collect_deps(
    const string& basepath,
    const string& filename, vector<string>& deps,
    const string& hExtension, const vector<string>& cppExtensions
) {
    //cout << "Collect dependencies of: " COLOR_FILENAME << filename << COLOR_DEFAULT << endl;
    vector<string> matches;
    string contents = file_get_contents(filename);
    if (!regx_match_all("\\n\\s*\\#include\\s*\"(.*)\"", contents, &matches)) return;
    for (size_t i = 1; i < matches.size(); i += 2) {
        string filepath = path_normalize(basepath + "/" + path_extract(filename) + "/" + matches[i]);
        if (vector_contains(deps, filepath)) continue;
        deps.push_back(filepath);
        if (str_ends_with(hExtension, filepath)) {
            for (const string& cppExtension: cppExtensions) {
                string cppFile = file_replace_extension(filepath, cppExtension);
                if (file_exists(cppFile)) {
                    if (vector_contains(deps, filepath)) continue;
                    deps.push_back(cppFile);
                }
            }
        }
        csrc_collect_deps(basepath, filepath, deps, hExtension, cppExtensions);
    }
    vector_unique(deps);
}

ms_t csrc_get_lst_mtime(
    const string& basepath,
    const string& filename,
    const string& hExtension,
    const vector<string>& cppExtensions,
    vector<string>& dependencies
) {
    ms_t lastModAt = file_get_mtime(filename);
    csrc_collect_deps(basepath, filename, dependencies, hExtension, cppExtensions);
    for (const string& dependency: dependencies) {
        ms_t depLastModAt = file_get_mtime(dependency);
        if (depLastModAt > lastModAt) lastModAt = depLastModAt;
    }
    return lastModAt;
}