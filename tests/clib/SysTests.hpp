#pragma once

#include <cassert>

#include "../../clib/sys.hpp"
#include "../../clib/str.hpp"

using namespace std;
using namespace clib;

class SysTests {
public:

    static void test_exec() {
        // Test capturing output
        string output = exec("echo 'Hello, World!'", true);
        assert(output == "Hello, World!\n");

        // Test capturing error
        string error = exec("ls /nonexistentfolder", true);
        assert(error == "ls: cannot access '/nonexistentfolder': No such file or directory\n");  // Error message should not be empty

        // Test not capturing output and error
        string neither = exec("echo 'Hello, World!'", false);
        assert(neither.empty());
        assert(exec_last_exit_code == 0);

        // Test running a valid command that should not throw
        try {
            exec("ls");
        } catch (const exception& e) {
            // This should not throw an exception
            assert(false);
        }
    }

};