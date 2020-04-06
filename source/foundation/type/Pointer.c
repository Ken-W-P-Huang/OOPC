//
// Created by kenhuang on 2019-10-23.
//

#include "Pointer.h"
PRIVATE(Pointer)
    void* value;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Pointer *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Pointer *Pointer_init(Pointer *self,void * value) {
    if (Object_init((Object*)self)) {
        self->p->value = value;
    }
    return self;
}

void* Pointer_getValue(Pointer* self){
    return  self->p->value ;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Pointer)

    Class_fillDeclaredFields((ClassType *) PointerClass,
                             "value", PointerClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) PointerClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Pointer_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Pointer_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END