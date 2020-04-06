//
// Created by kenhuang on 2019-10-23.
//

#include "Byte.h"

PRIVATE(Byte)
    byte value;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Byte *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Byte *Byte_init(Byte *self,byte value) {
    if (Object_init((Object*)self)) {
        self->p->value = value;
    }
    return self;
}
byte Byte_getValue(Byte*self){
    return self->p->value;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Byte)
    Class_fillDeclaredFields((ClassType *) ByteClass,
                             "value", ByteClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) ByteClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Byte_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Byte_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    byte *b = (byte*)&(ByteClass->MIN_VALUE);
    *b = 0;
    b = (byte*)&(ByteClass->MAX_VALUE);
    *b = 255;
END