//
// Created by kenhuang on 2019-12-02.
//

#include "SizeType.h"

PRIVATE(SizeType)
    size_t value;
END
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(SizeType *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
SizeType *SizeType_init(SizeType *self,size_t value) {
    if (Object_init((Object *) self)) {
        self->p->value = value;
    }
    return self;
}

size_t SizeType_getValue(SizeType* self){
    return self->p->value ;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(SizeType)
    Class_fillDeclaredFields((ClassType *) SizeTypeClass,
                             "value", SizeTypeClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) SizeTypeClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              SizeType_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              SizeType_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END