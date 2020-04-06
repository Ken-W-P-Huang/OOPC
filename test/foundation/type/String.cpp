#include "gtest/gtest.h"

extern "C"{
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
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
    void destroy(struct Object *self);
}


TEST(TEST, String) {
    char* a = "abc";
    String* c = String_init((String*)alloc(StringClass),a);
    ASSERT_TRUE(strcmp(String_getValue(c),a) == 0);
    ASSERT_EQ(String_length(c),3);
    String* d = String_init((String*)alloc(StringClass),"efg");
    String* e = String_concat(c,d);
    ASSERT_TRUE(strcmp(String_getValue(e),"abcefg") == 0);
    ASSERT_EQ(String_charAt(c,2),'c');
}

TEST(String_trim, String) {
    String* c = String_init((String*)alloc(StringClass)," abc ");
    ASSERT_TRUE(strcmp(String_getValue(String_trim(c)),"abc") == 0);
}

TEST(String_toLowerCase, String) {
    String* c = String_init((String*)alloc(StringClass),"ABcDEF");
    String* d = String_init((String*)alloc(StringClass),"abcdef");
    printf("\n%s",String_getValue(String_toLowerCase(c)));
    ASSERT_TRUE(String_compareTo(String_toLowerCase(c),d) == 0);
    ASSERT_FALSE(String_compareTo(String_toLowerCase(c),c) == 0);
}

TEST(String_toUpperCase, String) {
    String* c = String_init((String*)alloc(StringClass),"abcdef");
    String* d = String_init((String*)alloc(StringClass),"ABCDEF");
    ASSERT_TRUE(String_compareTo(String_toUpperCase(c),d) == 0);
    ASSERT_FALSE(String_compareTo(String_toUpperCase(c),c) == 0);
}

TEST(String_subString, String) {
    String* c = String_init((String*)alloc(StringClass),"abcdef");
    ASSERT_TRUE(strcmp(String_getValue(String_subString(c,1,3)),"bcd"));
}

TEST(String_indexOf, String) {
    String* c = String_init((String*)alloc(StringClass),"abc");
    String* d = String_init((String*)alloc(StringClass),"bc");
    ASSERT_EQ(String_indexOf(c,d,0),1);
}

TEST(String_endsWith, String) {
    String* c = String_init((String*)alloc(StringClass),"abc");
    String* d = String_init((String*)alloc(StringClass),"bc");
    ASSERT_TRUE(String_endsWith(c,d));
    destroy((Object*)d);
    d = String_init((String*)alloc(StringClass),"efg");
    ASSERT_FALSE(String_startsWith(c,d,1));
}

TEST(String_compareTo, String) {
    String* c = String_init((String*)alloc(StringClass),"abc");
    String* d = String_init((String*)alloc(StringClass),"abcde");
    ASSERT_TRUE(String_compareTo(c,d) < 0);
    destroy((Object*)d);
    d = String_init((String*)alloc(StringClass),"aba");
    ASSERT_TRUE(String_compareTo(c,d) > 0);
    destroy((Object*)d);
    d = String_init((String*)alloc(StringClass),"abc");
    ASSERT_TRUE(String_compareTo(c,d) == 0);
}

TEST(String_startsWith, String) {
    String* c = String_init((String*)alloc(StringClass),"abc");
    String* d = String_init((String*)alloc(StringClass),"ab");
    ASSERT_TRUE(String_startsWith(c,d,0));
    destroy((Object*)d);
    d = String_init((String*)alloc(StringClass),"bc");
    ASSERT_TRUE(String_startsWith(c,d,1));
    destroy((Object*)d);
    d = String_init((String*)alloc(StringClass),"efg");
    ASSERT_FALSE(String_startsWith(c,d,1));
}