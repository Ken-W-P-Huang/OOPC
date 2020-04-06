//
// Created by kenhuang on 2019-10-21.
//

#ifndef OOPC_METHOD_H
#define OOPC_METHOD_H
#include "../Object.h"
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
#include "Access.h"
#include "../type/String.h"
PUBLIC(Method,Object)
END

PUBLIC_STATIC(Method,Object)
#if defined(__POLYMORPHISM__)
    Method *(*init)(Method *self, const ClassType *clazz, char *name, Function method);
    const char * (*getName)(Method *self);
    struct Access* (*getAccess)(Method * self);
    Function (*getMethod)(Method *self);
#endif
END

Method *Method_init(Method *self, const ClassType *clazz,struct String *name, Function method);
const struct String * Method_getName(Method *self);
struct Access* Method_getAccess(Method * self);
Function Method_getMethod(Method *self);
#endif
#endif //OOPC_METHOD_H
