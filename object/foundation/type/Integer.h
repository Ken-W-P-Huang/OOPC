//
// Created by kenhuang on 2018/6/7.
//

#ifndef  Integer_H
#define  Integer_H
#include "../Object.h"
public_start(Integer,Object)
    struct __Integer* private;
public_end
/* 静态属性 */
extern const int INTEGER_MIN_VALUE;
extern const int INTEGER_MAX_VALUE;
/* 公开方法*/
int Integer_getValue(Integer* self);
Integer* Integer_new(Integer *self, int value);
/* 静态方法 */
void IntegerClazz_fillFieldsMethods();

#endif //Integer_H