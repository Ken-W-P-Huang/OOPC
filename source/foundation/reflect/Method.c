//
// Created by kenhuang on 2019-10-21.
//

#include "Method.h"
#if (defined(__OVERRIDE__) || defined(__POLYMORPHISM__)) && defined(__REFLECT__)
PRIVATE(Method)
    /*属于哪个类*/
    const ClassType* clazz;
    const String * name;
    Function function;
    Access* access;
//    ClassType** parameterTypes;
    ClassType* returnType;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Method *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Method *Method_init(Method *self,const ClassType *clazz, String *name, Function method) {
    if (Object_init((Object*)self)) {
        self->p->clazz = clazz;
        self->p->function = method;
        self->p->name = name;
    }
    return self;
}
const String * Method_getName(Method *self){
    return self->p->name;
}
Function Method_getMethod(Method *self){
    return  self->p->function;
}
Access* Method_getAccess(Method * self){
    return self->p->access;
}
/**
 * 不能传递参数
 * @param self
 * @param object
 * @param ...
 * @return
 */
//todo 如何将可变参数直接传给function执行。
//https://stackoverflow.com/questions/58441770/how-to-pass-variables-to-a-external-assembly-function
Object* Method_invoke(Method* self,Object* object,...){
    long result = self->p->function(object);
    Object* o = cast((void*)result,self->p->returnType);
//    if (o == NULL) {
//        ClassType* clazz = self->p->returnType;
//        o = alloc(clazz);
//        if(clazz == BoolClass){
//            Bool_init(o,(bool)self);
//        }else if(clazz == IntegerClass){
//            Integer_init(o,(int)self);
//        }else if(clazz == ByteClass){
//            Byte_init(o,(byte)self);
//        }else if(clazz == CharacterClass){
//            Character_init(o,(char)self);
//        }else if(clazz == DoubleClass){
//            Double_init(o,(double)(long)self);
//        }else if (clazz == FloatClass){
//            Float_init(o,(float)(long)self);
//        }else if(clazz == LongClass){
//            Long_init(o,(long)self);
//        }else if(clazz == PointerClass){
//            Pointer_init(o,(void*)self);
//        }else if(clazz == ShortClass) {
//            Short_init(o, (short) self);
//        } else {
//
//            o = NULL;
//        }
//    }
    return o;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Method)
    Class_fillDeclaredFields((ClassType*)MethodClass,
                             "clazz",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "name",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "function",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "access",CharacterClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "returnType",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType*)MethodClass,
                              deinit,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Method_init,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Method_getName,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Method_getMethod,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Method_getAccess,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Method_invoke,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END
#endif