#pragma once

#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace clib {
    #define MS_PER_SEC  (1000ul)
    #define MS_PER_MIN  (60ul * MS_PER_SEC)
    #define MS_PER_HOUR (60ul * MS_PER_MIN)
    #define MS_PER_DAY  (24ul * MS_PER_HOUR)
    #define MS_PER_WEEK (7ul * MS_PER_DAY)

    // https://www.codeproject.com/Articles/5329247/How-to-Change-Text-Color-in-a-Linux-Terminal
    #define TEXT_BOLD "\033[1m"
    #define TEXT_DIM "\033[2m"
    #define TEXT_UNDERLINE "\033[4m"
    #define TEXT_BLINK "\033[5m"
    #define TEXT_REVERSE "\033[7m"
    #define TEXT_CONCEALED "\033[8m"

    #define TEXT_RESET_BOLD_DIM "\033[22m"
    #define TEXT_RESET_UNDERLINE "\033[24m"
    #define TEXT_RESET_BLINK "\033[25m"
    #define TEXT_RESET_REVERSE "\033[27m"
    #define TEXT_RESET_CONCEALED "\033[28m"

    #define COLOR_BLACK "\033[30m"
    #define COLOR_BLUE "\033[34m"
    #define COLOR_GREEN "\033[32m"
    #define COLOR_CYAN "\033[36m"
    #define COLOR_RED "\033[31m"
    #define COLOR_MAGENTA "\033[35m"
    #define COLOR_YELLOW "\033[33m"
    #define COLOR_WHITE "\033[37m"
    
    #define COLOR_GRAY "\033[90m"
    #define COLOR_BRIGHT_BLUE "\033[94m"
    #define COLOR_BRIGHT_GREEN "\033[92m"
    #define COLOR_BRIGHT_CYAN "\033[96m"
    #define COLOR_BRIGHT_RED "\033[91m"
    #define COLOR_BRIGHT_MAGENTA "\033[95m"
    #define COLOR_BRIGHT_YELLOW "\033[93m"
    #define COLOR_DARK_GRAY "\033[90m"

    #define BACKGROUND_BLACK "\033[40m"
    #define BACKGROUND_RED "\033[41m"
    #define BACKGROUND_GREEN "\033[42m"
    #define BACKGROUND_YELLOW "\033[43m"
    #define BACKGROUND_BLUE "\033[44m"
    #define BACKGROUND_MAGENTA "\033[45m"
    #define BACKGROUND_CYAN "\033[46m"
    #define BACKGROUND_DARK_GRAY "\033[47m"

    #define BACKGROUND_GRAY "\033[100m"
    #define BACKGROUND_BRIGHT_RED "\033[101m"
    #define BACKGROUND_BRIGHT_GREEN "\033[102m"
    #define BACKGROUND_BRIGHT_YELLOW "\033[103m"
    #define BACKGROUND_BRIGHT_BLUE "\033[104m"
    #define BACKGROUND_BRIGHT_MAGENTA "\033[105m"
    #define BACKGROUND_BRIGHT_CYAN "\033[106m"
    #define BACKGROUND_WHITE "\033[107m"

    #define COLOR_DEFAULT "\033[0;0;0m"
    
    #define COLOR_ERROR     TEXT_BOLD COLOR_BRIGHT_RED
    #define COLOR_ALERT     TEXT_BOLD COLOR_BRIGHT_RED
    #define COLOR_WARNING   COLOR_BRIGHT_YELLOW
    #define COLOR_INFO      COLOR_BRIGHT_CYAN
    #define COLOR_SUCCESS   TEXT_BOLD COLOR_GREEN
    #define COLOR_DEBUG     COLOR_GREEN
    #define COLOR_FILENAME  COLOR_GRAY
    #define COLOR_DATETIME  COLOR_DARK_GRAY
    #define COLOR_HIGHLIGHT TEXT_BOLD BACKGROUND_GRAY COLOR_BLACK

    #define QUOTEME_1(x) #x
    #define QUOTEME(x) QUOTEME_1(x)

}