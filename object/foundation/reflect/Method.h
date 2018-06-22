//
// Created by kenhuang on 2018/6/6.
//
#include "../Object.h"
#include "Access.h"
#ifndef  Method_H
#define  Method_H

/***********************************************************************************************************************
 *  Method类
 **********************************************************************************************************************/
/*  类型声明*/
typedef long (* Function)();
public_start(Method,Object)
    struct __Method* private;
public_end

/* 公共方法*/
Method * Method_new(Method *self, const Class1 *clazz, String *name, Function method);
const String * Method_getName(Method *self);
Access* Method_getAccess(Method * self);
Function Method_getMethod(Method *self);
/* 静态方法 */
void MethodClazz_fillFieldsMethods();
#endif //Method_H