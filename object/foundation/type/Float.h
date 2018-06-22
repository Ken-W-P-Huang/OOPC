//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Float_H
#define  Float_H

#include "../Object.h"
public_start(Float, Object)
/* 公有属性 */
    struct __Float* private;
public_end
/* 静态属性 */
extern const float FLOAT_MIN_VALUE;
extern const float FLOAT_MAX_VALUE;
/* 公有方法*/
Float* Float_new(Float*self,char character);
float Float_getValue(Float*self);

/* 静态方法 */
void FloatClazz_fillFieldsMethods();

#endif //Float_H