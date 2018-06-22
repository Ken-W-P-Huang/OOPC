//
// Created by kenhuang on 2018/6/7.
//

#include "LinkedList.h"

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object *init(Object *self, va_list *parms) {
    return Class_getSuperClass(LinkedListNodeClazz)->init(self,parms);
}

static Object *deinit(Object *self) {
    return Class_getSuperClass(LinkedListNodeClazz)->deinit(self);
}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
LinkedListNode* LinkedListNode_new(LinkedListNode *self, LinkedListNode *prev, Object *element, LinkedListNode *next){
    if(!self){
        self = (LinkedListNode *) Class_newInstance(LinkedListNodeClazz);
    }
    self->prev = prev;
    self->element = element;
    self->next= next;
    return  self;
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(LinkedListNode,Object,0,0)
class_end
static void Class_init(Class1* class) {
    if (isReflectable) {
        //Reflect
    }
}

static void Class_deinit(Class1* class) {

}



