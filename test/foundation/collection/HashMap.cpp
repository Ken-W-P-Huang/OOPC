#include "gtest/gtest.h"

extern "C"{
    extern  struct ClassType * const ObjectClass;
    extern struct ClassType * const Class;
    extern struct HashMapClassType * const HashMapClass;
    void * alloc (const ClassType * clazz);
    extern ClassType *const LinkedListClass;
    struct Bool *Bool_init(Bool *self,bool value);
    extern struct BoolClassType *const BoolClass;
    bool Bool_getValue(Bool*self);
    void destroy(struct Object *self);
    struct HashMap *HashMap_init(HashMap *self);
    HashMap *HashMap_initWith(HashMap *self, unsigned int initialCapacity, float loadFactor);
    Object* HashMap_get(HashMap *self,Object* key);
    unsigned int HashMap_getSize(HashMap * self);
    bool HashMap_containsKey(HashMap * map,Object * key);
    bool HashMap_containsValue(HashMap * map,Object *  Value);
    void HashMap_put(HashMap * self, Object * key, Object * value);
    Object* HashMap_remove(HashMap * self,Object* key);
    extern ClassType *const StringClass;
    struct String *String_init(String *self, char* String);
    char* String_getValue(String *self);
    String* String_trim(String*self);
    String* String_toLowerCase(String*self);
    String* String_toUpperCase(String*self);
    String* String_subString(String*self, unsigned int beginIndex, size_t endIndex);
    int String_indexOf(String *self, String *str,unsigned int offset);
    bool  String_endsWith(String*self,String*suffix);
    bool String_startsWith(String *self, String *prefix, size_t offset);
    int String_compareTo(String*self,String* str);
    char String_charAt(String *self, unsigned int index);
    String* String_concat(String*self,String* str);
    size_t String_length(String*self);
        void HashMap_clear(HashMap * self);
    struct Integer *Integer_init(Integer *self, int value);
    int Integer_getValue(Integer* self);
    extern ClassType *const IntegerClass;
    bool HashMapEntryIterator_hasNext(struct HashMapEntryIterator *self);
    struct HashMapEntry * HashMapEntryIterator_next(HashMapEntryIterator *self);
}

TEST(TEST, HashMap) {
    HashMap* map = HashMap_init((HashMap*)alloc((ClassType*)HashMapClass));
    struct Bool* TRUE = Bool_init((Bool *)alloc((ClassType*)BoolClass),true);
    struct Bool* FALSE = Bool_init((Bool *)alloc((ClassType*)BoolClass), false);
    struct String* h = String_init((String*)alloc((ClassType*)StringClass),"Hello");
    struct String* h1 = String_init((String*)alloc((ClassType*)StringClass),"what");
//    struct String* a = String_init((String*)alloc((ClassType*)StringClass),"What");
    HashMap_put(map,(Object*)h,(Object*)TRUE);
    ASSERT_TRUE((Bool*)HashMap_get(map,(Object*)h) == TRUE);
    ASSERT_TRUE(HashMap_getSize(map) == 1);

    HashMap_put(map,(Object*)h,(Object*)FALSE);
    ASSERT_TRUE((Bool*)HashMap_get(map,(Object*)h) == FALSE);
    ASSERT_TRUE(HashMap_getSize(map) == 1);

    HashMap_put(map,(Object*)h1,(Object*)FALSE);
    ASSERT_TRUE((Bool*)HashMap_get(map,(Object*)h1) == FALSE);
    ASSERT_TRUE(HashMap_getSize(map) == 2);
    for (int i = 0; i < 100; ++i) {
        Integer* integer = Integer_init((Integer*)alloc(IntegerClass),i);
        HashMap_put(map,(Object*)integer,(Object*)integer);
    }
    ASSERT_TRUE(HashMap_getSize(map) == 102);
    Bool* value = (Bool*) HashMap_remove(map,(Object*)h1);
    ASSERT_TRUE(value == FALSE);
    destroy((Object*)map);
}