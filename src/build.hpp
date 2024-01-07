#pragma once

#include <string>

#include "../clib/args.hpp"
#include "../clib/files.hpp"
#include "../clib/maps.hpp"

#include "args_gets.hpp"
#include "csrc.hpp"
#include "iexec.hpp"

using namespace std;
using namespace clib;

string get_build_arguments(const args_t& args) {
    const bool debug = args_get_debug(args);
    const bool coverage = args_get_coverage(args);
    // const bool shared = args_get_shared(args);
    const string flags = args_get_flags(args);
    return flags +
        (debug ? " -g" : "") + 
        (coverage ? " -fprofile-arcs -ftest-coverage" : "") + 
        ((debug || coverage) ? " -O0" : " -O3");// +
        // (shared ? " -shared -fPIC" : "");
}

string compile_cpp_file(const args_t& args, vector<string>& dependencies) {
    const string outputFolder = args_get_output_folder(args);
    const string inputFilename = args_get_input_filename(args);
    const string outputExtension = args_get_output_extension(args);
    const string hExtension = args_get_h_extension(args);
    const vector<string> cppExtensions = args_get_cpp_extensions(args);
    const string includePaths = args_get_include_paths(args);
    const bool shared = args_get_shared(args);

    if (inputFilename.empty())
        throw ERROR("Input file is missing");

    if (!file_exists(inputFilename))
        throw ERROR("Input file is not found: " COLOR_FILENAME + inputFilename + COLOR_DEFAULT);

    if (is_dir(inputFilename))
        throw ERROR("Input file can not be a folder: " COLOR_FILENAME + inputFilename + COLOR_DEFAULT);

    const string outputFilename = path_normalize(outputFolder + "/" +
        file_replace_extension(inputFilename, outputExtension));

    ms_t lst_mtime = csrc_get_lst_mtime(
        path_extract(inputFilename), filename_extract(inputFilename), 
        hExtension, cppExtensions, dependencies
    );
    csrc_collect_deps(
        path_extract(inputFilename), filename_extract(inputFilename), 
        dependencies, hExtension, cppExtensions);

    if (file_exists(outputFilename) && 
        file_get_mtime(outputFilename) >= lst_mtime //file_get_mtime(inputFilename)
    ) {
        // cout 
        //     << COLOR_INFO "File already built (skip): "
        //     << COLOR_FILENAME << inputFilename << COLOR_DEFAULT " -> " 
        //     << COLOR_FILENAME << outputFilename << COLOR_DEFAULT << endl;
        return outputFilename;
    }
    cout << "Compile: " COLOR_FILENAME << inputFilename << COLOR_DEFAULT << endl;

    const string executablePath = path_extract(outputFilename);
    if (!file_exists(executablePath)) 
        if (!iexec("mkdir -p " + executablePath)) throw ERROR("Execution failed");

    const string arguments = get_build_arguments(args);

    if (!iexec(
        "g++ " + arguments + 
        (shared ? " -shared -fPIC " : " -c ") + inputFilename + 
        " -o " + outputFilename + 
        (includePaths != "" ? " " + includePaths : "")
    )) throw ERROR("Execution failed");

    return outputFilename;
}

string link_to_executable(const args_t args, const string& object, const vector<string>& dep_objects) {
    const string executableFilename = args_get_executable_filename(args);
    const string libs = args_get_libs(args);

    const string arguments = get_build_arguments(args);

    bool exists = file_exists(executableFilename);
    bool modified = exists && 
        file_get_mtime(executableFilename) <= file_get_mtime(object);
    if (!modified) {
        for (const string& dep_object: dep_objects)
            if (modified = 
                exists &&
                file_get_mtime(executableFilename) <= file_get_mtime(dep_object)
            ) break;
    }
    if (!exists || modified) {
        if (!iexec(
            "g++ " + arguments + " " + object + (dep_objects.empty() ? "" : " " + str_concat(dep_objects, " ")) + 
            " -o " + executableFilename 
            + (libs != "" ? " " + libs : "")
        )) throw ERROR("Execution failed");
    }

    return executableFilename;
}

void execute_file(const args_t& args, const string& executableFilename) {
    const bool coverage = args_get_coverage(args);

    if (!iexec(executableFilename)) throw ERROR("Execution failed");
    if (coverage) if (!iexec(
        "lcov -c -d . -o coverage.info && "
        "genhtml coverage.info -o coverage_report && "
        "google-chrome ./coverage_report/index.html")) throw ERROR("Execution failed");
}

string build_file(const args_t& args) {
    const bool execute = args_get_execute(args);
    const bool coverage = args_get_coverage(args);
    const vector<string> cppExtensions = args_get_cpp_extensions(args);
    // const string hppExtension = args_get_hpp_extension(args);
    const bool shared = args_get_shared(args);
    
    vector<string> dependencies;
    const string outputFilename = compile_cpp_file(args, dependencies);
    if (shared) return outputFilename;

    args_t dep_args = args;
    vector<string> objects;
    for (const string& dependency: dependencies) {
        const string extension = file_get_extension(dependency);
        if (vector_contains<string>(cppExtensions, extension)) {
            dep_args["input-filename"] = dependency;
            objects.push_back(compile_cpp_file(dep_args, dependencies));
        }
    }

    // reverse(objects.begin(), objects.end());
    string executableFilename = link_to_executable(args, outputFilename, objects);

    if (execute || coverage) execute_file(args, executableFilename);

    return executableFilename;
}

vector<string> build_files(const args_t& args, const string& inputFolder) {
    const vector<string> cppExtensions = args_get_cpp_extensions(args);
    const bool execute = args_get_execute(args);
    const bool coverage = args_get_coverage(args);

    const vector<string> inputFilenames = file_find_by_extensions(inputFolder, cppExtensions);
    args_t prj_args = args;
    vector<string> executableFilenames;
    for (const string& inputFilename: inputFilenames) {
        prj_args["input-filename"] = inputFilename;
        map_remove<args_key_t, args_val_t>(prj_args, "execute");
        executableFilenames.push_back(build_file(prj_args));
    }

    if (execute || coverage) {
        if (executableFilenames.size() != 1) 
            throw ERROR("Couldn't choose what to execute if the input is a folder and not only one C++ file in it.");
        
        execute_file(args, executableFilenames[0]);
    }

    return executableFilenames;
}

void build(const args_t& args) {
    const string inputFilename = args_get_input_filename(args);

    if (is_dir(inputFilename)) build_files(args, inputFilename);
    else build_file(args);
}