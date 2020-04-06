//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_POINTER_H
#define OOPC_POINTER_H

#include "../Object.h"
PUBLIC(Pointer,Object)
END

PUBLIC_STATIC(Pointer,Object)
#if defined(__POLYMORPHISM__)
    Pointer *(*init)(Pointer *self,void* value);
    void* (*getValue)(Pointer* self);
#endif
END
Pointer *Pointer_init(Pointer *self,void* value);
void* Pointer_getValue(Pointer* self);
#endif //OOPC_POINTER_H
