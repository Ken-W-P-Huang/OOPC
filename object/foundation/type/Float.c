//
// Created by kenhuang on 2018/6/13.
//

#include "Float.h"
#include "Pointer.h"

private_start(Float, Object)
/* 私有属性 */
float value;
private_end
/* 静态属性 */
const float FLOAT_MIN_VALUE=1.4E-45;
const float FLOAT_MAX_VALUE=3.4028235E38;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(FloatClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(FloatClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Float* Float_new(Float*self,char character){
    if(!self){
        self = Class_newInstance(FloatClazz);
    }
    self->private->value = character;
    return self;
}
float Float_getValue(Float*self){
    return self->private->value;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Float, Object,offsetof(Float,private), sizeof(__Float))

class_end
static void Class_init(Class1* class) {

}

static void Class_deinit(Class1* class) {

}
void FloatClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(FloatClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "value",FloatClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(FloatClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Float_new,READABLE_UNWRITABLE_PUBLIC,
                              Float_getValue,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}