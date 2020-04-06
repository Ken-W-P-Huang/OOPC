//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_IRUNNABLE_H
#define OOPC_IRUNNABLE_H

#include "../Object.h"

INTERFACE(Runnable,Interface)
    void* (*run)(void*);
END

PUBLIC_STATIC(Runnable,Interface)

END
Runnable *Runnable_init(Runnable *self,void* (*run)(void*));
#endif //OOPC_IRUNNABLE_H
