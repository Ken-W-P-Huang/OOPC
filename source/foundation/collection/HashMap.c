//
// Created by kenhuang on 2019-10-21.
//

#include "HashMap.h"
#ifdef __OVERRIDE__
static const float DEFAULT_LOAD_FACTOR =  0.75;
static const unsigned  int DEFAULT_INITIAL_CAPACITY = 32;
static const unsigned  int MAXIMUM_CAPACITY = 1 << 30;
PRIVATE(HashMap)
    /* entry个数 */
    unsigned int size;
    /* entryListArray最大个数 */
    unsigned int capacity;
    /* 加载因子  默认0.75 */
    float loadFactor;
    /* 被修改的次数 */
    int modCount;
    /* 存储元素的实体数组    默认初始长度为 16 */
    LinkedList** entryListArray;
    /* 临界值   当实际大小超过临界值时，会进行扩容threshold = 加载因子*容量 */
    //  int threshold;
END

struct HashMapEntry *HashMapEntry_init(struct HashMapEntry *self, int hash, Object *key, Object *value);
struct HashMapEntryIterator *HashMapEntryIterator_init(struct HashMapEntryIterator *self, HashMap *map,
        LinkedList **linkedListArray);
Object* HashMapEntry_getKey(struct HashMapEntry* self);
Object* HashMapEntry_getValue(struct HashMapEntry* self);
int HashMapEntry_getHash(struct HashMapEntry* self);
void HashMapEntry_setValue(struct HashMapEntry* self,Object* object);
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static int hash(Object* key){
    const ClassType* clazz = getClass(key);
    int h = clazz->hashCode(key);
    return (key == NULL) ? 0 : h ^ (h >> 16);
}

static int tableSizeFor(int cap){
    unsigned int n = (unsigned int)cap - 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return  n < 0 ? 1 :(n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
}

static struct HashMapEntry* getEntry(HashMap *self,int hash,Object* key){
    LinkedList** listArray = self->p->entryListArray;
    LinkedList* list;
    struct HashMapEntry* entry;
    if(listArray != NULL &&  (list =listArray[hash % (self->p->capacity - 1) ]) != NULL){
        struct LinkedListIterator* iterator = LinkedList_getIterator(list);
        while(LinkedListIterator_hasNext(iterator)){
            entry = (struct HashMapEntry *)LinkedListIterator_next(iterator);
            if(HashMapEntry_getHash(entry) == hash
               && (key != NULL && (getClass(key)->equals)(key,HashMapEntry_getKey(entry)))){
                return entry;
            }
        }
        destroy((Object*)iterator);
    }
    return NULL;
}

static struct HashMapEntry* removeEntry(HashMap *self,int hash,Object* key,Object* value,bool matchValue, bool movable){
    LinkedList* *listArray = self->p->entryListArray;
    LinkedList* list;
    struct HashMapEntry* entry;
    Object* k;
    Object* v;
    int h;
    if(listArray != NULL  && (list = listArray[ hash % (self->p->capacity - 1)]) != NULL){
        const ClassType* keyClass = NULL;
        const ClassType* valueClass = NULL;
        if (key != NULL) {
            keyClass = getClass(key);
        }
        if (value != NULL) {
            valueClass = getClass(value);
        }
        int size = LinkedList_getSize(list);
        for (int i = 0; i < size; ++i) {
            entry = (struct HashMapEntry*)LinkedList_get(list,i);
            k = HashMapEntry_getKey(entry);
            v = HashMapEntry_getValue(entry);
            h = HashMapEntry_getHash(entry);
            if(entry != NULL && h == hash && (k == key || (k != NULL && keyClass->equals(key,k)))
            && (!matchValue || v == value || (v != NULL && valueClass->equals(value,v)))){
                return (struct HashMapEntry*)LinkedList_removeWithIndex(list,i);
            }
        }
    }
    return NULL;
}
//todo 一致性哈希
static void resize(HashMap *self) {
    struct LinkedListIterator* iterator;
    struct HashMapEntry * entry;
    int index;
    HashMap_P* private = self->p;
    if(private->capacity >= MAXIMUM_CAPACITY){
        return;
    }
    unsigned int newCapacity =private->capacity * 2;
    LinkedList ** newEntryListArray =  (LinkedList**)calloc(1,sizeof(LinkedList*) * newCapacity);
    /**
     * 将旧的entryListArray上的每个List复制到newEntryListArray，并删除这些entryListArray及其节点。
     */
    for (int i = 0; i < private->capacity; i++) {
        if(!private->entryListArray[i]){
            continue;
        }
        iterator = LinkedList_getIterator(private->entryListArray[i]);
        while (LinkedListIterator_hasNext(iterator)) {
            entry = (struct HashMapEntry*)LinkedListIterator_next(iterator);
            index = HashMapEntry_getHash(entry) % (newCapacity - 1);
            if(!newEntryListArray[index]){
                newEntryListArray[index] = alloc((ClassType*)LinkedListClass);
            }
            LinkedList_add(newEntryListArray[index],(Object*)entry);
        }
        destroy((Object*)iterator);
        destroy((Object*)private->entryListArray[i]);
    }
    free((Object*)private->entryListArray);
    private->entryListArray = newEntryListArray;
    private->capacity = newCapacity;
}
static Object *putValue(HashMap *self, int hashCode, Object *key, Object *value, bool onlyIfAbsent) {
    struct HashMapEntry * entry;
    Object* oldValue = NULL;
    unsigned  int index;
    HashMap_P* private = self->p;
    entry = getEntry(self,hashCode,key);
    if (entry == NULL) {
        entry = HashMapEntry_init(alloc((ClassType*)HashMapEntryClass), hashCode, key, value);
        index =  hashCode % (private->capacity - 1) ;
        if(!private->entryListArray[index]){
            private->entryListArray[index] = alloc((ClassType*)LinkedListClass);
        }
        LinkedList_add(private->entryListArray[index],(Object *)entry);
        (private->size)++;
        if (private->size > private->capacity * private->loadFactor) {
            resize(self);
        }
    } else {
        /* key值相同，则将旧值替换为新值，并将旧值返回 */
        if(!onlyIfAbsent){
            oldValue =  HashMapEntry_getValue(entry);
            HashMapEntry_setValue(entry,value);
        }
    }
    return oldValue;
}

static void deinit(HashMap *self){
    if (self) {
        HashMap_clear(self);
        free(self->p->entryListArray);
    }
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
/**
 * 通过key得到数据，如果没有数据则返回null
 * @param self
 * @param key
 * @return
 */
Object* HashMap_get(HashMap *self,Object* key){
    struct HashMapEntry* entry = getEntry(self,hash(key),key);
    return entry == NULL ? NULL:HashMapEntry_getValue(entry);
}
/**
 * 使用全部参数创建HashMap
 * @param self
 * @param initialCapacity
 * @param loadFactor
 * @return
 */
HashMap *HashMap_initWith(HashMap *self, unsigned int initialCapacity, float loadFactor) {
    if (Object_init((Object*)self)) {
        if (initialCapacity > MAXIMUM_CAPACITY) {
            initialCapacity = MAXIMUM_CAPACITY;
        }
        HashMap_P * private = self->p;
        private->capacity = initialCapacity;
        private->loadFactor = loadFactor;
        private->size = 0;
        private->entryListArray = (LinkedList**)calloc(1, sizeof(LinkedList*) * private->capacity);
    }
    return self;
}

/**
 * 创建HashMap
 * @param self
 * @return
 */
HashMap *HashMap_init(HashMap *self) {
    self = HashMap_initWith(self, DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR);
    return self;
}


/**
 * 是否包含某个key
 * @param self
 * @param key
 * @return
 */
bool HashMap_containsKey(HashMap *const self, Object *const key) {
    return getEntry(self,hash(key),key) != NULL;
}
/**
 * //是否包含某个Value
 * @param self
 * @param key
 * @return
 */
bool HashMap_containsValue(HashMap *const self,Object *const value) {
    LinkedList** listArray = self->p->entryListArray;
    size_t length = sizeof(listArray)/ sizeof(LinkedList*);
    LinkedList* list = NULL;
    struct LinkedListIterator* iterator;
    struct HashMapEntry* entry;
    if(listArray != NULL && self->p->size > 0){
        const ClassType* valueClass = getClass(value);
        for (int i = 0; i < length; ++i) {
            iterator = LinkedList_getIterator(list);
            while(LinkedListIterator_hasNext(iterator)){
                entry = (struct HashMapEntry *)LinkedListIterator_next(iterator);
                if(HashMapEntry_getValue(entry) == value || (value != NULL && (valueClass->equals)(value,
                        HashMapEntry_getValue(entry)))){
                    return true;
                }
            }
            destroy((Object*)iterator);
        }
    }
    return false;
}
/**
 * 删除一条数据，返回删除的值
 * @param self
 * @param key
 * @return
 */
Object* HashMap_remove(HashMap *const self,Object* key){
    struct HashMapEntry* entry = removeEntry(self,hash(key),key,NULL,false,true);
    return entry ?  HashMapEntry_getValue(entry):NULL;

}
struct HashMapEntryIterator* HashMap_getIterator(HashMap *const self){
    return HashMapEntryIterator_init(alloc((ClassType*)HashMapEntryIteratorClass), self, self->p->entryListArray);
}
/**
 * 增加一条映射
 * @param self
 * @param key
 * @param value
 */
void HashMap_put(HashMap *const self, Object *const key, Object *const value) {
    putValue(self, hash(key), key, value, false);
}

/**
 * 获取存储的键值对的数量
 * @param self
 * @return
 */
unsigned int HashMap_getSize( HashMap * self) {
    return self->p->size;
}

/**
 * 清空entry和链表，但保留entryListArray，key和value的内存空间
 * @param self
 */
void HashMap_clear(HashMap * self){
    for (int i = 0; i < self->p->capacity; ++i) {
        Object* linkedlist = (Object*)self->p->entryListArray[i];
        if(linkedlist){
            destroy(linkedlist);
        }
        self->p->entryListArray[i] = NULL;
    }
}

/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(HashMap)
    Class_fillDeclaredFields((ClassType *) HashMapClass,
                             "size", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "capacity", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "loadFactor", FloatClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "modCount", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "entryListArray", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) HashMapClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              hash, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              tableSizeFor, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              getEntry, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              removeEntry, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              resize, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              putValue, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              HashMap_get, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_initWith, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_containsKey, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_containsValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_remove, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_getIterator, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_put, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_getSize, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMap_clear, AccessClass->READABLE_UNWRITABLE_PUBLIC, NULL);
END
#endif