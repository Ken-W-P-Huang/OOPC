//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Bool_H
#define  Bool_H
#include "../Object.h"
public_start(Bool, Object)
/* 公有属性 */
    struct __Bool* private;
public_end
/* 公有方法*/
Bool* Bool_new(Bool*self,bool value);

/* 静态方法 */
const Bool* Bool_valueOf(bool value);
void BoolClazz_fillFieldsMethods();
#endif //Bool_H