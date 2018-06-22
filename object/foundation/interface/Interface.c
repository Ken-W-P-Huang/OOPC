//
// Created by kenhuang on 2018/6/13.
//

#include "Interface.h"
#include "../type/Pointer.h"

/* 私有属性 */

/* 静态属性 */

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(InterfaceClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(InterfaceClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Interface,Object,0,0)

class_end
static void Class_init(Class1 *class) {
    if (isReflectable) {
        Class_fillDeclaredFields(InterfaceClazz,
                                 "object",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "");
        Class_fillDeclaredMethods(InterfaceClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  NULL);
    }
}

static void Class_deinit(Class1 *class) {

}
