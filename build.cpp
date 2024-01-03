#include <iostream>
#include "src/args.hpp"
#include "src/Log.hpp"
#include "src/files.hpp"
#include "src/sys.hpp"
#include "src/vectors.hpp"

using namespace clib;

void csrc_collect_deps(
    const string& filename, vector<string>& deps,
    const string& hExtension, const vector<string>& cppExtensions
) {
    //cout << "Collect dependencies of: " COLOR_FILENAME << filename << COLOR_DEFAULT << endl;
    vector<string> matches;
    string contents = file_get_contents(filename);
    if (!regx_match_all("\\n\\s*\\#include\\s*\"(.*)\"", contents, &matches)) return;
    for (size_t i = 1; i < matches.size(); i += 2) {
        string filepath = path_normalize(__DIR__ + "/" + path_extract(filename) + "/" + matches[i]);
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
        csrc_collect_deps(filepath, deps, hExtension, cppExtensions);
    }
    vector_unique(deps);
}

ms_t csrc_get_lst_mtime(
    const string& filename,
    const string& hExtension,
    const vector<string>& cppExtensions,
    vector<string>& dependencies
) {
    ms_t lastModAt = file_get_mtime(filename);
    csrc_collect_deps(filename, dependencies, hExtension, cppExtensions);
    for (const string& dependency: dependencies) {
        ms_t depLastModAt = file_get_mtime(dependency);
        if (depLastModAt > lastModAt) lastModAt = depLastModAt;
    }
    return lastModAt;
}

bool iexec(const string& cmd) {
    cout << COLOR_INFO "Execute command:" COLOR_DEFAULT " $ " << cmd << endl;
    string output = exec(cmd);    
    if (!output.empty()) {
        string replaced = output;
        replaced = str_replace(replaced, "error:", COLOR_ERROR "error:" COLOR_DEFAULT);
        cout << replaced << endl;
        if (replaced != output) {
            LOGE("Command output contains error(s).");
            return false;
        }
    }
    if (exec_last_exit_code != 0) {
        LOGE("Command exit code is non-zero: " + to_string(exec_last_exit_code));
        return false;
    }
    return true;
}

string args_get_flags(const args_t& args) {
    return args_get(args, "flags", 
        "-Wall -Werror -pedantic -Wextra -Wunused -Wuninitialized "
        "-Wformat -Wconversion -Wcast-align -Wnull-dereference "
        "-Wlogical-op -Wfloat-conversion -Wdouble-promotion "
        "-Wsign-conversion -Wsign-promo -Wcast-qual "
        "-Wdisabled-optimization -Werror=return-type -Werror=main "
        "-std=c++17");
}


bool args_get_debug(const args_t& args) {
    return args_has(args, "debug");
}

string args_get_output_folder(const args_t& args) {
    const bool debug = args_get_debug(args);
    return path_normalize(__DIR__ + args_get(args, "output-folder", string("build/") + (debug ? "debug" : "release")));
}

string args_get_input_filename(const args_t& args) {
    return path_normalize(__DIR__ + args_get(args, "input-filename", "."));
}

string args_get_executable_extension(const args_t& args) {
    return args_get(args, "executable-extension", "");
}

string args_get_executable_filename(const args_t& args) {
    const string outputFolder = args_get_output_folder(args);
    const string inputFilename = args_get_input_filename(args);
    const string executableExtension = args_get_executable_extension(args);
    return path_normalize(
        outputFolder + "/" + 
        args_get(
            args, "executable-filename", 
            path_extract(inputFilename) + "/" + 
            filename_extract(inputFilename, true) + executableExtension
        )
    );
}

string args_get_libs(const args_t& args) {
    return args_get(args, "libs", "-ldl");
}

bool args_get_shared(const args_t& args) {
    return args_has(args, "shared");
}

string args_get_output_extension(const args_t& args) {
    const bool shared = args_get_shared(args);
    return args_get(args, "output-extension", shared ? "so" : "o");
}

string args_get_h_extension(const args_t& args) {
    return args_get(args, "h-extension", "h");
}

bool args_get_execute(const args_t& args) {
    return args_has(args, "execute");
}

bool args_get_coverage(const args_t& args) {
    return args_has(args, "coverage");
}

vector<string> args_get_cpp_extensions(const args_t& args) {
    return str_split(",", args_get(args, "cpp-extensions", "c,cpp"));
}

string args_get_include_paths(const args_t& args) {
    return args_get(args, "include-paths", ""); // TODO: use path_normalize()
}

string get_build_arguments(const args_t& args) {
    const bool debug = args_get_debug(args);
    const bool coverage = args_get_coverage(args);
    const bool shared = args_get_shared(args);
    const string flags = args_get_flags(args);
    return flags +
        (debug ? " -g" : "") + 
        (coverage ? " -fprofile-arcs -ftest-coverage" : "") + 
        ((debug || coverage) ? " -O0" : " -O3") +
        (shared ? " -shared -fPID" : "");
}

string compile_cpp_file(const args_t& args, vector<string>& dependencies) {
    const string outputFolder = args_get_output_folder(args);
    const string inputFilename = args_get_input_filename(args);
    const string outputExtension = args_get_output_extension(args);
    const string hExtension = args_get_h_extension(args);
    const vector<string> cppExtensions = args_get_cpp_extensions(args);
    const string includePaths = args_get_include_paths(args);


    const string outputFilename = path_normalize(outputFolder + "/" +
        file_replace_extension(inputFilename, outputExtension));

    if (
        file_exists(outputFilename) && file_get_mtime(outputFilename) >=
            csrc_get_lst_mtime(inputFilename, hExtension, cppExtensions, dependencies)
    ) {
        cout 
            << COLOR_INFO "File already built (skip): "
            << COLOR_FILENAME << inputFilename << COLOR_DEFAULT " -> " 
            << COLOR_FILENAME << outputFilename << COLOR_DEFAULT << endl;
        return outputFilename;
    }

    const string executablePath = path_extract(outputFilename);
    if (!file_exists(executablePath)) iexec("mkdir -p " + executablePath);

    const string arguments = get_build_arguments(args);

    iexec(
        "g++ " + arguments + 
        " -c " + inputFilename + 
        " -o " + outputFilename + 
        (includePaths != "" ? " " + includePaths : "")
    );

    return outputFilename;
}

string link_to_executable(const args_t args, const string& outputFilename, const vector<string>& objects) {
    const string executableFilename = args_get_executable_filename(args);
    const string libs = args_get_libs(args);

    const string arguments = get_build_arguments(args);

    iexec(
        "g++ " + arguments + " " + outputFilename + " " + str_concat(objects, " ") + 
        " -o " + executableFilename 
        + (libs != "" ? " " + libs : "")
    );

    return executableFilename;
}

void build_file(const args_t& args) {
    const bool execute = args_get_execute(args);
    const bool coverage = args_get_coverage(args);
    const vector<string> cppExtensions = args_get_cpp_extensions(args);
    
    vector<string> dependencies;
    const string outputFilename = compile_cpp_file(args, dependencies);

    args_t dep_args = args;
    vector<string> objects;
    for (const string& dependency: dependencies) {
        if (vector_contains<string>(cppExtensions, file_get_extension(dependency))) {
            dep_args["input-filename"] = dependency;
            objects.push_back(compile_cpp_file(dep_args, dependencies));
        }
    }

    string executableFilename = link_to_executable(args, outputFilename, objects);

    if (execute || coverage) {
        iexec(executableFilename);
        if (coverage) iexec(
            "lcov -c -d . -o coverage.info && "
            "genhtml coverage.info -o coverage_report && "
            "google-chrome ./coverage_report/index.html");
    }
}

void build_files(const args_t& args, const string& inputFolder) {
    const vector<string> cppExtensions = args_get_cpp_extensions(args);
    const bool execute = args_get_execute(args);

    const vector<string> inputFilenames = file_find_by_extensions(inputFolder, cppExtensions);
    if (execute && inputFilenames.size() != 1) 
        throw ERROR("Couldn't choose what to execute if the input is a folder and not only one C++ file in it.");
    args_t prj_args = args;
    for (const string& inputFilename: inputFilenames) {
        prj_args["input-filename"] = inputFilename;
        build_file(prj_args);
    }
}

int main(int argc, const char* argv[]) {
    try {

        args_t args = args_parse(argc, argv, {
            {'i', "input-filename"},
            {'o', "output-folder"},
            {'x', "executable-filename"},
            {'d', "debug"},
            {'c', "coverage"},
            {'l', "libs"},
            {'e', "execute"},
            {'s', "shared"},
        });

        const string inputFilename = args_get_input_filename(args);

        if (is_dir(inputFilename)) build_files(args, inputFilename);
        else build_file(args);

    } catch (exception &e) {
        const string errmsg = "Exception in build process: " + string(e.what());
        cerr << errmsg << endl;
        LOGE(errmsg);
        return -1;
    }


    return 0;
}