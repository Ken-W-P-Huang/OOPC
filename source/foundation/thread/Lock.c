//
// Created by kenhuang on 2019-10-23.
//

#include "Lock.h"
#include <pthread.h>
PRIVATE(Lock)
    pthread_mutex_t* mutex;
    pthread_mutexattr_t* attr;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Lock *self) {
    if (self) {
        free(self->p->mutex);
        free(self->p->attr);
    }
}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Lock* Lock_init(Lock* self){
    return Lock_initWith(self,PTHREAD_MUTEX_DEFAULT,false);
}
Lock* Lock_initWith(Lock* self, int lockType,bool isShared){
    int ret;
    if(Object_init((Object*) self)){
        self->p->mutex = malloc(sizeof(pthread_mutex_t));
        self->p->attr = malloc(sizeof(pthread_mutexattr_t));
        ret = pthread_mutexattr_init(self->p->attr);
        if(ret != 0){
            fprintf(stderr, "create mutex attribute error. msg:%s", strerror(ret));
            exit(1);
        }
        if(isShared){
            pthread_mutexattr_setpshared(self->p->attr,PTHREAD_PROCESS_SHARED);
        }
        pthread_mutexattr_settype(self->p->attr, lockType);
        ret = pthread_mutex_init(self->p->mutex,self->p->attr);
        if(ret!= 0){
            fprintf(stderr, "create mutexerror. msg:%s", strerror(ret));
            exit(1);
        }
    }
    return  self;
}
bool Lock_lock(Lock* self){
    return  pthread_mutex_lock(self->p->mutex) == 0;
}
bool Lock_trylock(Lock* self){
    return pthread_mutex_trylock(self->p->mutex) == 0;
}
bool Lock_unlock(Lock* self){
    return pthread_mutex_unlock(self->p->mutex) == 0;
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Lock)
    Class_fillDeclaredFields((ClassType*)LockClass,
                             "mutex",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "attr",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType*)LockClass,
                              deinit,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Lock_init,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Lock_initWith,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Lock_lock,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Lock_trylock,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Lock_unlock,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END