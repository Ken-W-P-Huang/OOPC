//
// Created by kenhuang on 2019-10-21.
//
#include "../Object.h"
#ifndef OOPC_FIELD_H
#define OOPC_FIELD_H
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
#include "Access.h"
#include "../type/String.h"
PUBLIC(Field,Object)
END

PUBLIC_STATIC(Field,Object)
#if defined(__POLYMORPHISM__)
    Field *(*init)(Field *self, const ClassType *clazz, struct String *name, ClassType *type, size_t offset, size_t length,
                      struct Access* access);
    char* (*getName)(Field *self);
    const ClassType * (*getType)(Field *self);
    struct Access* (*getAccess)(Field *self);
    size_t (*getLength)(Field *self);
    size_t (*getOffset)(Field *self);
#endif
END
Field *Field_init(Field *self, const ClassType *clazz, struct String *name, ClassType *type, size_t offset, size_t length,
                  struct Access* access);
struct String* Field_getName(Field *self);
const ClassType * Field_getType(Field *self);
struct Access* Field_getAccess(Field *self);
size_t Field_getLength(Field *self);
size_t Field_getOffset(Field *self);
#endif
#endif //OOPC_FIELD_H
