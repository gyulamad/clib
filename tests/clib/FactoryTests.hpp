#pragma once

#include <cassert>
#include <sstream>
#include <iostream>

#include "../../clib/files.hpp"
#include "../../clib/Factory.hpp"
#include "../../clib/Printer.hpp"

using namespace std;
using namespace clib;

class FactoryTests {
public:

    static void test_Factory_shared_lib() {
        // Redirect standard output to a stringstream
        stringstream buffer;
        streambuf* original_cout = cout.rdbuf(buffer.rdbuf());

        Factory factory = Factory<Printer>();

        Printer* printer1 = factory.createInstance(__BUILD_DIR__ "tests/shared/Test1Printer.so");
        printer1->println("Printer1 is printing");
        assert(buffer.str() == "Test1Printer prints: Printer1 is printing\n");

        buffer.str("");  // Clear the buffer for reuse
        
        Printer* printer2 = factory.createInstance(__BUILD_DIR__ "tests/shared/Test2Printer.so");
        printer2->println("Printer2 is printing");
        assert(buffer.str() == "Test2Printer prints: Printer2 is printing\n");

        // Restore the original cout
        cout.rdbuf(original_cout);
    }
};