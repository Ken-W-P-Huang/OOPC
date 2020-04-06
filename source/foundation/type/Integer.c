//
// Created by kenhuang on 2019-10-23.
//

#include "Integer.h"
PRIVATE(Integer)
    int value;
END
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Integer *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Integer *Integer_init(Integer *self, int value) {
    if (Object_init((Object*)self)) {
        self->p->value  = value;
    }
    return self;
}

int Integer_getValue(Integer* self){
    return self->p->value ;
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Integer)

    Class_fillDeclaredFields((ClassType *) IntegerClass,
                             "value", IntegerClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) IntegerClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Integer_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Integer_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    int * i = (int*)&IntegerClass->MIN_VALUE;
    *i = 0x80000000;
    i = (int*)&IntegerClass->MAX_VALUE;
    *i = 0x7fffffff;
END