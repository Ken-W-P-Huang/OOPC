//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Thread_H
#define  Thread_H
#include "../Object.h"
#include "../interface/Runnable.h"

public_start(Thread, Object)
/* 公有属性 */
    struct __Thread* private;
public_end
/* 公有方法*/
Thread* Thread_new(Thread* self,Runnable* runnable);
bool Thread_start(Thread* self);
void* Thread_join(Thread* self);
void Thread_exit(void* returnValue);
void Thread_cancel(Thread* self);
/* 静态方法 */


#endif //Thread_H