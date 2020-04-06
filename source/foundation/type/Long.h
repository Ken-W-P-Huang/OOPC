//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_LONG_H
#define OOPC_LONG_H

#include "../Object.h"
PUBLIC(Long,Object)
END

PUBLIC_STATIC(Long,Object)
    const long MIN_VALUE;
    const long MAX_VALUE;
#if defined(__POLYMORPHISM__)
    Long *(*init)(Long *self, long value);
    long (*getValue)(Long* self);
#endif
END

Long *Long_init(Long *self, long value);
long Long_getValue(Long* self);
#endif //OOPC_LONG_H
