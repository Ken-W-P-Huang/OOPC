//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_LOCK_H
#define OOPC_LOCK_H

#include "../Object.h"
PUBLIC(Lock,Object)
END

PUBLIC_STATIC(Lock,Object)
#if defined(__POLYMORPHISM__)
    Lock *(*init)(Lock *self);
    Lock* (*initWith)(Lock* self, int lockType,bool isShared);
    bool (*lock)(Lock* self);
    bool (*trylock)(Lock* self);
    bool (*unlock)(Lock* self);
#endif
END
Lock *Lock_init(Lock *self);
Lock* Lock_initWith(Lock* self, int lockType,bool isShared);
bool Lock_lock(Lock* self);
bool Lock_trylock(Lock* self);
bool Lock_unlock(Lock* self);
#endif //OOPC_LOCK_H
