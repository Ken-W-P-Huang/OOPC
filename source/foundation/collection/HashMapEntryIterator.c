//
// Created by kenhuang on 2019-10-21.
//

#include "HashMap.h"
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)

PRIVATE(HashMapEntryIterator)
    //第几个链表中的第几个元素
    unsigned int index;
    //第几个链表
    unsigned  int listIndex;
    HashMap *map;
    //第几个键值对
    unsigned int count;
    LinkedList** listArray;
END
PUBLIC(HashMapEntryIterator,Object)

END

PUBLIC_STATIC(HashMapEntryIterator,Object)
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(HashMapEntryIterator *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
HashMapEntryIterator *HashMapEntryIterator_init(HashMapEntryIterator *self, HashMap *map, LinkedList **linkedListArray) {
    if (Object_init((Object*)self)) {
        HashMapEntryIterator_P* private = self->p;
        private->count = 0;
        private->index = 0;
        private->map = map;
        private->listArray = linkedListArray;
    }
    return self;
}


bool HashMapEntryIterator_hasNext(HashMapEntryIterator *self){
    return self->p->count < HashMap_getSize(self->p->map);
}

struct HashMapEntry * HashMapEntryIterator_next(HashMapEntryIterator *self){
    struct HashMapEntry * entry;
    HashMapEntryIterator_P* private = self->p;
    if(!private->listArray[private->listIndex] ||
       private->index >= LinkedList_getSize(private->listArray[private->listIndex])){
        private->index = 0;
        (private->listIndex)++;
        while(!private->listArray[private->listIndex]){
            (private->listIndex)++;
        }
    }
    entry = (struct HashMapEntry*)LinkedList_get(private->listArray[private->listIndex],private->index);
    private->count++;
    private->index++;
    return entry;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(HashMapEntryIterator)
    Class_fillDeclaredFields((ClassType *) HashMapEntryIteratorClass,
                             "index", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "listIndex", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "map", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "count", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "listArray", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) HashMapEntryIteratorClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              HashMapEntryIterator_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMapEntryIterator_hasNext, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMapEntryIterator_next, AccessClass->READABLE_UNWRITABLE_PUBLIC, NULL);
END
#endif
