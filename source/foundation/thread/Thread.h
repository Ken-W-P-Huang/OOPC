//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_THREAD_H
#define OOPC_THREAD_H
#include "../interface/Runnable.h"
#include "../Object.h"
#include <pthread.h>
PUBLIC(Thread,Object)
END

PUBLIC_STATIC(Thread,Object)
#if defined(__POLYMORPHISM__)
    Thread *(*init)(Thread *self,Object* runnable);
    bool (*start)(Thread* self);
    void* (*join)(Thread* self);
    void (*exit)(void* returnValue);
    void (*cancel)(Thread* self);
#endif
END
Thread *Thread_init(Thread *self,Object* runnable);
bool Thread_start(Thread* self);
void* Thread_join(Thread* self);
void Thread_exit(void* returnValue);
void Thread_cancel(Thread* self);

#endif //OOPC_THREAD_H
