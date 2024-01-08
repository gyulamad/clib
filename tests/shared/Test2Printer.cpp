#include <iostream>

#include "../../clib/Printer.hpp"

using namespace std;
using namespace clib;

namespace madlib::printer {

    class Test2Printer: public Printer {
    public:
        using Printer::Printer;
        virtual void print(const string& output) override {
            cout << "Test2Printer prints: " << output;
        }
    };

    extern "C" Test2Printer* createTest2Printer(const string& prefix = nullptr) {
        return new Test2Printer(prefix);
    }
}