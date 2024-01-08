#pragma once

#include <cassert>

#include "../../clib/str.hpp"

using namespace std;
using namespace clib;

class StrTests {
public:

    static void test_str_trim() {
        const string testStr1 = "   Hello, World!   ";
        const string testStr2 = "  \t  \t Trim this string \t  \t  ";
        const string testStr3 = "No spaces to trim";

        string result1 = str_trim(testStr1);
        string result2 = str_trim(testStr2);
        string result3 = str_trim(testStr3);

        // Check if the results match the expected trimmed strings
        assert(result1 == "Hello, World!");
        assert(result2 == "Trim this string");
        assert(result3 == "No spaces to trim");
    }

    static void test_str_ltrim() {
        const string testStr1 = "   Hello, World!   ";
        const string testStr2 = "  \t  \t Trim this string \t  \t  ";
        const string testStr3 = "No spaces to trim";

        string result1 = str_ltrim(testStr1);
        string result2 = str_ltrim(testStr2);
        string result3 = str_ltrim(testStr3);

        // Check if the results match the expected trimmed strings
        assert(result1 == "Hello, World!   ");
        assert(result2 == "Trim this string \t  \t  ");
        assert(result3 == "No spaces to trim");
    }

    static void test_str_rtrim() {
        const string testStr1 = "   Hello, World!   ";
        const string testStr2 = "  \t  \t Trim this string \t  \t  ";
        const string testStr3 = "No spaces to trim";

        string result1 = str_rtrim(testStr1);
        string result2 = str_rtrim(testStr2);
        string result3 = str_rtrim(testStr3);

        // Check if the results match the expected trimmed strings
        assert(result1 == "   Hello, World!");
        assert(result2 == "  \t  \t Trim this string");
        assert(result3 == "No spaces to trim");
    }

    static void test_normalize_datetime() {
        const string testDateTime1 = "2023-01-15 08:30:45.123";
        const string testDateTime2 = "2022-05-02 12:23";
        const string testDateTime3 = "2021-06-10";

        string result1 = normalize_datetime(testDateTime1);
        string result2 = normalize_datetime(testDateTime2);
        string result3 = normalize_datetime(testDateTime3);

        // Check if the results match the expected normalized strings
        assert(result1 == "2023-01-15 08:30:45.123");
        assert(result2 == "2022-05-02 12:23:00.000");
        assert(result3 == "2021-06-10 00:00:00.000");
    }


    static void test_str_split() {
        // Test case 1: Split by a comma
        string data1 = "apple,banana,cherry";
        string separator1 = ",";
        vector<string> result1 = str_split(separator1, data1);
        assert(result1.size() == 3);
        assert(result1[0] == "apple");
        assert(result1[1] == "banana");
        assert(result1[2] == "cherry");

        // Test case 2: Split by a space
        string data2 = "one two three";
        string separator2 = " ";
        vector<string> result2 = str_split(separator2, data2);
        assert(result2.size() == 3);
        assert(result2[0] == "one");
        assert(result2[1] == "two");
        assert(result2[2] == "three");

        // Test case 3: Split by a semicolon
        string data3 = "Alice;Bob;Charlie";
        string separator3 = ";";
        vector<string> result3 = str_split(separator3, data3);
        assert(result3.size() == 3);
        assert(result3[0] == "Alice");
        assert(result3[1] == "Bob");
        assert(result3[2] == "Charlie");

        // Test case 4: Split by multiple characters
        string data4 = "apple...banana...cherry";
        string separator4 = "...";
        vector<string> result4 = str_split(separator4, data4);
        assert(result4.size() == 3);
        assert(result4[0] == "apple");
        assert(result4[1] == "banana");
        assert(result4[2] == "cherry");

        // Negative case 1: Empty data
        string data5 = "";
        string separator5 = ",";
        vector<string> result5 = str_split(separator5, data5);
        assert(result5.empty()); // Should result in an empty vector

        // Negative case 2: Separator not found
        string data6 = "abcdefg";
        string separator6 = ",";
        vector<string> result6 = str_split(separator6, data6);
        assert(result6.size() == 1); // Should result in a single element vector with the entire string
    }

    static void test_is_numeric() {
        // Test cases for is_numeric function
        assert(is_numeric("12345") == true);
        assert(is_numeric("-123.45") == true);
        assert(is_numeric("abc") == false);
        assert(is_numeric("   123   ") == true);
        assert(is_numeric("123abc") == false);
    }

    static void test_parse() {
        // Test cases for valid values
        assert(parse<double>("3.14159") == 3.14159);
        assert(parse<double>("-123.45") == -123.45);
        assert(parse<double>("0.0") == 0.0);
        assert(parse<double>(".0") == 0.0);
        assert(parse<long>("123") == 123);
        assert(parse<long>("-123") == -123);
        assert(parse<long>("0") == 0);
        
        // Test cases for invalid values
        try {
            parse<double>("abc"); // Invalid input
        } catch (exception &e) {
            assert("Invalid value: abc");
        }

        try {
            parse<double>("");   // Empty string
        } catch (exception &e) {
            assert("Invalid value");
        }

        try {
            parse<long>("abc"); // Invalid input
        } catch (exception &e) {
            assert("Invalid value: abc");
        }

        try {
            parse<long>("");   // Empty string
        } catch (exception &e) {
            assert("Invalid value");
        }
    }
    
    static void test_str_replace() {
        // Test case 1: Replace "This" with "That"
        string input1 = "This is a test. This is only a test.";
        string expected1 = "That is a test. That is only a test.";
        string result1 = str_replace(input1, "This", "That");
        assert(result1 == expected1);

        // Test case 2: Replace "Hello" with "Hi"
        string input2 = "Hello, World!";
        string expected2 = "Hi, World!";
        string result2 = str_replace(input2, "Hello", "Hi");
        assert(result2 == expected2);

        // Test case 3: Key not found (no replacements should occur)
        string input3 = "No replacements here.";
        string expected3 = "No replacements here.";
        string result3 = str_replace(input3, "Hello", "Hi");
        assert(result3 == expected3);

        // Test case 4: Empty input string (result should also be empty)
        string input4 = "";
        string expected4 = "";
        string result4 = str_replace(input4, "This", "That");
        assert(result4 == expected4);
    }

    static void test_str_replace_map() {
        // Test case 1: Replace multiple substrings
        string input1 = "Hello, World! This is a test.";
        map<string, string> replacements1 = {
            {"Hello", "Hi"},
            {"World", "Universe"},
            {"test", "example"}
        };
        string expected1 = "Hi, Universe! This is a example.";
        string result1 = str_replace(input1, replacements1);
        assert(result1 == expected1);

        // Test case 2: Empty input string (result should also be empty)
        string input2 = "";
        map<string, string> replacements2 = {
            {"Hello", "Hi"}
        };
        string expected2 = "";
        string result2 = str_replace(input2, replacements2);
        assert(result2 == expected2);
    }

    static void test_str_to_lower() {
        // Test case 1: Empty string
        assert(str_to_lower("") == "");

        // Test case 2: All uppercase characters
        assert(str_to_lower("HELLO") == "hello");

        // Test case 3: All lowercase characters
        assert(str_to_lower("world") == "world");

        // Test case 4: Mixed case characters
        assert(str_to_lower("HeLLo WoRLd") == "hello world");

        // Test case 5: String with non-alphabetic characters
        assert(str_to_lower("123!#@") == "123!#@");
    }

    static void test_parse_bool() {
        // Test case 1: Empty string
        assert(parse_bool("") == false);

        // Test case 2: String containing "0"
        assert(parse_bool("0") == false);

        // Test case 3: String containing "false" (case-insensitive)
        assert(parse_bool("FaLsE") == false);

        // Test case 4: String containing "no" (case-insensitive)
        assert(parse_bool("No") == false);

        // Test case 5: String containing "true"
        assert(parse_bool("true") == true);

        // Test case 6: String containing "yes"
        assert(parse_bool("Yes") == true);

        // Test case 7: String containing "1"
        assert(parse_bool("1") == true);
    }
    
    static void test_str_start_with_positive() {
        assert(str_starts_with("Hello", "Hello, World!"));
        assert(str_starts_with("abc", "abcdef"));
        assert(str_starts_with("", "Anything")); // An empty needle should match any haystack
        assert(str_starts_with("", "")); // Empty needle and haystack should match
        assert(str_starts_with("a", "a")); // Single character needle and haystack match
    }

    static void test_str_start_with_negative() {
        assert(!str_starts_with("World", "Hello, World!"));
        assert(!str_starts_with("abc", "ABCDEF")); // Case-sensitive check
        assert(!str_starts_with("Something", "")); // Needle is longer than haystack
        assert(!str_starts_with("abc", "ab")); // Needle is longer than haystack
    }

    static void test_regx_match() {
        string str = "This is a test string with some numbers 123 and some special characters !#@$%^&*";
        vector<string> matches;
        
        // Test for matching pattern that exists in the string
        assert(regx_match("[0-9]+", str, &matches) == 1);
        assert(matches.size() == 1);
        assert(matches[0] == string("123"));
        
        // Test for matching pattern that does not exist in the string
        assert(regx_match("[a-z]+", str, &matches) == 1);
        assert(matches.size() == 1);
        
        // Test for matching pattern that captures multiple groups
        assert(regx_match("([a-zA-Z]+)\\s([a-z]+)", str, &matches) == 1);
        assert(matches.size() == 3);
        assert(matches[0] == string("This is"));
        assert(matches[1] == string("This"));
        assert(matches[2] == string("is"));
    }

    static void test_regx_match_alphabets() {
        string str = "Thisisateststringwithsomenumbersandallsorts of special characters!#@$%^&*()_+";
        vector<string> matches;

        // Test for matching pattern that does not exist in the string
        assert(regx_match("[0-9]+", str, &matches) == 0);
        assert(matches.size() == 0);

        // Test for matching pattern that exists in the string
        assert(regx_match("test", str, &matches) == 1);
        assert(matches.size() == 1);
        assert(matches[0] == string("test"));

        // Test for matching pattern that appears multiple times in the string
        assert(regx_match("[a-z]+", str, &matches) == 1);
        assert(matches.size() == 1);
    }
};