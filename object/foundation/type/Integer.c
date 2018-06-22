//
// Created by kenhuang on 2018/6/7.
//

#include "Integer.h"
#include "Pointer.h"

private_start(Integer,Object)
    int value;
private_end
/* 静态属性 */
const int INTEGER_MIN_VALUE = 0x80000000;
const int INTEGER_MAX_VALUE = 0x7fffffff;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object* init(Object*self,va_list* parms){
    return Class_getSuperClass(IntegerClazz)->init(self,parms);
}
static Object* deinit(Object*self){
    return Class_getSuperClass(IntegerClazz)->deinit(self);
}


/***********************************************************************************************************************
 *  公开方法
 **********************************************************************************************************************/


size_t IntegerClazz_getSize(){
    return sizeof(int);
}
Integer* Integer_new(Integer *self, int value){
    if(!self){
        self = (Integer*) Class_newInstance(IntegerClazz);
    }
    self->private->value  = value;
    return self;
}
int Integer_getValue(Integer* self){
    return self->private->value ;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(Integer,Object,offsetof(Integer,private), sizeof(__Integer))
class_end
static void Class_init(Class1* class){
}
static void Class_deinit(Class1* class){

}
void IntegerClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(IntegerClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "value",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(IntegerClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Integer_new,READABLE_UNWRITABLE_PUBLIC,
                              Integer_getValue,READABLE_UNWRITABLE_PUBLIC,
                              IntegerClazz_getSize,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}