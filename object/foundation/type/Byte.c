//
// Created by kenhuang on 2018/6/13.
//

#include "Byte.h"
#include "Pointer.h"

private_start(ByteT, Object)
/* 私有属性 */
    byte value;
private_end
/* 静态属性 */
const byte BYTE_MIN_VALUE=-128;
const byte BYTE_MAX_VALUE=127;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(ByteTClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(ByteTClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
ByteT* Byte_new(ByteT*self,byte value){
    if(!self){
        self = Class_newInstance(ByteTClazz);
    }
    self->private->value = value;
    return self;
}
byte Byte_getValue(ByteT*self){
    return self->private->value;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(ByteT, Object,offsetof(ByteT,private), sizeof(__ByteT))

class_end
static void Class_init(Class1* class) {

}

static void Class_deinit(Class1* class) {

}
void ByteTClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(ByteTClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "value",ByteTClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(ByteTClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Byte_new,READABLE_UNWRITABLE_PUBLIC,
                              Byte_getValue,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}