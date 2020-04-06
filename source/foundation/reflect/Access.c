//
// Created by kenhuang on 2019-10-21.
//

#include "Access.h"

PRIVATE(Access)
    short readable :1;
    short writable :1;
    short isPrivate :1;
END
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Access *self) {

}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Access *Access_init(Access *self,bool readable,bool writable,bool isPrivate) {
    if (Object_init((Object*)self)) {
        self->p->readable = readable;
        self->p->writable = writable;
        self->p->isPrivate = isPrivate;
    }
    return self;
}
bool Access_getReadable(Access* self){
    return self->p->readable;
}
bool Access_getWritable(Access* self){
    return self->p->writable;
}
bool Access_getIsPrivate(Access* self){
    return self->p->isPrivate;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Access)
    Access** access = (Access**)&AccessClass->READABLE_WRITABLE_PRIVATE;
    *access = Access_init(alloc((ClassType*)AccessClass),true,true,true);
    access = (Access**)&AccessClass->READABLE_UNWRITABLE_PRIVATE;
    *access = Access_init(alloc((ClassType*)AccessClass),true,false,true);
    access = (Access**)&AccessClass->READABLE_UNWRITABLE_PUBLIC;
    *access = Access_init(alloc((ClassType*)AccessClass),true,false,false);
    access = (Access**)&AccessClass->READABLE_WRITABLE_PUBLIC;
    *access = Access_init(alloc((ClassType*)AccessClass),true,true,false);
    access = (Access**)&AccessClass->UNREADABLE_WRITABLE_PRIVATE;
    *access = Access_init(alloc((ClassType*)AccessClass),false,true,true);
    access = (Access**)&AccessClass->UNREADABLE_UNWRITABLE_PRIVATE;
    *access = Access_init(alloc((ClassType*)AccessClass),false,false,true);
    access = (Access**)&AccessClass->UNREADABLE_UNWRITABLE_PUBLIC;
    *access = Access_init(alloc((ClassType*)AccessClass),false,false,false);
    access = (Access**)&AccessClass->UNREADABLE_WRITABLE_PUBLIC;
    *access = Access_init(alloc((ClassType*)AccessClass),false,true,false);
    Class_fillDeclaredFields((ClassType *) AccessClass,
                             "access", ShortClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) AccessClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Access_init, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Access_getReadable, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Access_getWritable, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Access_getIsPrivate, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END
