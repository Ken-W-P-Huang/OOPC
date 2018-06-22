//
// Created by kenhuang on 2018/6/14.
//

#include <pthread.h>
#include "Singleton.h"
static Singleton* singleton = NULL;
/* 静态属性 */
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(SingletonClazz)->init(self, parms);
}
static void new(){
    _DEBUG("newing");
    if(!singleton){
        singleton = Class_newInstance(SingletonClazz);
    }
}
static void *deinit(void *self) {
    return Class_getSuperClass(SingletonClazz)->deinit(self);
}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/

Singleton* Singleton_getInstance(){
    static pthread_once_t once = PTHREAD_ONCE_INIT;
    int code = pthread_once(&once,new);
    if(code){
        fprintf(stderr, "pthread_once error. [err=%s]",
                strerror(code));
        abort();
    }
   // new();
    return singleton;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Singleton, Object,0,0)

class_end
static void Class_init(Class1 *class) {
    if (isReflectable) {
        //Reflect
    }
}

static void Class_deinit(Class1 *class) {

}
