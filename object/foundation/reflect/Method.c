//
// Created by kenhuang on 2018/6/6.
//

#include "Method.h"
#include "../type/Pointer.h"
#include "../type/Character.h"
/***********************************************************************************************************************
 *  Selector类
 **********************************************************************************************************************/
/* 类型声明 */
private_start(Method,Object)
    const Class1* clazz;  /*属于哪个类*/
    const String * name;
    Function function;
    Access* access;
//    Class** parameterTypes;
    Class1* returnType;
private_end
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object *init(Object * self, va_list *parms) {
    return Class_getSuperClass(MethodClazz)->init(self,parms);
}

static Object *deinit(Object *self) {
    return Class_getSuperClass(MethodClazz)->deinit(self);
}

/***********************************************************************************************************************
 *  公共方法
 **********************************************************************************************************************/
const String * Method_getName(Method *self){
    return self->private->name;
}
Function Method_getMethod(Method *self){
    return  self->private->function;
}


Method *Method_new(Method *self, const Class1 *clazz, String *name, Function method) {
    if(!self){
        self = (Method *) Class_newInstance(MethodClazz);
    }
    self->private->clazz = clazz;
    self->private->function = method;
    self->private->name = name;
    return self;
}
Access* Method_getAccess(Method * self){
    return self->private->access;
}
/**
 * 不能传递参数
 * @param self
 * @param object
 * @param ...
 * @return
 */
Object* Method_invoke(Method* self,Object* object,...){
    long result = self->private->function(object);
    return Object_cast((void*)result,self->private->returnType);
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(Method,Object,offsetof(Method,private), sizeof(__Method))
class_end
static void Class_init(Class1* class) {

}

static void Class_deinit(Class1* class) {

}

void MethodClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(MethodClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "clazz",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "name",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "function",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "access",CharacterClazz,READABLE_UNWRITABLE_PRIVATE,
                             "returnType",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(MethodClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Method_getName,READABLE_UNWRITABLE_PUBLIC,
                              Method_getMethod,READABLE_UNWRITABLE_PUBLIC,
                              Method_new,READABLE_UNWRITABLE_PUBLIC,
                              Method_getAccess,READABLE_UNWRITABLE_PUBLIC,
                              Method_invoke,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}
