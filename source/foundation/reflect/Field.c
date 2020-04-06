//
// Created by kenhuang on 2019-10-21.
//

#include "Field.h"
#include "../type/String.h"
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
PRIVATE(Field)
    /*属于哪个类*/
    const ClassType* clazz;
    String* name;
    /*数据类型，如果是基本数据类型，则必须先包装成对象*/
    const ClassType* type;
    size_t length;
    size_t offset;
    Access* access;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Field *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Field *Field_init(Field *self, const ClassType *clazz, String *name, ClassType *type, size_t offset, size_t length, Access* access) {
    if (Object_init((Object*)self)) {
        Field_P* private = self->p;
        private->clazz = clazz;
        private->name = name;
        private->type = type;
        private->length = length;
        private->offset = offset;
        private->access = access;
    }
    return self;
}

String * Field_getName(Field *self){
    return  self->p->name;
}
const ClassType * Field_getType(Field *self){
    return self->p->type;
}

size_t Field_getOffset(Field *self){
    return self->p->offset;
}

size_t Field_getLength(Field *self){
    return self->p->length;
}

Access* Field_getAccess(Field *self){
    return self->p->access;
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Field)
    Class_fillDeclaredFields((ClassType*)FieldClass,
                             "clazz",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "name",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "type",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "length",IntegerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "offset",IntegerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "access",CharacterClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType*)FieldClass,
                              deinit,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Field_init,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Field_getName,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Field_getType,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Field_getOffset,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Field_getLength,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Field_getAccess,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END
#endif