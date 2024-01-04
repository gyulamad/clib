#pragma once

#include <map>
#include <string>
#include <cassert>

#include "../../clib/str.hpp"
#include "../../clib/args.hpp"

using namespace std;
using namespace clib;

class ArgsTests {
public:

    static void test_args_parse_with_shortcuts() {
        const char* argv[] = { "program", "-x", "valueX", "--long", "valueLong", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        const map<const char, string> shorts = { { 'x', "short" } };
        map<const string, string> result = args_parse(argc, argv, &shorts);

        assert(result["short"] == "valueX");
        assert(result["long"] == "valueLong");
    }

    static void test_args_parse_empty_key() {
        const char* argv[] = { "program", "-", "value", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        bool thrown = false;
        try {
            args_parse(argc, argv);
            // Should not reach here, an exception is expected
        } catch (const exception& e) {
            thrown = true;
            assert(str_ends_with("Empty argument key", string(e.what())));
        }

        assert(thrown == true);
    }

    static void test_args_parse_duplicate_arguments() {
        const char* argv[] = { "program", "-a", "valueA", "-a", "valueB", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        map<const string, string> result = args_parse(argc, argv);

        assert(result["a"] == "valueB");
    }

    static void test_args_parse_missing_values() {
        const char* argv[] = { "program", "-a", "-b", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        map<const string, string> result = args_parse(argc, argv);

        assert(result["a"].empty());
        assert(result["b"].empty());
    }

    static void test_args_parse_unrecognized_arguments() {
        const char* argv[] = { "program", "-a", "valueA", "--unknown", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        map<const string, string> result = args_parse(argc, argv);

        assert(result["a"] == "valueA");
        assert(result.find("unknown") != result.end());
        assert(result["unknown"] == ""); // Ensure that "--unknown" has an empty string value
    }


    static void test_args_parse_mix_short_and_long() {
        const char* argv[] = { "program", "-a", "valueA", "--long", "valueLong", "-b", "valueB", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        map<const string, string> result = args_parse(argc, argv);

        assert(result["a"] == "valueA");
        assert(result["long"] == "valueLong");
        assert(result["b"] == "valueB");
    }

    static void test_args_parse_no_arguments() {
        const char* argv[] = { "program", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        map<const string, string> result = args_parse(argc, argv);

        assert(result.empty());
    }

    static void test_args_parse_complex_values() {
        const char* argv[] = { "program", "-a", "value with spaces", "--long", "value\"with\"quotes", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        map<const string, string> result = args_parse(argc, argv);

        assert(result["a"] == "value with spaces");
        assert(result["long"] == "value\"with\"quotes");
    }

    static void test_args_parse_no_value_for_last_argument() {
        const char* argv[] = { "program", "-a", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        map<const string, string> result = args_parse(argc, argv);

        assert(result["a"].empty());
    }

    static void test_args_parse_long_arguments_with_shortcuts() {
        const char* argv[] = { "program", "--longA", "valueA", "-b", "valueB", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        const map<const char, string> shorts = { { 'b', "shortB" } };
        map<const string, string> result = args_parse(argc, argv, &shorts);

        assert(result["longA"] == "valueA");
        assert(result["shortB"] == "valueB");
    }

    static void test_args_parse_short_arguments_without_values() {
        const char* argv[] = { "program", "-a", "-b", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        map<const string, string> result = args_parse(argc, argv);

        assert(result["a"].empty());
        assert(result["b"].empty());
    }

    static void test_args_parse_short_arguments_with_values_starting_with_dash() {
        const char* argv[] = { "program", "-a", "-valueA", "-b", "--valueB", nullptr };
        int argc = sizeof(argv) / sizeof(argv[0]) - 1;

        bool thrown = false;
        try {
            map<const string, string> result = args_parse(argc, argv);
            // If no exception is thrown, the test has failed
        } catch (const exception& e) {
            thrown = true;
            // Exception expected for invalid key "-valueA"
            assert(str_ends_with("Invalid argument key: -valueA", e.what()));
        }

        assert(thrown == true);
    }

    static void test_args_parse_with_whitespaces() {
        
    }
};
