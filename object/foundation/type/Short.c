//
// Created by kenhuang on 2018/6/13.
//

#include "Short.h"
#include "Pointer.h"

private_start(Short, Object)
/* 私有属性 */
short value;
private_end
/* 静态属性 */
const short SHORT_MIN_VALUE=-32768;
const short SHORT_MAX_VALUE=32767;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(ShortClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(ShortClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Short* Short_new(Short *self, int value){
    if(!self){
        self = (Short*) Class_newInstance(ShortClazz);
    }
    self->private->value  = value;
    return self;
}
short Short_getValue(Short* self){
    return self->private->value ;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Short, Object,offsetof(Short,private), sizeof(__Short))

class_end
static void Class_init(Class1* class) {

}

static void Class_deinit(Class1* class) {

}
void ShortClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(ShortClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "value",ShortClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(ShortClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Short_new,READABLE_UNWRITABLE_PUBLIC,
                              Short_getValue,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}