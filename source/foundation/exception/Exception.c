//
// Created by kenhuang on 19-10-20.
//

#include "Exception.h"

PRIVATE(Exception)
    jmp_buf buffer;
END
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Exception *self) {

}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Exception* Exception_init(Exception *self){
    if(Object_init((Object*)self)){

    }
    return self;
}

jmp_buf* Exception_getBuffer(Exception* self){
    return &(self->p->buffer);
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Exception)
    Class_fillDeclaredFields((ClassType *) ExceptionClass,
                             "message", StringClass, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                             "type", IntegerClass, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                             "buffer", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) ExceptionClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Exception_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Exception_getBuffer, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END