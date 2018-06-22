//
// Created by kenhuang on 18-6-13.
//

#ifndef  Lock_H
#define  Lock_H
#include "../Object.h"
public_start(Lock, Object)
/* 公有属性 */
    struct __Lock* private;
public_end
/* 公有方法*/
Lock* Lock_new(Lock* self);
Lock* Lock_newWith(Lock* self, int lockType,bool isShared);
bool Lock_lock(Lock* self);
bool Lock_trylock(Lock* self);
bool Lock_unlock(Lock* self);
/* 静态方法 */


#endif //Lock_H