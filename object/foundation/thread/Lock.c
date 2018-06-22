//
// Created by kenhuang on 18-6-13.
//

#include <pthread.h>
#include "Lock.h"
private_start(Lock, Object)
/* 私有属性 */
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
private_end
/* 静态属性 */

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(LockClazz)->init(self, parms);
}

static void *deinit(void *self) {
    Lock* mutex = (Lock*)self;
    pthread_mutex_destroy(&mutex->private->mutex);
    return Class_getSuperClass(LockClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Lock* Lock_new(Lock* self){
    return Lock_newWith(self,PTHREAD_MUTEX_DEFAULT,false);
}
Lock* Lock_newWith(Lock* self, int lockType,bool isShared){
    int ret;
    if(!self){
        self = Class_newInstance(LockClazz);
    }
    ret = pthread_mutexattr_init(&self->private->attr);
    if(ret != 0){
        fprintf(stderr, "create mutex attribute error. msg:%s", strerror(ret));
        exit(1);
    }
    if(isShared){
        pthread_mutexattr_setpshared(&self->private->attr,PTHREAD_PROCESS_SHARED);
    }
    pthread_mutexattr_settype(&self->private->attr, lockType);
    ret = pthread_mutex_init(&self->private->mutex,&self->private->attr);
    if(ret!= 0){
        fprintf(stderr, "create mutexerror. msg:%s", strerror(ret));
        exit(1);
    }
    return  self;
}
bool Lock_lock(Lock* self){
    return  pthread_mutex_lock(&self->private->mutex) == 0;
}
bool Lock_trylock(Lock* self){
    return pthread_mutex_trylock(&self->private->mutex) == 0;
}
bool Lock_unlock(Lock* self){
    return pthread_mutex_unlock(&self->private->mutex) == 0;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(Lock, Object,offsetof(Lock,private), sizeof(__Lock))

class_end
static void Class_init(Class1 *class) {

}

static void Class_deinit(Class1 *class) {

}
