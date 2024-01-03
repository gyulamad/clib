#pragma once

#include <stdexcept>
#include "macros.hpp"
#include "str.hpp"

using namespace std;

namespace clib {
    #define ERROR(msg) runtime_error(COLOR_ERROR "Error: " __FILE_LINE__ + " " + string(msg));
}
