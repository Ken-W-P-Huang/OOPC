#include "gtest/gtest.h"
#include "type.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const BoolClass;
    extern ClassType *const ByteClass;
    struct Byte *Byte_init(Byte *self,byte value);
    byte Byte_getValue(Byte*self);
}


TEST(TEST, Byte) {
    Byte* b = Byte_init((Byte*)alloc(ByteClass),255);
    ASSERT_EQ(Byte_getValue(b),255);
}