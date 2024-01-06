#pragma once 

#include <string>
#include <vector>
#include <iostream>

#include "../clib/sys.hpp"
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
    // if (basepath.empty()) basepath = getcwd();
    // cout << "Collect dependencies for: " COLOR_FILENAME << "[path:" << basepath + "]" + filename << COLOR_DEFAULT << endl;
    string contents = file_get_contents(
        path_normalize(
            (basepath.empty() ? getcwd() : basepath) + "/" + filename
        )
    );
    vector<string> lines = str_split("\n", contents);
    vector<vector<string>> line_matches;
    for (const string& line: lines) {
        vector<string> matches;
        if (
            regx_match("\\n\\s*\\#include\\s*\"(.*)\"", line, &matches) ||
            regx_match("^\\s*\\#include\\s*\"(.*)\"", line, &matches)
        )
            line_matches.push_back(matches);
    }
    //if (!regx_match_all("\\n\\s*\\#include\\s*\"(.*)\"", contents, &matches)) return; //  TODO: bug: it can not see the if the #include in the very first line
    for (const vector<string>& matches: line_matches) {
        for (size_t i = 1; i < matches.size(); i += 2) {
            string path_extracted = path_extract(filename);
            string filepath = path_normalize(
                (basepath.empty() ? "" : basepath + "/") 
                    + (path_extracted.empty() ? "" : path_extracted + "/")
                    + matches[i]
            );
            if (vector_contains(deps, filepath)) continue;
            deps.push_back(filepath);
            if (str_ends_with(file_extension_with_dot(hExtension), filepath)) {
                for (const string& cppExtension: cppExtensions) {
                    string cppFile = file_replace_extension(filepath, cppExtension);
                    if (file_exists(cppFile)) {
                        if (vector_contains(deps, cppFile)) continue;
                        deps.push_back(cppFile);
                    }
                }
            }
            csrc_collect_deps(
                path_extract(filepath), filename_extract(filepath), 
                deps, hExtension, cppExtensions
            );
        }
        vector_unique(deps);
    }
}

ms_t csrc_get_lst_mtime(
    const string& basepath,
    const string& filename,
    const string& hExtension,
    const vector<string>& cppExtensions,
    vector<string>& dependencies
) {
    // if (basepath.empty()) basepath = getcwd();
    ms_t lastModAt = file_get_mtime(path_normalize((basepath.empty() ? getcwd() : basepath) + "/" + filename));
    csrc_collect_deps((basepath.empty() ? getcwd() : basepath), filename, dependencies, hExtension, cppExtensions);
    for (const string& dependency: dependencies) {
        ms_t depLastModAt = file_get_mtime(dependency);
        if (depLastModAt > lastModAt) lastModAt = depLastModAt;
    }
    return lastModAt;
}