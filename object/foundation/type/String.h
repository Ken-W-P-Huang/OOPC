//
// Created by kenhuang on 2018/6/7.
//
#include "../Object.h"
#ifndef  String_H
#define  String_H

/* 类型声明 */
public_start(String,Object)
    struct __String* private;
public_end
/* 公开方法*/
String* String_new(String* self,char* string);
char* String_getValue(String*self);
String* String_newWithOffsetLength(String* self,char* value,int offset,int length);
/* 静态方法 */
void StringClazz_fillFieldsMethods();

#endif //String_H