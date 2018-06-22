//
// Created by kenhuang on 2018/6/13.
//

#include "Runnable.h"
#include "../type/Pointer.h"



/* 私有属性 */


/* 静态属性 */

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(RunnableClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(RunnableClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Runnable, Interface,0,0)

class_end
static void Class_init(Class1 *class) {
    if (isReflectable) {
        Class_fillDeclaredFields(RunnableClazz,
                                 "run",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "");
        Class_fillDeclaredMethods(RunnableClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  NULL);
    }
}

static void Class_deinit(Class1 *class) {

}
