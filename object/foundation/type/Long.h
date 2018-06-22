//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Long_H
#define  Long_H
#include "../Object.h"
public_start(Long, Object)
/* 公有属性 */
    struct __Long* private;
public_end
/* 静态属性 */
extern const long LONG_MIN_VALUE;
extern const long LONG_MAX_VALUE;
/* 公有方法*/
Long* Long_new(Long *self, int value);
long Long_getValue(Long* self);

/* 静态方法 */
void LongClazz_fillFieldsMethods();

#endif //Long_H