//
// Created by kenhuang on 2018/6/10.
//

#include "ArrayList.h"
#include "../type/Integer.h"
#include "../type/Pointer.h"


private_start(ArrayList,Object)
/* 私有属性 */
    unsigned int size ;
    unsigned int capacity;
    Object** data;
private_end
/* 静态属性 */
static const int DEFAULT_CAPACITY = 10;
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object* init(Object*self, va_list *parms){
    return Class_getSuperClass(ArrayListClazz)->init(self,parms);
}

static Object* deinit(Object* self) {
    return Class_getSuperClass(ArrayListClazz)->deinit(self);
}
static void resize(ArrayList *arrayList, unsigned int capacity) {
    if (capacity <= arrayList->private->capacity) {
        return;
    }
    Object ** data = (Object**)calloc(1,sizeof(Object*) * capacity);
    memcpy(data, arrayList->private->data, arrayList->private->size * sizeof(Object *));
    free(arrayList->private->data);
    arrayList->private->data = data;
    arrayList->private->capacity = capacity;
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/

ArrayList* ArrayList_new(ArrayList *self, unsigned int capacity){
    if(!self){
        self = Class_newInstance(ArrayListClazz);
    }
    __ArrayList* private = self->private;
    if(capacity == 0){
        private->capacity = DEFAULT_CAPACITY;
    }else{
        private->capacity = capacity;
    }
    private->data = (Object**)calloc(1,sizeof(Object*) * capacity);
    private->size = 0;
    return self;
}

void ArrayList_push(ArrayList* self, Object* element) {
    if (self->private->capacity == self->private->size) {
        resize(self, self->private->capacity << 1);
    }
    self->private->data[self->private->size++] = element;
}
Object* ArrayList_pop(ArrayList* self) {
    return self->private->data[--self->private->size];
}

unsigned int ArrayList_size(ArrayList* self) {
    return self->private->size;
}

void ArrayList_removeAll(ArrayList* self) {
    self->private->size = 0;
}
/**
 * 使用时需要释放ArrayListIterator
 * @param self
 * @return
 */
ArrayListIterator* ArrayList_getIterator(ArrayList *self) {
    return ArrayListIterator_new(NULL, self);
}

Object* ArrayList_get(ArrayList* arrayList, size_t index) {
    return arrayList->private->data[index];
}

/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(ArrayList,Object,offsetof(ArrayList,private), sizeof(__ArrayList))
class_end
static void Class_init(Class1* class) {
    Class_registerArrayListIteratorClazz(isReflectable);
    if (isReflectable) {
        Class_fillDeclaredFields(ArrayListClazz,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "size",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "capacity",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "data",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(ArrayListClazz, init, READABLE_UNWRITABLE_PRIVATE,
                                  deinit, READABLE_UNWRITABLE_PRIVATE,
                                  resize, READABLE_UNWRITABLE_PRIVATE,
                                  ArrayList_new, READABLE_UNWRITABLE_PUBLIC,
                                  ArrayList_push, READABLE_UNWRITABLE_PUBLIC,
                                  ArrayList_pop, READABLE_UNWRITABLE_PUBLIC,
                                  ArrayList_size, READABLE_UNWRITABLE_PUBLIC,
                                  ArrayList_removeAll, READABLE_UNWRITABLE_PUBLIC,
                                  ArrayList_getIterator, READABLE_UNWRITABLE_PUBLIC,
                                  ArrayList_get, READABLE_UNWRITABLE_PUBLIC, NULL);
        Class_fillDeclaredFields(ArrayListIteratorClazz,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "cursor",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "list",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(ArrayListIteratorClazz,init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  resize,READABLE_UNWRITABLE_PRIVATE,
                                  ArrayListIterator_new,READABLE_UNWRITABLE_PUBLIC,
                                  ArrayListIterator_next,READABLE_UNWRITABLE_PUBLIC,
                                  ArrayListIterator_hasNext,READABLE_UNWRITABLE_PUBLIC,NULL);
    }
}

static void Class_deinit(Class1* class) {
    Class_unregisterArrayListIteratorClazz(isReflectable);
}
