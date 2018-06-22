//
// Created by kenhuang on 2018/6/10.
//

#include "ArrayList.h"

private_start(ArrayListIterator,Object)
/* 私有属性 */
    unsigned  int cursor;
    ArrayList* list;
private_end
/* 静态属性 */

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object *init(Object *self, va_list *parms){
    return Class_getSuperClass(ArrayListIteratorClazz)->init(self,parms);
}

static Object *deinit(Object * self) {
    return Class_getSuperClass(ArrayListIteratorClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
ArrayListIterator* ArrayListIterator_new(ArrayListIterator *self, ArrayList *list){
    if(!self){
        self = Class_newInstance(ArrayListIteratorClazz);
    }
    self->private->list = list;
    self->private->cursor = 0;
    return  self;
}
bool ArrayListIterator_hasNext(ArrayListIterator* self) {
    return self->private->cursor < ArrayList_size(self->private->list);
}

Object* ArrayListIterator_next(ArrayListIterator* self) {
    return ArrayList_get(self->private->list, self->private->cursor++);
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(ArrayListIterator,Object,offsetof(ArrayListIterator,private), sizeof(__ArrayListIterator))
class_end
static void Class_init(Class1* class) {
    if (isReflectable) {
        //Reflect
    }
}
static void Class_deinit(Class1* class) {

}
