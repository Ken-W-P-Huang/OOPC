//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Pointer_H
#define  Pointer_H

#include "../Object.h"

public_start(Pointer, Object)
/* 公有属性 */
    struct __Pointer* private;
public_end
/* 公有方法*/


/* 静态方法 */
void PointerClazz_fillFieldsMethods();

#endif //Pointer_H