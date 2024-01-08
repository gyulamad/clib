#pragma once

#include <cassert>

#include "../../clib/VectorFactory.hpp"

using namespace std;
using namespace clib;

class VectorFactoryTests {
public:

    static void test_VectorFactory_withParams_Int() {
        VectorFactory<int> intFactory;
        int* intInstance = intFactory.create(42);

        assert(*intInstance == 42);

        intFactory.destroy(intInstance);

        // After destroying, attempting to access the instance should fail.
        assert(*intInstance != 42);
    }

    static void test_VectorFactory_withParams_CustomClass() {
        // Additional test for objects of a custom class
        class MyObject {
        public:
            int value;
            explicit MyObject(int val) : value(val) {}
        };

        VectorFactory<MyObject> objFactory;
        MyObject* objInstance = objFactory.create(100);

        assert(objInstance->value == 100);

        objFactory.destroy(objInstance);

        // After destroying, attempting to access the instance should fail.
        assert(objInstance->value != 100);
    }

    static void test_VectorFactory_withoutParams() {
        // Test for classes without constructor parameters
        class NoParamClass {
        public:
            int value;
            NoParamClass() : value(999) {}  // Default value for testing
        };

        VectorFactory<NoParamClass> noParamFactory;
        NoParamClass* noParamInstance = noParamFactory.create();

        assert(noParamInstance->value == 999);

        noParamFactory.destroy(noParamInstance);

        // After destroying, attempting to access the instance should fail.
        assert(noParamInstance->value != 999);
    }
};