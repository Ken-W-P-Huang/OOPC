//
// Created by kenhuang on 2018/6/13.
//

#include "Long.h"
#include "Pointer.h"

private_start(Long, Object)
/* 私有属性 */
long value;
private_end
/* 静态属性 */
const long LONG_MIN_VALUE=0x8000000000000000;
const long LONG_MAX_VALUE=0x7fffffffffffffff;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(LongClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(LongClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Long* Long_new(Long *self, int value){
    if(!self){
        self = (Long*) Class_newInstance(LongClazz);
    }
    self->private->value  = value;
    return self;
}
long Long_getValue(Long* self){
    return self->private->value ;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Long, Object,offsetof(Long,private), sizeof(__Long))
class_end
static void Class_init(Class1* class) {
    if (isReflectable) {

    }
}

static void Class_deinit(Class1* class) {

}
void LongClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(LongClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "value",LongClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(LongClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Long_new,READABLE_UNWRITABLE_PUBLIC,
                              Long_getValue,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}