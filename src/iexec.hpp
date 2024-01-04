#pragma once

#include <string>
#include <iostream>

#include "../clib/macros.hpp"
#include "../clib/sys.hpp"

using namespace std;
using namespace clib;

bool iexec(const string& cmd) {
    cout << COLOR_INFO "Execute command:" COLOR_DEFAULT " $ " << cmd << endl;
    string output = exec(cmd);    
    if (!output.empty()) {
        string replaced = output;
        replaced = str_replace(replaced, "error:", COLOR_ERROR "error:" COLOR_DEFAULT);
        cout << replaced << endl;
        if (replaced != output) {
            LOGE("Command output contains error(s).");
            return false;
        }
    }
    if (exec_last_exit_code != 0) {
        LOGE("Command exit code is non-zero: " + to_string(exec_last_exit_code));
        return false;
    }
    return true;
}