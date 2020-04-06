//
// Created by kenhuang on 2019-10-21.
//
#ifndef OOPC_STRING_H
#define OOPC_STRING_H
#include "../Object.h"
#include "../interface/Comparable.h"
PUBLIC(String,Object)
END

PUBLIC_STATIC(String,Object)
    IMPLEMENTS(Comparable);
#if defined(__POLYMORPHISM__)
    String *(*init)(String *self,char* value);
    char* (*getValue)(String*self);
    String* (*initWithOffsetLength)(String* self,char* value,int offset,size_t length);
    String* (*trim)(String*self);
    String* (*toLowerCase)(String*self);
    String* (*toUpperCase)(String*self);
    String* (*subString)(String*self, unsigned int beginIndex, size_t endIndex);
    int (*indexOf)(String *self, String *str,unsigned int offset);
    bool  (*endsWith)(String*self,String*suffix);
    bool (*startsWith)(String *self, String *prefix, size_t offset);
    int (*compareTo)(String*self,String* str);
    char (*charAt)(String *self, unsigned int index);
    String* (*concat)(String*self,String* str);
    size_t (*length)(String*self);
#endif
END

String *String_init(String *self,char* value);
char* String_getValue(String*self);
String* String_initWithOffsetLength(String* self,char* value,int offset,size_t length);
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
#endif //OOPC_STRING_H
