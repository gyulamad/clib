#include "ArgsTests.hpp"
#include "StrTests.hpp"
#include "TimeTests.hpp"
#include "FilesTests.hpp"
#include "LogTests.hpp"
#include "SysTests.hpp"

int main() {

    cout << "Runing tests..." << endl;
    
    ArgsTests::test_args_parse_with_shortcuts();
    ArgsTests::test_args_parse_empty_key();
    ArgsTests::test_args_parse_duplicate_arguments();
    ArgsTests::test_args_parse_missing_values();
    ArgsTests::test_args_parse_unrecognized_arguments();
    ArgsTests::test_args_parse_mix_short_and_long();
    ArgsTests::test_args_parse_no_arguments();
    ArgsTests::test_args_parse_complex_values();
    ArgsTests::test_args_parse_no_value_for_last_argument();
    ArgsTests::test_args_parse_long_arguments_with_shortcuts();
    ArgsTests::test_args_parse_short_arguments_without_values();
    ArgsTests::test_args_parse_short_arguments_with_values_starting_with_dash();
    
    StrTests::test(); // TODO
    
    TimeTests::test(); // TODO

    FilesTests::test_Files_findByExtension();
    FilesTests::test_Files_findByExtensions();
    FilesTests::test_Files_replaceExtension();
    FilesTests::test_Files_normalizePath();
    FilesTests::test_Files_extractPath();
    FilesTests::test_Files_extractFilename();
    FilesTests::test_Files_exists();
    FilesTests::test_Files_createPath();
    FilesTests::test_Files_getLastModificationTime();
    FilesTests::test_Files_file_get_contents();
    FilesTests::test_Files_file_put_contents();

    LogTests::test_Log_writeln();

    SysTests::test(); // TODO

    // TODO: coverage

    cout << "Tests OK" << endl;
    
    return 0;
}