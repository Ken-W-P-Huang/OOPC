//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_DOUBLE_H
#define OOPC_DOUBLE_H

#include "../Object.h"
PUBLIC(Double,Object)

END

PUBLIC_STATIC(Double,Object)
    const double MIN_VALUE;
    const double MAX_VALUE;
#if defined(__POLYMORPHISM__)
    Double *(*init)(Double *self, double value);
    double (*getValue)(Double *self);
#endif
END
Double *Double_init(Double *self, double value);
double Double_getValue(Double *self);
#endif //OOPC_DOUBLE_H
