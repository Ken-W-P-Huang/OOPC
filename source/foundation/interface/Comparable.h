//
// Created by kenhuang on 2019-12-03.
//
#include "../Object.h"
#ifndef OS_COMPARABLE_H
#define OS_COMPARABLE_H




INTERFACE(Comparable,Interface)
    int (*compareTo)(Object* o);
END

PUBLIC_STATIC(Comparable, Interface)
#if defined(__POLYMORPHISM__)

#endif
END

Comparable *Comparable_init(Comparable *self,int (*compareTo)(Object* o));

#endif //OS_COMPARABLE_H
