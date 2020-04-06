//
// Created by kenhuang on 2019-10-23.
//

#include "Float.h"
PRIVATE(Float)
    float value;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Float *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Float *Float_init(Float *self, float value) {
    if (Object_init((Object*)self)) {
        self->p->value = value;
    }
    return self;
}

float Float_getValue(Float*self){
    return self->p->value;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Float)

    Class_fillDeclaredFields((ClassType *) FloatClass,
                             "value", FloatClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) FloatClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Float_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Float_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    float* f = (float*)&(FloatClass->MAX_VALUE);
    *f = (float)3.4028235E38;
    f = (float*)&(FloatClass->MIN_VALUE);
    *f = 1.4E-45;
END