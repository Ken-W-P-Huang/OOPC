//
// Created by kenhuang on 2019-10-21.
//

#ifndef OOPC_LINKEDLIST_H
#define OOPC_LINKEDLIST_H

#include "../Object.h"
PUBLIC(LinkedList,Object)
END

PUBLIC_STATIC(LinkedList,Object)
#if defined(__POLYMORPHISM__)
    LinkedList *(*init)(LinkedList *self);
    struct LinkedListIterator* (*getIterator)(LinkedList* self);
    void (*addFirst)(LinkedList* self, Object* element);
    void (*addLast)(LinkedList* self,Object* element);
    bool (*remove)(LinkedList* self,Object* element);
    void (*add)(LinkedList *self, Object *element);
    int (*indexOf)(LinkedList *self, Object *element);
    Object* (*get)(LinkedList* self, int index);
    Object* (*getLast)(LinkedList* self);
    Object* (*getFirst)(LinkedList* self);
    void (*clear)(LinkedList* self);
    Object* (*set)(LinkedList* self, int index,Object* element);
    int (*getSize)(LinkedList* self);
    Object* (*pop)(LinkedList* self);
    void (*push)(LinkedList* self,Object *element);
#endif
END
LinkedList *LinkedList_init(LinkedList *self);
struct LinkedListIterator* LinkedList_getIterator(LinkedList* self);
void LinkedList_addFirst(LinkedList* self, Object* element);
void LinkedList_addLast(LinkedList* self,Object* element);
bool LinkedList_remove(LinkedList* self,Object* element);
Object* LinkedList_removeFirst(LinkedList* self);
Object* LinkedList_removeLast(LinkedList* self);
//Object* LinkedList_remove(LinkedList* self);
void LinkedList_add(LinkedList *self, Object *element);
int LinkedList_indexOf(LinkedList *self, Object *element);
Object* LinkedList_get(LinkedList* self, int index);
Object* LinkedList_getLast(LinkedList* self);
Object* LinkedList_getFirst(LinkedList* self);
void LinkedList_clear(LinkedList* self);
Object* LinkList_set(LinkedList* self, int index,Object* element);
int LinkedList_getSize(LinkedList* self);
Object* LinkedList_pop(LinkedList* self);
void LinkedList_push(LinkedList* self,Object *element);
Object* LinkedList_removeWithIndex(LinkedList* self,int index);

extern struct LinkedListIteratorClassType *const LinkedListIteratorClass;
bool LinkedListIterator_hasNext(struct LinkedListIterator *self);
Object *LinkedListIterator_next(struct LinkedListIterator *self);
#endif //OOPC_LINKEDLIST_H
