//
// Created by kenhuang on 2019-10-23.
//

#include "Long.h"
PRIVATE(Long)
    long value;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Long *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Long *Long_init(Long *self, long value) {
    if (Object_init((Object*)self)) {
        self->p->value  = value;
    }
    return self;
}

long Long_getValue(Long* self){
    return self->p->value ;
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Long)

    Class_fillDeclaredFields((ClassType *) LongClass,
                             "value", LongClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) LongClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Long_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Long_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    long* l = (long*)&LongClass->MIN_VALUE;
    *l = 0x8000000000000000;
    l = (long*)&LongClass->MAX_VALUE;
    *l = 0x7fffffffffffffff;
END