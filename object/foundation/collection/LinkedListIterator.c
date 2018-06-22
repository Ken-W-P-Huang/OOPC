//
// Created by kenhuang on 18-6-9.
//

#include "LinkedList.h"

private_start(LinkedListIterator,Object)
/* 私有属性 */
LinkedListNode* currentNode;
private_end
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/

static Object* init(Object* self,va_list* parms){
    return Class_getSuperClass(LinkedListIteratorClazz)->init(self,parms);
}
static Object* deinit(Object* self){
    return Class_getSuperClass(LinkedListIteratorClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
LinkedListIterator* LinkedListIterator_new(LinkedListIterator *self, LinkedListNode *node){
    if(!self){
        self = Class_newInstance(LinkedListIteratorClazz);
    }
    self->private->currentNode = node;
    return self;
}
bool LinkedListIterator_hasNext(LinkedListIterator *self){
    return self->private->currentNode != NULL;
}

Object* LinkedListIterator_next(LinkedListIterator* self){
    Object* element = self->private->currentNode->element;
    self->private->currentNode = self->private->currentNode->next;
    return element;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(LinkedListIterator,Object,offsetof(LinkedListIterator,private), sizeof(__LinkedListIterator))
class_end
static void Class_init(Class1* class){
    if (isReflectable) {
        //Reflect
    }
}
static void Class_deinit(Class1* class){

}

