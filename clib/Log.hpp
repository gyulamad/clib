#pragma once

#include <iostream>
#include "Printer.hpp"
#include "time.hpp"
#include "files.hpp"
#include "str.hpp"

using namespace std;

namespace clib {

#define LOG(...) Log::getLogger().date().writeln(__VA_ARGS__)
#define LOGE(...) Log::getLogger().date().writeln(COLOR_ERROR "[ERROR] ", __VA_ARGS__, COLOR_DEFAULT)
#define LOGA(...) Log::getLogger().date().writeln(COLOR_ALERT "[ALERT] ", __VA_ARGS__, COLOR_DEFAULT)   
#define LOGW(...) Log::getLogger().date().writeln(COLOR_WARNING "[WARNING] ", __VA_ARGS__, COLOR_DEFAULT) 
#define LOGI(...) Log::getLogger().date().writeln(COLOR_INFO "[INFO] ", __VA_ARGS__, COLOR_DEFAULT) 
#define LOGS(...) Log::getLogger().date().writeln(COLOR_SUCCESS "[SUCCESS] ", __VA_ARGS__, COLOR_DEFAULT)

#define DBG(...) Log::getLogger().date().writeln(__FILE_LINE__, " ", COLOR_DEBUG, __VA_ARGS__, COLOR_DEFAULT)

#define LOG_DEFAULT_FILE "app.log"

    class Log: public Printer {
    protected:
        string filename;

    public:

        Log(const string& f = LOG_DEFAULT_FILE): Printer(), filename(f) {}

        static Log getLogger(const string& f = LOG_DEFAULT_FILE) {
            Log logger(f);
            return logger;
        }

        Log& date() {
            write(__DATE_TIME__);
            return *this;
        }

        // Variadic template for writeln method
        template <typename... Args>
        Log& writeln(const Args&... args) {
            // Concatenate all arguments into a single string
            string message = concat(args...);
            write(message + "\n");
            return *this;
        }

        // Variadic template for writeln method
        template <typename... Args>
        Log& write(const Args&... args) {
            // Concatenate all arguments into a single string
            string message = concat(args...);
            print(message);
            
            return *this;
        }

        virtual void print(const string& message) override {
            file_put_contents(filename, message, true);
        }

    };// logger;

}