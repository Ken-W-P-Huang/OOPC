//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Double_H
#define  Double_H

#include "../Object.h"
public_start(Double, Object)
/* 公有属性 */
    struct __Double* private;
public_end
/* 静态属性 */
extern const double DOUBLE_MIN_VALUE;
extern const double DOUBLE_MAX_VALUE;
/* 公有方法*/
Double* Double_new(Double*self,char character);
double Double_getValue(Double *self);

/* 静态方法 */
void DoubleClazz_fillFieldsMethods();

#endif //Double_H