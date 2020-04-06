//
// Created by kenhuang on 2019-10-23.
//

#include "Short.h"
PRIVATE(Short)
    short value;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Short *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Short *Short_init(Short *self, short value) {
    if (Object_init((Object*)self)) {
        self->p->value  = value;
    }
    return self;
}
short Short_getValue(Short* self){
    return self->p->value ;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Short)
    Class_fillDeclaredFields((ClassType *) ShortClass,
                             "value", ShortClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) ShortClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Short_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Short_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    short* s = (short*)&ShortClass->MIN_VALUE;
    *s = -32768;
    s = (short*)&ShortClass->MAX_VALUE;
    *s = 32767;
END
