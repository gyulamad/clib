#pragma once

#include <cassert>

#include "../../clib/maps.hpp"

using namespace std;
using namespace clib;

class MapsTests {
public:

    static void test_map_has() {
        map<int, string> intMap;
        intMap[1] = "One";
        intMap[2] = "Two";

        // Test map_has with an integer key
        assert(map_has(intMap, 1));
        assert(map_has(intMap, 2));
        assert(!map_has(intMap, 3));

        map<string, int> strMap;
        strMap["apple"] = 5;
        strMap["banana"] = 7;

        // Test map_has with a string key
        assert(map_has(strMap, "apple"));
        assert(map_has(strMap, "banana"));
        assert(!map_has(strMap, "cherry"));

        // Test with an empty integer map
        map<int, string> intMapEmpty;
        assert(!map_has(intMapEmpty, 1));
        assert(!map_has(intMapEmpty, 2));

        // Test with an empty string map
        map<string, int> strMapEmpty;
        assert(!map_has(strMapEmpty, "apple"));
        assert(!map_has(strMapEmpty, "banana"));
    }

    static void test_map_keys() {
        map<int, string> intMap;
        intMap[1] = "One";
        intMap[2] = "Two";

        vector<int> intKeys = map_keys(intMap);
        assert(intKeys.size() == 2);
        assert(intKeys[0] == 1);
        assert(intKeys[1] == 2);

        map<string, int> strMap;
        strMap["apple"] = 5;
        strMap["banana"] = 7;

        vector<string> strKeys = map_keys(strMap);
        assert(strKeys.size() == 2);
        assert(strKeys[0] == "apple");
        assert(strKeys[1] == "banana");

        // Test with an empty map
        map<int, string> emptyMap;
        vector<int> emptyKeys = map_keys(emptyMap);
        assert(emptyKeys.empty());
    }

    static void test_map_key_exists() {
        map<int, string> intMap;
        intMap[1] = "One";
        intMap[2] = "Two";

        assert(map_key_exists(intMap, 1));
        assert(map_key_exists(intMap, 2));
        assert(!map_key_exists(intMap, 3));

        map<string, int> strMap;
        strMap["apple"] = 5;
        strMap["banana"] = 7;

        assert(map_key_exists(strMap, "apple"));
        assert(map_key_exists(strMap, "banana"));
        assert(!map_key_exists(strMap, "cherry"));

        // Test with an empty map
        map<int, string> emptyMap;
        assert(!map_key_exists(emptyMap, 1));
        assert(!map_key_exists(emptyMap, 2));
    }
};