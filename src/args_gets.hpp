#pragma once

#include <string>

#include "../clib/args.hpp"

using namespace std;
using namespace clib;

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
    return path_normalize(__DIR__ + "/../" + args_get(args, "output-folder", string("build/") + (debug ? "debug" : "release")));
}

string args_get_input_filename(const args_t& args) {
    return path_normalize(__DIR__ + "/../" + args_get(args, "input-filename", "."));
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

string args_get_hpp_extension(const args_t& args) {
    return args_get(args, "hpp-extension", "hpp");
}

bool args_get_execute(const args_t& args) {
    return args_has(args, "execute");
}

bool args_get_coverage(const args_t& args) {
    return args_has(args, "coverage");
}

vector<string> args_get_cpp_extensions(const args_t& args) {
    return str_split(",", args_get(args, "cpp-extensions", "cpp,c"));
}

string args_get_include_paths(const args_t& args) {
    return args_get(args, "include-paths", ""); // TODO: use path_normalize()
}

string args_get_hpp_cut_folder(const args_t& args) {
    return args_get(args, "hpp-cut-folder", "hppcut");
}

bool args_get_hpp_cut(const args_t& args) {
    return args_has(args, "hpp-cut");
}

bool args_get_no_build(const args_t& args) {
    return args_has(args, "no-build");
}