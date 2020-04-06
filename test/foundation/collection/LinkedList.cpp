#include "gtest/gtest.h"

extern "C"{
    extern  struct ClassType * const ObjectClass;
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const IntegerClass;
    extern ClassType *const LinkedListClass;
    extern struct Integer *Integer_init(Integer *self, int value);
    extern int Integer_getValue(Integer* self);
    struct LinkedList *LinkedList_init(LinkedList *self);
    struct LinkedListIterator* LinkedList_getIterator(LinkedList* self);
    void LinkedList_addFirst(LinkedList* self, struct Object* element);
    void LinkedList_addLast(LinkedList* self,Object* element);
    bool LinkedList_remove(LinkedList* self,Object* element);
    Object* LinkedList_removeFirst(LinkedList* self);
    Object* LinkedList_removeLast(LinkedList* self);
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
    extern ClassType *const LinkedListIteratorClass;
    bool LinkedListIterator_hasNext(struct LinkedListIterator *self);
    Object *LinkedListIterator_next(struct LinkedListIterator *self);
    struct Bool *Bool_init(Bool *self,bool value);
    extern struct BoolClassType *const BoolClass;
    bool Bool_getValue(Bool*self);
    void destroy(Object *self);
}

TEST(TEST, LinkedList) {
    struct Bool* TRUE = Bool_init((Bool *)alloc((ClassType*)BoolClass),true);
    struct Bool* FALSE = Bool_init((Bool *)alloc((ClassType*)BoolClass), false);
    LinkedList* list = LinkedList_init((LinkedList*)alloc(LinkedListClass));
    int COUNT = 10;
    for (int i = 0; i < COUNT; ++i) {
        LinkedList_add(list,(Object*)TRUE);
    }
    ASSERT_TRUE(LinkedList_getSize(list) == COUNT);
    for (int i = 0; i < COUNT; ++i) {
        Bool* b = (Bool*)LinkedList_get(list,i);
        ASSERT_TRUE(Bool_getValue(b) == true);
    }
    LinkedList_clear(list);
    ASSERT_TRUE(LinkedList_getSize(list) == 0);
    destroy((Object*)list);
}

TEST(LinkedList_get, LinkedList) {
    struct Bool* TRUE = Bool_init((Bool *)alloc((ClassType*)BoolClass),true);
    struct Bool* FALSE = Bool_init((Bool *)alloc((ClassType*)BoolClass), false);
    LinkedList* list = LinkedList_init((LinkedList*)alloc(LinkedListClass));
    int COUNT = 10;
    for (int i = 0; i < COUNT; ++i) {
        LinkedList_add(list,(Object*)TRUE);
    }
    ASSERT_TRUE(LinkedList_getSize(list) == COUNT);
    LinkedList_addFirst(list,(Object*)FALSE);
    ASSERT_TRUE(Bool_getValue((Bool*)LinkedList_get(list,0)) == false);
    ASSERT_TRUE(LinkedList_getSize(list) == COUNT + 1);
    LinkedList_addLast(list,(Object*)TRUE);
    ASSERT_TRUE(Bool_getValue((Bool*)LinkedList_getLast(list)) == true);
    ASSERT_TRUE(LinkedList_indexOf(list,(Object*)TRUE) == 1);
    ASSERT_TRUE(LinkedList_indexOf(list,(Object*)FALSE) == 0);
    ASSERT_TRUE(LinkedList_getSize(list) == COUNT + 2);
    Bool* b = (Bool*)LinkedList_removeFirst(list);
    ASSERT_TRUE(Bool_getValue(b) == false);
    b = (Bool*)LinkedList_removeLast(list);
    ASSERT_TRUE(Bool_getValue(b) == true);
    destroy((Object*)list);
}

TEST(LinkedList_pop, LinkedList) {
    struct Bool* TRUE = Bool_init((Bool *)alloc((ClassType*)BoolClass),true);
    struct Bool* FALSE = Bool_init((Bool *)alloc((ClassType*)BoolClass), false);
    LinkedList* list = LinkedList_init((LinkedList*)alloc(LinkedListClass));
    int COUNT = 10;
    for (int i = 0; i < COUNT; ++i) {
        LinkedList_push(list,(Object*)TRUE);
    }
    ASSERT_TRUE(LinkedList_getSize(list) == COUNT);
    for (int i = 0; i < COUNT; ++i) {
        Bool* b = (Bool*)LinkedList_pop(list);
        ASSERT_TRUE(Bool_getValue(b) == true);
    }
    ASSERT_TRUE(LinkedList_getSize(list) == 0);
    destroy((Object*)list);
}

TEST(LinkList_set, LinkedList) {
    struct Bool* TRUE = Bool_init((Bool *)alloc((ClassType*)BoolClass),true);
    struct Bool* FALSE = Bool_init((Bool *)alloc((ClassType*)BoolClass), false);
    LinkedList* list = LinkedList_init((LinkedList*)alloc(LinkedListClass));
    int COUNT = 100;
    for (int i = 0; i < COUNT; ++i) {
        LinkedList_push(list,(Object*)TRUE);
    }
    ASSERT_TRUE(LinkedList_getSize(list) == COUNT);
    for (int i = 0; i < COUNT; ++i) {
       Bool* b =  ( Bool* )LinkList_set(list,i,(Object*)FALSE);
       ASSERT_TRUE(Bool_getValue(b) == true);
    }
    for (int i = 0; i < COUNT; ++i) {
        ASSERT_TRUE(Bool_getValue((Bool*)LinkedList_getLast(list)) == false);
    }
    destroy((Object*)list);
}

TEST(LinkedList_getIterator, LinkedList) {
    struct Bool* TRUE = Bool_init((Bool *)alloc((ClassType*)BoolClass),true);
    struct Bool* FALSE = Bool_init((Bool *)alloc((ClassType*)BoolClass), false);
    LinkedList* list = LinkedList_init((LinkedList*)alloc(LinkedListClass));
    int COUNT = 10;
    for (int i = 0; i < COUNT; ++i) {
        if (i % 2 == 0) {
            LinkedList_push(list,(Object*)TRUE);
        }else{
            LinkedList_push(list,(Object*)FALSE);
        }
    }
    LinkedListIterator*  iterator = LinkedList_getIterator(list);
    int i = 0;
    while (LinkedListIterator_hasNext(iterator)) {
        if (i % 2 == 0) {
            ASSERT_TRUE((Bool*)LinkedListIterator_next(iterator) == FALSE);
        }else{
            ASSERT_TRUE((Bool*)LinkedListIterator_next(iterator) == TRUE);
        }
        i++;
    }
    destroy((Object*)list);
}
