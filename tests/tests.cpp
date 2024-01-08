#include <iostream>

#include "clib/ArgsTests.hpp"
#include "clib/StrTests.hpp"
#include "clib/TimeTests.hpp"
#include "clib/FilesTests.hpp"
#include "clib/LogTests.hpp"
#include "clib/SysTests.hpp"
#include "clib/VectorsTests.hpp"
#include "clib/MapsTests.hpp"
#include "clib/ChiperTests.hpp"
#include "clib/RandTests.hpp"
#include "clib/VectorFactoryTests.hpp"
#include "clib/FactoryTests.hpp"

#include "src/CSrcTests.hpp"
#include "src/IExecTests.hpp"

using namespace std;

int main() {

    cout << "Running clib tests..." << endl;

    RandTests::test(); // TODO

    ChiperTests::test_base64();
    ChiperTests::test_rsa();
    
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
    
    StrTests::test_str_trim();
    StrTests::test_str_ltrim();
    StrTests::test_str_rtrim();
    StrTests::test_normalize_datetime();
    StrTests::test_str_split();
    StrTests::test_is_numeric();
    StrTests::test_parse();
    StrTests::test_str_replace();
    StrTests::test_str_replace_map();
    StrTests::test_str_to_lower();
    StrTests::test_parse_bool();
    StrTests::test_str_start_with_positive();
    StrTests::test_str_start_with_negative();
    StrTests::test_regx_match();
    StrTests::test_regx_match_alphabets();
    
    TimeTests::test_datetime_conversion();

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

    SysTests::test_exec();

    VectorsTests::test_vector_create_destroy();
    VectorsTests::test_vector_concat();
    VectorsTests::test_vector_save_and_load();
    VectorsTests::test_vector_load_and_load_with_reference();
    
    MapsTests::test_map_has();
    MapsTests::test_map_keys();
    MapsTests::test_map_key_exists();

    VectorFactoryTests::test_VectorFactory_withParams_Int();
    VectorFactoryTests::test_VectorFactory_withParams_CustomClass();
    VectorFactoryTests::test_VectorFactory_withoutParams();

    FactoryTests::test_Factory_shared_lib();
    

    cout << "Running builder tests..." << endl;

    CSrcTests::test(); // TODO
    IExecTests::test(); // TODO

    // TODO: coverage

    cout << COLOR_SUCCESS "Tests OK" COLOR_DEFAULT << endl;
    
    return 0;
}