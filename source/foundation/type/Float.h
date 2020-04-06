//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_FLOAT_H
#define OOPC_FLOAT_H

#include "../Object.h"
PUBLIC(Float,Object)
END

PUBLIC_STATIC(Float,Object)
    const float MIN_VALUE;
    const float MAX_VALUE;
#if defined(__POLYMORPHISM__)
    Float *(*init)(Float *self, float value);
    float (*getValue)(Float*self);
#endif
END

Float *Float_init(Float *self, float value);
float Float_getValue(Float*self);
#endif //OOPC_FLOAT_H
