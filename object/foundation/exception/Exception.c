//
// Created by kenhuang on 2018/6/8.
//


#include "Exception.h"
#include "../type/Integer.h"
#include "../type/Pointer.h"
/* 类型声明 */

/* 私有属性 */
private_start(Exception,Object)
    jmp_buf buffer;
private_end

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object *init(Object *self, va_list *parms) {
    return Class_getSuperClass(ExceptionClazz)->init(self, parms);
}

static Object *deinit(Object *self) {
    return Class_getSuperClass(ExceptionClazz)->deinit(self);

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Exception* Exception_new(Exception *self){
    if(!self){
        self=Class_newInstance(ExceptionClazz);
    }
    return self;
}
jmp_buf* Exception_getBuffer(Exception* self){
    return &(self->private->buffer);
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(Exception,Object,offsetof(Exception,private), sizeof(__Exception))
class_end
static void Class_init(Class1* class){
    if (isReflectable) {
        Class_fillDeclaredFields(ExceptionClazz,
                                 "message",StringClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "type",IntegerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "buffer",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(ExceptionClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  Exception_new,READABLE_UNWRITABLE_PUBLIC,
                                  Exception_getBuffer,READABLE_UNWRITABLE_PUBLIC,
                                  NULL);
    }
}
static void Class_deinit(Class1* class){

}

