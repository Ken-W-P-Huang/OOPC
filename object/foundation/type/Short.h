//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Short_H
#define  Short_H

#include "../Object.h"
public_start(Short, Object)
/* 公有属性 */
    struct __Short* private;
public_end
/* 静态属性 */
extern const short SHORT_MIN_VALUE;
extern const short SHORT_MAX_VALUE;
/* 公有方法*/
Short* Short_new(Short *self, int value);
short Short_getValue(Short* self);

/* 静态方法 */
void ShortClazz_fillFieldsMethods();

#endif //Short_H