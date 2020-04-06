//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_SHORT_H
#define OOPC_SHORT_H

#include "../Object.h"
PUBLIC(Short,Object)
END

PUBLIC_STATIC(Short,Object)
    const short MAX_VALUE;
    const short MIN_VALUE;
#if defined(__POLYMORPHISM__)
    Short *(*init)(Short *self, short value);
    short (*getValue)(Short* self);
#endif
END

Short *Short_init(Short *self, short value);
short Short_getValue(Short* self);
#endif //OOPC_SHORT_H
