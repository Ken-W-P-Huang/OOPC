//
// Created by kenhuang on 2019-10-23.
//

#include "Double.h"

PRIVATE(Double)
    double value;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Double *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Double *Double_init(Double *self, double value) {
    if (Object_init((Object*)self)) {
        self->p->value = value;
    }
    return self;
}

double Double_getValue(Double *self){
    return self->p->value;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Double)
    Class_fillDeclaredFields((ClassType *) DoubleClass,
                             "value", DoubleClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) DoubleClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Double_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Double_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    double* d = (double*)&(DoubleClass->MAX_VALUE);
    *d = 1.7976931348623157E308;
    d = (double*)&(DoubleClass->MIN_VALUE);
    *d = 4.9E-324;
END