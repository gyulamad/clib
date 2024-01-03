#include <iostream>
#include "src/args.hpp"
#include "src/Log.hpp"
#include "src/files.hpp"
#include "src/sys.hpp"

using namespace clib;

void _exec(const string& cmd) {
    cout << COLOR_INFO << "Execute command: " << COLOR_DEFAULT << cmd << endl;
    cout << exec(cmd);
}

int main(int argc, const char* argv[]) {
    try {

        args_t args = args_parse(argc, argv, {
            {'f', "source-filename"},
            {'o', "output-folder"},
            {'d', "debug"},
        });

        const string sourceFilename = __DIR__ + args_get(args, "source-filename");
        const string executableExtension = args_get(args, "executable-extension", "");
        const string outputFolder = __DIR__ + args_get(args, "output-folder", "build");
        const bool debug = args_has(args, "debug");

        const string executableFilename = outputFolder + "/" + file_replace_extension(sourceFilename, executableExtension);
        const string executablePath = path_extract(executableFilename);
        if (!file_exists(executablePath)) _exec("mkdir -p " + executablePath);
        _exec("g++ " + string(debug ? "-g " : "") + sourceFilename + " -o " + executableFilename);
        if (!debug) _exec(executableFilename);


    } catch (exception &e) {
        const string errmsg = "Exception in build process: " + string(e.what());
        cerr << errmsg << endl;
        LOGE(errmsg);
    }


    return 0;
}