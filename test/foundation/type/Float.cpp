#include "gtest/gtest.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const FloatClass;
    struct Float *Float_init(Float *self, float Float);
    float Float_getValue(Float *self);
}

TEST(TEST, Float) {
    Float* c = Float_init((Float*)alloc(FloatClass),20.0f);
    ASSERT_EQ(Float_getValue(c),20.0);
}