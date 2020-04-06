#include "gtest/gtest.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const CharacterClass;
    struct Character *Character_init(Character *self,char character);
    char Character_getValue(Character *self);
}

TEST(TEST, Character) {
    Character* c = Character_init((Character*)alloc(CharacterClass),'a');
    ASSERT_EQ(Character_getValue(c),'a');
}