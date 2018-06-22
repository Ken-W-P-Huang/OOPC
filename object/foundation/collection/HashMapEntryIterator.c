//
// Created by kenhuang on 18-6-9.
//

#include "HashMap.h"

private_start(HashMapEntryIterator,Object)
    unsigned int index;          //第几个链表中的第几个元素
    unsigned  int listIndex;     //第几个链表
    HashMap *map;
    unsigned int count;        //第几个键值对
    LinkedList** listArray;
private_end
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object* init(Object* self,va_list* parms){
    return Class_getSuperClass(HashMapEntryIteratorClazz)->init(self,parms);
}
static Object* deinit(Object* self){
    return Class_getSuperClass(HashMapEntryIteratorClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
HashMapEntryIterator *HashMapEntryIterator_new(HashMapEntryIterator *self, HashMap *map, LinkedList **linkedListArray) {
    if(!self){
        self = Class_newInstance(HashMapEntryIteratorClazz);
    }
    __HashMapEntryIterator* private = self->private;
    private->count = 0;
    private->index = 0;
    private->map = map;
    private->listArray = linkedListArray;
    return self;
}
bool HashMapEntryIterator_hasNext(HashMapEntryIterator *self){
    return self->private->count < HashMap_getSize(self->private->map);
}
HashMapEntry * HashMapEntryIterator_next(HashMapEntryIterator *self){
    HashMapEntry * entry;
    __HashMapEntryIterator* private = self->private;
    if(!private->listArray[private->listIndex] ||
            private->index >= LinkedList_getSize(private->listArray[private->listIndex])){
        private->index = 0;
        (private->listIndex)++;
        while(!private->listArray[private->listIndex]){
            (private->listIndex)++;
        }
    }
    entry = (HashMapEntry*)LinkedList_get(private->listArray[private->listIndex],private->index);
    private->count++;
    private->index++;
    return entry;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(HashMapEntryIterator,Object,offsetof(HashMapEntryIterator,private), sizeof(__HashMapEntryIterator))
class_end
static void Class_init(Class1* class){
    if (isReflectable) {
        //Reflect
    }
}
static void Class_deinit(Class1* class){

}
