#include "src/args_gets.hpp"
#include "src/hppcut.hpp"
#include "src/build.hpp"

using namespace clib;

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
            {'s', "shared"}, // TODO: testing shared libs
            {'h', "hpp-cut"},
            {'n', "no-build"},
        });

        const bool hpp_cut = args_get_hpp_cut(args);
        const bool no_build = args_get_no_build(args);
        const string inputFilename = args_get_input_filename(args);
        const string outputFolder = args_get_output_folder(args);
        const string hppCutFolder = args_get_hpp_cut_folder(args);

        args_t build_args = args;
        if (hpp_cut) {
            hppcut(args); // TODO: do not override .h and .cpp files if already exists and newer that the source files
            build_args["input-filename"] = path_normalize(outputFolder + "/" + hppCutFolder + "/" + inputFilename);
        }
        if (!no_build) build(build_args);

    } catch (exception &e) {
        const string errmsg = "Exception in build process: " + string(e.what());
        cerr << errmsg << endl;
        LOGE(errmsg);
        return -1;
    }


    return 0;
}