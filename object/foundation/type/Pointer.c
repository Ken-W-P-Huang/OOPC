//
// Created by kenhuang on 2018/6/13.
//

#include "Pointer.h"
private_start(Pointer, Object)
/* 私有属性 */
void* value;
private_end
/* 静态属性 */

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(PointerClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(PointerClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Pointer* Pointer_new(Pointer* self,void * value){
    if(!self){
        self = Class_newInstance(PointerClazz);
    }
    self->private->value = value;
    return  self;
}
void* Pointer_getValue(Pointer* self){
    return  self->private->value ;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Pointer, Object,offsetof(Pointer,private), sizeof(__Pointer))

class_end
static void Class_init(Class1 *class) {

}

static void Class_deinit(Class1 *class) {

}
void PointerClazz_fillFieldsMethods(){
        Class_fillDeclaredFields(PointerClazz,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "value",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(PointerClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  Pointer_new,READABLE_UNWRITABLE_PUBLIC,
                                  Pointer_getValue,READABLE_UNWRITABLE_PUBLIC,
                                  NULL);
}