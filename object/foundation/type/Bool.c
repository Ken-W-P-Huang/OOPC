//
// Created by kenhuang on 2018/6/13.
//

#include "Bool.h"
#include "Character.h"
#include "Pointer.h"
#include "Integer.h"

private_start(Bool, Object)
/* 私有属性 */
bool value;
private_end
/* 静态属性 */
const Bool* TRUE ;
const Bool* FALSE ;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(BoolClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(BoolClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Bool* Bool_new(Bool*self,bool value){
    if(!self){
        self = Class_newInstance(BoolClazz);
    }
    self->private->value = value;
    return self;
}

/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Bool, Object,offsetof(Bool,private), sizeof(__Bool))

class_end
const Bool* Bool_valueOf(bool value){
    return value? TRUE:FALSE;
}
static void Class_init(Class1* class) {
    TRUE = Bool_new(NULL,true);
    FALSE = Bool_new(NULL,false);
}

static void Class_deinit(Class1* class) {
    Object_destroy((Object*)TRUE);
    Object_destroy((Object*)FALSE);
}
void BoolClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(BoolClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "value",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(BoolClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Bool_new,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}
