//
// Created by kenhuang on 2018/6/20.
//

#include "Access.h"
#include "../type/Pointer.h"
#include "../type/Character.h"

private_start(Access, Object)
/* 私有属性 */
    unsigned char readable :1;
    unsigned char writable :1;
    unsigned char isPrivate :1;
private_end
/* 静态属性 */
const Access* READABLE_WRITABLE_PRIVATE;
const Access* READABLE_UNWRITABLE_PRIVATE;
const Access* READABLE_UNWRITABLE_PUBLIC;
const Access* READABLE_WRITABLE_PUBLIC;
const Access* UNREADABLE_WRITABLE_PRIVATE;
const Access* UNREADABLE_UNWRITABLE_PRIVATE;
const Access* UNREADABLE_UNWRITABLE_PUBLIC;
const Access* UNREADABLE_WRITABLE_PUBLIC;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(AccessClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(AccessClazz)->deinit(self);
}
static Access* new(bool readable,bool writable,bool isPrivate ){
    Access* self = Class_newInstance(AccessClazz);
    self->private->readable = readable;
    self->private->writable = writable;
    self->private->isPrivate = isPrivate;
    return self;
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
bool Access_getReadable(Access* self){
    return self->private->readable;
}
bool Access_getWritable(Access* self){
    return self->private->writable;
}
bool Access_getIsPrivate(Access* self){
    return self->private->isPrivate;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Access,Object,offsetof(Access, private),sizeof(__Access))

class_end
static void Class_init(Class1 *class) {
    READABLE_WRITABLE_PRIVATE = new(true,true,true);
    READABLE_UNWRITABLE_PRIVATE = new(true,false,true);
    READABLE_UNWRITABLE_PUBLIC= new(true,false,false);
    READABLE_WRITABLE_PUBLIC= new(true,true,false);
    UNREADABLE_WRITABLE_PRIVATE= new(false,true,true);
    UNREADABLE_UNWRITABLE_PRIVATE= new(false,false,true);
    UNREADABLE_UNWRITABLE_PUBLIC= new(false,false,false);
    UNREADABLE_WRITABLE_PUBLIC= new(false,true,false);
}

static void Class_deinit(Class1 *class) {

}
void AccessClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(AccessClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "access",CharacterClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(AccessClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              new,READABLE_UNWRITABLE_PRIVATE,
                              Access_getReadable,READABLE_UNWRITABLE_PUBLIC,
                              Access_getWritable,READABLE_UNWRITABLE_PUBLIC,
                              Access_getIsPrivate,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}
