//
// Created by kenhuang on 2019-10-21.
//

#include "ArrayList.h"

PRIVATE(ArrayList)
    unsigned int size ;
    unsigned int capacity;
    Object** dataArray;
END

extern struct ArrayListIterator *ArrayListIterator_init(struct ArrayListIterator *self, ArrayList *list);
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(ArrayList *self) {
    free(self->p->dataArray);
}

static void resize(ArrayList *arrayList, unsigned int capacity) {
    if (capacity <= arrayList->p->capacity) {
        return;
    }
    Object ** dataArray = (Object**)calloc(1,sizeof(Object*) * capacity);
    memcpy(dataArray, arrayList->p->dataArray, arrayList->p->size * sizeof(Object *));
    free(arrayList->p->dataArray);
    arrayList->p->dataArray = dataArray;
    arrayList->p->capacity = capacity;
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
ArrayList *ArrayList_init(ArrayList *self, unsigned int capacity) {
    static const int DEFAULT_CAPACITY = 10;
    if(Object_init((Object*)self)){
        ArrayList_P* private = self->p;
        if(capacity == 0){
            private->capacity = DEFAULT_CAPACITY;
        }else{
            private->capacity = capacity;
        }
        private->dataArray = (Object**)calloc(1,sizeof(Object*) * capacity);
        private->size = 0;
    }
    return self;
}

void ArrayList_push(ArrayList* self, Object* element) {
    if (self->p->capacity <= self->p->size) {
        resize(self, self->p->capacity << 1);
    }
    self->p->dataArray[self->p->size++] = element;
}

Object* ArrayList_pop(ArrayList* self) {
    return self->p->dataArray[--self->p->size];
}

unsigned int ArrayList_size(ArrayList* self) {
    return self->p->size;
}

void ArrayList_removeAll(ArrayList* self) {
    self->p->size = 0;
}


Object* ArrayList_get(ArrayList* arrayList, size_t index) {
    return arrayList->p->dataArray[index];
}

/**
 * 使用时需要释放ArrayListIterator
 * @param self
 * @return
 */
struct ArrayListIterator* ArrayList_getIterator(ArrayList *self) {
    return ArrayListIterator_init(alloc((ClassType*)ArrayListIteratorClass), self);
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(ArrayList)
    Class_fillDeclaredFields((ClassType *) ArrayListClass,
                             "size", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "capacity", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "dataArray", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) ArrayListClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              resize, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              ArrayList_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              ArrayList_push, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              ArrayList_pop, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              ArrayList_size, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              ArrayList_removeAll, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              ArrayList_getIterator, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              ArrayList_get, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END

