//
// Created by kenhuang on 2019-11-12.
//
#include "LinkedList.h"
PRIVATE(LinkedListIterator)
END
PUBLIC(LinkedListIterator,Object)
struct LinkedListNode* currentNode;
END

PUBLIC_STATIC(LinkedListIterator,Object)
END

typedef struct LinkedListNode{
    struct LinkedListNode * next;
    struct LinkedListNode * prev;
    Object * element;
} LinkedListNode;

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(LinkedListIterator *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
LinkedListIterator * LinkedListIterator_init(LinkedListIterator *self, struct LinkedListNode *node) {
    if (Object_init((Object*)self)) {
        self->currentNode = node;
    }
    return self;
}

bool LinkedListIterator_hasNext(struct LinkedListIterator *self){
    return self->currentNode != NULL;
}

Object *LinkedListIterator_next(struct LinkedListIterator *self){
    Object* element = self->currentNode->element;
    self->currentNode = self->currentNode->next;
    return element;
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(LinkedListIterator)
    Class_fillDeclaredFields((ClassType *) LinkedListIteratorClass,
                             "currentNode", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) LinkedListIteratorClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              LinkedListIterator_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedListIterator_hasNext, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedListIterator_next, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END
