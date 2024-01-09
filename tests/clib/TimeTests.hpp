#pragma once

#include <cassert>

#include "../../clib/time.hpp"
#include "../../clib/str.hpp"

using namespace std;
using namespace clib;

class TimeTests {
public:

    static void test_datetime_conversion() {
        // Test cases
        const string testDateTime0 = "2023-01-15 08:30:45.123";
        const string testDateTime1 = "2023-01-15 08:30:45";
        const string testDateTime2 = "2022-05-02 12:23";
        const string testDateTime3 = "2021-06-10";

        // Convert datetime strings to milliseconds and back to datetime
        ms_t ms0 = datetime_to_ms(testDateTime0);
        string resultDateTime0 = ms_to_datetime(ms0);
        assert(1673771445123 == ms0);
        assert(resultDateTime0 == testDateTime0);

        ms_t ms1 = datetime_to_ms(testDateTime1);
        string resultDateTime1 = ms_to_datetime(ms1);
        assert(1673771445000 == ms1);
        assert(resultDateTime1 == normalize_datetime(testDateTime1));

        ms_t ms2 = datetime_to_ms(testDateTime2);
        string resultDateTime2 = ms_to_datetime(ms2);
        assert(1651494180000 == ms2);
        assert(resultDateTime2 == normalize_datetime(testDateTime2));

        ms_t ms3 = datetime_to_ms(testDateTime3);
        string resultDateTime3 = ms_to_datetime(ms3);
        assert(1623283200000 == ms3);
        assert(resultDateTime3 == normalize_datetime(testDateTime3));
    }
};