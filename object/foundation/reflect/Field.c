//
// Created by kenhuang on 2018/6/6.
//

#include "Field.h"
#include "../type/Pointer.h"
#include "../type/Integer.h"
#include "../type/Character.h"

private_start(Field,Object)
    const Class1* clazz; /*属于哪个类*/
    String* name;
    const Class1* type; /*数据类型，如果是基本数据类型，则必须先包装成对象*/
    size_t length;
    size_t offset;
    Access* access;
private_end

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object *init(Object *self, va_list *parms) {
    return Class_getSuperClass(FieldClazz)->init(self,parms);
}

static Object *deinit(Object *self) {
    return Class_getSuperClass(FieldClazz)->deinit(self);
}

/***********************************************************************************************************************
 *  公共方法
 **********************************************************************************************************************/
String * Field_getName(Field *self){
    return  self->private->name;
}
const Class1 * Field_getType(Field *self){
    return self->private->type;
}

size_t Field_getOffset(Field *self){
    return self->private->offset;
}

size_t Field_getLength(Field *self){
    return self->private->length;
}
Access* Field_getAccess(Field *self){
    return self->private->access;
}
Field *
Field_new(Field *self, const Class1 *clazz, String *name, Class1 *type, size_t offset, size_t length, Access* access) {
    if(!self){
       self = (Field*) Class_newInstance(FieldClazz);
    }
    __Field* private = self->private;
    private->clazz = clazz;
    private->name = name;
    private->type = type;
    private->length = length;
    private->offset = offset;
    private->access = access;
    return self;
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(Field,Object,offsetof(Field,private), sizeof(__Field))
class_end
static void Class_init(Class1* class) {
    Class_registerAccessClazz();
    Class_registerMethodClazz();
}

static void Class_deinit(Class1* class) {
    Class_unregisterAccessClazz();
    Class_unregisterMethodClazz();
}
void FieldClazz_fillFieldsMethods(){
    MethodClazz_fillFieldsMethods();
    AccessClazz_fillFieldsMethods();
    Class_fillDeclaredFields(FieldClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "clazz",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "name",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "type",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "length",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "offset",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "access",CharacterClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(FieldClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Field_getName,READABLE_UNWRITABLE_PUBLIC,
                              Field_getType,READABLE_UNWRITABLE_PUBLIC,
                              Field_getOffset,READABLE_UNWRITABLE_PUBLIC,
                              Field_getLength,READABLE_UNWRITABLE_PUBLIC,
                              Field_getAccess,READABLE_UNWRITABLE_PUBLIC,
                              Field_new,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}


