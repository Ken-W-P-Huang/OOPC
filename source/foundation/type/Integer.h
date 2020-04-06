//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_INTEGER_H
#define OOPC_INTEGER_H

#include "../Object.h"
PUBLIC(Integer,Object)
END

PUBLIC_STATIC(Integer,Object)
    const int MIN_VALUE;
    const int MAX_VALUE;
#if defined(__POLYMORPHISM__)
    Integer *(*init)(Integer *self, int value);
    int (*getValue)(Integer* self);
#endif
END



Integer *Integer_init(Integer *self, int value);
int Integer_getValue(Integer* self);
#endif //OOPC_INTEGER_H
