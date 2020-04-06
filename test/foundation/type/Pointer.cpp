#include "gtest/gtest.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const PointerClass;
    struct Pointer *Pointer_init(Pointer *self, void* Pointer);
    void* Pointer_getValue(Pointer *self);
}

TEST(TEST, Pointer) {
    Pointer* c = Pointer_init((Pointer*)alloc(PointerClass),(void*)Class);
    ASSERT_EQ(Pointer_getValue(c),Class);
}