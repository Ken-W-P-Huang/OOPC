#include "gtest/gtest.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const DoubleClass;
    struct Double *Double_init(Double *self, double value);
    double Double_getValue(Double *self);
}

TEST(TEST, Double) {
    Double* d = Double_init((Double*)alloc(DoubleClass),10.0);
    ASSERT_EQ(Double_getValue(d),10.0);
}