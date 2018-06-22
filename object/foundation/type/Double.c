//
// Created by kenhuang on 2018/6/13.
//

#include "Double.h"
#include "Pointer.h"

private_start(Double, Object)
/* 私有属性 */
double value;
private_end
/* 静态属性 */
const double DOUBLE_MIN_VALUE=4.9E-324;
const double DOUBLE_MAX_VALUE=1.7976931348623157E308;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(DoubleClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(DoubleClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Double* Double_new(Double*self,char character){
    if(!self){
        self = Class_newInstance(DoubleClazz);
    }
    self->private->value = character;
    return self;
}
double Double_getValue(Double *self){
    return self->private->value;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Double, Object,offsetof(Double,private), sizeof(__Double))

class_end
static void Class_init(Class1* class) {
}

static void Class_deinit(Class1* class) {

}
void DoubleClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(DoubleClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "value",DoubleClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(DoubleClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Double_new,READABLE_UNWRITABLE_PUBLIC,
                              Double_getValue,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}