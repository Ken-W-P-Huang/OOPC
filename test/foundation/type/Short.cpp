#include "gtest/gtest.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const ShortClass;
    struct Short *Short_init(Short *self, short Short);
    short Short_getValue(Short *self);
}

TEST(TEST, Short) {
    Short* c = Short_init((Short*)alloc(ShortClass),10);
    ASSERT_EQ(Short_getValue(c),10);
}