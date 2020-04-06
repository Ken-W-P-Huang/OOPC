#include "gtest/gtest.h"

extern "C"{
    extern  struct ClassType * const ObjectClass;
    extern struct ClassType * const Class;
    void * alloc (const ClassType * clazz);
    extern ClassType *const ArrayListClass;
    extern ClassType *const ArrayListIteratorClass;
    extern struct BoolClassType *const BoolClass;
    struct ArrayList *ArrayList_init(ArrayList *self,unsigned int capacity);
    void ArrayList_push(ArrayList* self, struct Object* element);
    Object* ArrayList_pop(ArrayList* self) ;
    unsigned int ArrayList_size(ArrayList* self) ;
    void ArrayList_removeAll(ArrayList* self);
    struct ArrayListIterator* ArrayList_getIterator(ArrayList *self) ;
    Object* ArrayList_get(ArrayList* arrayList, size_t index) ;
    ArrayListIterator *ArrayListIterator_init(ArrayListIterator *self, ArrayList *list);
    Object* ArrayListIterator_next(ArrayListIterator* self);
    bool ArrayListIterator_hasNext(ArrayListIterator* self);
    struct Bool *Bool_init(Bool *self,bool value);
}

TEST(ArrayList_init, ArrayList) {
     ArrayList* list = ArrayList_init((ArrayList*)alloc(ArrayListClass),10);
    struct Bool* TRUE = Bool_init((Bool *)alloc((ClassType*)BoolClass),true);
    struct Bool* FALSE = Bool_init((Bool *)alloc((ClassType*)BoolClass), false);
     ASSERT_EQ(ArrayList_size(list),0);
    for (int i = 0; i < 11; ++i) {
        if (i % 2 == 0) {
            ArrayList_push(list,(Object*)TRUE);
        }else {
            ArrayList_push(list,(Object*)FALSE);
        }
    }
    ASSERT_EQ(ArrayList_size(list),11);
    for (int j = 0; j < 11; ++j) {
        printf("ArrayList_get(list,%d)=%p,TRUE=%p\n",j,ArrayList_get(list,j),(Object*)TRUE);
        if (j % 2 == 0) {
            ASSERT_EQ((Bool*)ArrayList_get(list,j),TRUE);
        }else {
            ASSERT_EQ((Bool*)ArrayList_get(list,j),FALSE);
        }
    }
}

TEST(ArrayListIterator_init, ArrayList) {
    ArrayList* list = ArrayList_init((ArrayList*)alloc(ArrayListClass),10);
    struct Bool* TRUE = Bool_init((Bool *)alloc((ClassType*)BoolClass),true);
    struct Bool* FALSE = Bool_init((Bool *)alloc((ClassType*)BoolClass), false);
    for (int i = 0; i < 11; ++i) {
        if (i % 2 == 0) {
            ArrayList_push(list,(Object*)TRUE);
        }else {
            ArrayList_push(list,(Object*)FALSE);
        }
    }
    ArrayListIterator* iterator = ArrayList_getIterator(list);
    int i = 0;
    while (ArrayListIterator_hasNext(iterator)) {
        Bool* b = (Bool*) ArrayListIterator_next(iterator);
        printf("ArrayList_get(list,%d)=%p,TRUE=%p\n",i,b,(Object*)TRUE);
        if (i % 2 == 0) {
            ASSERT_EQ(b,TRUE);
        }else {
            ASSERT_EQ(b,FALSE);
        }
        i++;
    }
}