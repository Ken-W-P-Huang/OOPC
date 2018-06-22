//
// Created by kenhuang on 2018/6/13.
//

#include <pthread.h>
#include "Thread.h"
#include "Lock.h"
#include "../type/Pointer.h"

private_start(Thread, Object)
/* 私有属性 */
    pthread_t pthread;
    Runnable* runnable;
private_end
/* 静态属性 */

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(ThreadClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(ThreadClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Thread* Thread_new(Thread* self,Runnable* runnable){
    if(runnable &&  runnable->run){
        if(!self){
            self = Class_newInstance(ThreadClazz);
        }
        self->private->runnable = runnable;
    }
    return  self;
}
bool Thread_start(Thread* self){
    return pthread_create(&self->private->pthread,NULL,(void *  (* )(void *))self->private->runnable->run,NULL) != -1;
}
void* Thread_join(Thread* self){
    void*p;
    pthread_join(self->private->pthread,&p);
    return  p;
}
void Thread_exit(void* returnValue){
    pthread_exit(returnValue);
}
void Thread_cancel(Thread* self){
    pthread_cancel(self->private->pthread);
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Thread, Object,offsetof(Thread,private), sizeof(__Thread))

class_end

static void Class_init(Class1 *class) {
    Class_registerLockClazz(isReflectable);
    Class_registerRunnableClazz();
    if (isReflectable) {
        Class_fillDeclaredFields(ThreadClazz,"_",ObjectClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "pthread",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "runnable",PointerClazz,READABLE_UNWRITABLE_PRIVATE,"");
        Class_fillDeclaredMethods(ThreadClazz,init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  Thread_new,READABLE_UNWRITABLE_PUBLIC,
                                  Thread_start,READABLE_UNWRITABLE_PUBLIC,
                                  Thread_join,READABLE_UNWRITABLE_PUBLIC,
                                  Thread_exit,READABLE_UNWRITABLE_PUBLIC,
                                  Thread_cancel,READABLE_UNWRITABLE_PUBLIC, NULL);
    }
}

static void Class_deinit(Class1 *class) {
    Class_unregisterLockClazz(isReflectable);
    Class_unregisterRunnableClazz();
}
