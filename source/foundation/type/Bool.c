//
// Created by kenhuang on 2019-10-21.
//

#include "Bool.h"
PRIVATE(Bool)
    bool value;
END
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Bool *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Bool *Bool_init(Bool *self,bool value) {
    if (Object_init((Object*)self)) {
        self->p->value = value;
    }
    return self;
}
bool Bool_getValue(Bool*self){
    return  self->p->value;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Bool)
    Class_fillDeclaredFields((ClassType *) BoolClass,
                             "value", BoolClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) BoolClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Bool_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
    Class_addFunctions((ClassType *) BoolClass, Bool_init, Bool_getValue, NULL);
    Bool** b = (Bool**)&(BoolClass->TRUE);
    *b = Bool_init(alloc((ClassType*)BoolClass),true);
    b = (Bool**)&(BoolClass->FALSE);
    *b = Bool_init(alloc((ClassType*)BoolClass),false);
END
