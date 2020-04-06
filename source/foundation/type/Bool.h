//
// Created by kenhuang on 2019-10-21.
//

#ifndef OOPC_BOOL_H
#define OOPC_BOOL_H

#include "../Object.h"

PUBLIC(Bool,Object)
END

PUBLIC_STATIC(Bool,Object)
    Bool *const TRUE;
    Bool *const FALSE;
#if defined(__POLYMORPHISM__)
    Bool *(*const init)(Bool *self,bool value);
    bool (*const getValue)(Bool*self);
#endif
END
Bool *Bool_init(Bool *self,bool value);
bool Bool_getValue(Bool*self);
#endif //OOPC_BOOL_H
