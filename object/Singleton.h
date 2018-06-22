//
// Created by kenhuang on 2018/6/14.
//

#ifndef  SingleInstance_H
#define  SingleInstance_H

#include "foundation/Object.h"

public_start(Singleton, Object)
int a;
public_end
/* 公有方法*/
Singleton* Singleton_getInstance();

/* 静态方法 */


#endif //SingleInstance_H