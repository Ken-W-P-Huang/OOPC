#include "gtest/gtest.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const IntegerClass;
    extern struct Integer *Integer_init(Integer *self, int value);
    extern int Integer_getValue(Integer* self);
}

TEST(TEST, Integer) {
    Integer* integer = Integer_init((Integer*)alloc(IntegerClass),1);
    ASSERT_EQ(Integer_getValue(integer),1);
}