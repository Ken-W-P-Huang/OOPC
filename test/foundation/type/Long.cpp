#include "gtest/gtest.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const LongClass;
    struct Long *Long_init(Long *self, long Long);
    long Long_getValue(Long *self);
}

TEST(TEST, Long) {
    Long* c = Long_init((Long*)alloc(LongClass),100);
    ASSERT_EQ(Long_getValue(c),100);
}