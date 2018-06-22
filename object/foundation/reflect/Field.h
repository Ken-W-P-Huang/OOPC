//
// Created by kenhuang on 2018/6/6.
//

#include "../Object.h"
#include "../type/String.h"
#include "Access.h"
#ifndef  Field_H
#define  Field_H

/***********************************************************************************************************************
 *  Field类
 **********************************************************************************************************************/
/* 类型声明 */
public_start(Field,Object)
    struct __Field* private;
public_end
/* 公共方法*/
Field * Field_new(Field *self, const Class1 *clazz, String *name, Class1 *type, size_t offset, size_t length,
                  Access* access);
String* Field_getName(Field *self);
const Class1 * Field_getType(Field *self);
Access* Field_getAccess(Field *self);
size_t Field_getLength(Field *self);
size_t Field_getOffset(Field *self);
/* 静态方法 */
void FieldClazz_fillFieldsMethods();
#endif //Field_H