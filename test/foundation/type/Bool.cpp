#include "gtest/gtest.h"
extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const BoolClass;
    struct Bool *Bool_init(Bool *self,bool value);
    bool Bool_getValue(Bool*self);
}

TEST(TEST, Bool) {
    printf("ggg");
    Bool* b = Bool_init((Bool*)alloc(BoolClass), false);
    ASSERT_EQ(Bool_getValue(b), false);
}