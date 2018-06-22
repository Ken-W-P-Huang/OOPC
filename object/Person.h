/////        //
// Created by kenhuang on 2018/6/3.
//

#ifndef  Person_H
#define  Person_H

#include "Circle.h"
#include "foundation/interface/Runnable.h"

public_start(Person,Circle)
    int age;
    /* 私有属性*/ \
    struct __Person *private; \
public_end
Runnable* Person_conformToRunable(Person* self);
#endif //

