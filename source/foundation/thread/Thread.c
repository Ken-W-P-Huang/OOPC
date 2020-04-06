//
// Created by kenhuang on 2019-10-23.
//

#include "Thread.h"

PRIVATE(Thread)
    pthread_t pthread;
    Object* runnable;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Thread *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Thread *Thread_init(Thread *self,Object* runnable) {
    if (Object_init((Object*)self)) {
        self->p->runnable = runnable;
    }
    return self;
}

bool Thread_start(Thread* self){
    Runnable* runnable = (Runnable*)getInterface(self->p->runnable,(ClassType*)RunnableClass);
    return pthread_create(&self->p->pthread,NULL,runnable->run,self->p->runnable) != 0;
}
void* Thread_join(Thread* self){
    void*p;
    pthread_join(self->p->pthread,&p);
    return  p;
}
void Thread_exit(void* returnValue){
    pthread_exit(returnValue);
}
void Thread_cancel(Thread* self){
    pthread_cancel(self->p->pthread);
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Thread)
    Class_fillDeclaredFields((ClassType*)ThreadClass,
                             "pthread",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "runnable",PointerClass,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType*)ThreadClass,
                              deinit,AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Thread_init,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Thread_start,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Thread_join,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Thread_exit,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Thread_cancel,AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END