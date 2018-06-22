//
// Created by kenhuang on 2018/6/8.
//

#include "HashMap.h"
#include "../type/Pointer.h"
#include "../type/Integer.h"
#include "../type/Float.h"

/* 类型声明 */
static const float DEFAULT_LOAD_FACTOR =  0.75;
static const unsigned  int DEFAULT_INITIAL_CAPACITY = 32;
static const unsigned  int MAXIMUM_CAPACITY = 1 << 30;
private_start(HashMap,Object)
    /* 私有属性 */
    unsigned int size;                      /* entry个数 */
    unsigned int capacity;                  /* entryListArray最大个数 */
    float loadFactor;                       /* 加载因子  默认0.75 */
    int modCount;                           /* 被修改的次数 */
    LinkedList** entryListArray;            /* 存储元素的实体数组    默认初始长度为 16 */
  //  int threshold;                        /* 临界值   当实际大小超过临界值时，会进行扩容threshold = 加载因子*容量 */
private_end
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object *init(Object *_self, va_list *parms) {
    return Class_getSuperClass(HashMapClazz)->init(_self, parms);
}

static Object *deinit(Object *_self) {
    HashMap* self = (HashMap*)Class_getSuperClass(HashMapClazz)->deinit(_self);
    HashMap_clear(self);
    free(self->private->entryListArray);
    return (Object *)self;
}
static int hash(Object* key){
    int h = (*Object_getClass(key)->hashCode)(key);
    return (key == NULL) ? 0 : h ^ ((unsigned int)h >> 16);
}
static HashMapEntry* getEntry(HashMap *self,int hash,Object* key){
    LinkedList** listArray = self->private->entryListArray;
    LinkedList* list;
    HashMapEntry* entry;
    if(listArray != NULL &&  (list =listArray[hash % (self->private->capacity - 1) ]) != NULL){
        LinkedListIterator* iterator = LinkedList_getIterator(list);
        while(LinkedListIterator_hasNext(iterator)){
            entry = (HashMapEntry *)LinkedListIterator_next(iterator);
            if(HashMapEntry_getHash(entry) == hash
               && (key != NULL && (*Object_getClass(key)->equals)(key,HashMapEntry_getKey(entry)))){
                return entry;
            }
        }
        Object_destroy((Object*)iterator);
    }
    return NULL;
}
static HashMapEntry* removeEntry(HashMap *self,int hash,Object* key,Object* value,bool matchValue, bool movable){
    LinkedList* *listArray = self->private->entryListArray;
    LinkedList* list;
    HashMapEntry* entry;
    if(listArray != NULL  && (list =listArray[(self->private->capacity - 1) & hash]) != NULL){
        LinkedListIterator* iterator = LinkedList_getIterator(list);
        const Class1* valueClass = Object_getClass(value);
        while(LinkedListIterator_hasNext(iterator)){
            entry = (HashMapEntry *)LinkedListIterator_next(iterator);
            if(HashMapEntry_getHash(entry) == hash
               && (key != NULL && (*valueClass->equals)(key,HashMapEntry_getKey(entry)))
               && (entry->value == value || (!matchValue && value != NULL && valueClass->equals(value,entry->value)))){
                break;
            }
        }
        Object_destroy((Object*)iterator);
    }

    return NULL;
}
static void resize(HashMap *self) {
    LinkedListIterator* iterator;
    HashMapEntry * entry;
    int index;
    __HashMap* private = self->private;
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
            entry = (HashMapEntry*)LinkedListIterator_next(iterator);
            index = HashMapEntry_getHash(entry) % (newCapacity - 1);
            if(!newEntryListArray[index]){
                newEntryListArray[index] = LinkedList_new(NULL);
            }
            LinkedList_add(newEntryListArray[index],(Object*)entry);
        }
        Object_destroy((Object*)iterator);
        Object_destroy((Object*)private->entryListArray[i]);
    }
    free((Object*)private->entryListArray);
    private->entryListArray = newEntryListArray;
    private->capacity = newCapacity;
}
static Object *putValue(HashMap *self, int hashCode, Object *key, Object *value, bool onlyIfAbsent) {
    HashMapEntry * entry;
    Object* oldValue;
    unsigned  int index;
    __HashMap* private = self->private;
    entry = getEntry(self,hashCode,key);
    if (entry == NULL) {
        entry = HashMapEntry_new(NULL, hashCode, key, value);
        index =  hashCode % (private->capacity - 1) ;
        if(!private->entryListArray[index]){
            private->entryListArray[index] = LinkedList_new(NULL);
        }
        LinkedList_add(private->entryListArray[index],(Object *)entry);
        (private->size)++;
        if (private->size > private->capacity * private->loadFactor) {
            resize(self);
        }
    } else {
        /* key值相同，则将旧值替换为新值，并将旧值返回 */
        if(!onlyIfAbsent){
            oldValue = entry->value;
            entry->value = value;
        }
    }
    return oldValue;
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
    HashMapEntry* entry = getEntry(self,hash(key),key);
    return entry == NULL ? NULL:entry->value;
}
/**
 * 使用全部参数创建HashMap
 * @param self
 * @param initialCapacity
 * @param loadFactor
 * @return
 */
HashMap *HashMap_newWith(HashMap *self, unsigned int initialCapacity, float loadFactor) {
    if(!self){
        self = Class_newInstance(HashMapClazz);
    }
    __HashMap * private = self->private;
    private->capacity = initialCapacity;
    private->loadFactor = loadFactor;
    private->size = 0;
    private->entryListArray = (LinkedList**)calloc(1, sizeof(LinkedList*) * private->capacity);
    return self;
}
/**
 * 创建HashMap
 * @param self
 * @return
 */
HashMap *HashMap_new(HashMap *self) {
    self = HashMap_newWith(self, DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR);
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
    LinkedList** listArray = self->private->entryListArray;
    size_t length = sizeof(listArray)/ sizeof(LinkedList*);
    LinkedList* list;
    LinkedListIterator* iterator;
    HashMapEntry* entry;
    if(listArray != NULL && self->private->size > 0){
        const Class1* valueClass = Object_getClass(value);
        for (int i = 0; i < length; ++i) {
            iterator = LinkedList_getIterator(list);
            while(LinkedListIterator_hasNext(iterator)){
                entry = (HashMapEntry *)LinkedListIterator_next(iterator);
                if(entry->value == value || (value != NULL && (*valueClass->equals)(value,entry->value))){
                    return true;
                }
            }
            Object_destroy((Object*)iterator);
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
    HashMapEntry* entry = removeEntry(self,hash(key),key,NULL,false,true);
    return entry ? NULL: entry->value;

}
HashMapEntryIterator* HashMap_getIterator(HashMap *const self){
    return HashMapEntryIterator_new(NULL, self, self->private->entryListArray);
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
unsigned int HashMap_getSize(const HashMap *const self) {
    return self->private->size;
}

/**
 * 数据容量,即链表的个数
 * @param self
 * @return
 */
unsigned int HashMap_getCapacity(const HashMap *const self) {
    return self->private->capacity;
}
/**
 * 清空entry和链表，但保留entryListArray，key和value的内存空间
 * @param self
 */
void HashMap_clear(HashMap * self){
    for (int i = 0; i < self->private->capacity; ++i) {
        Object* linkedlist = (Object*)self->private->entryListArray[i];
        if(linkedlist){
            Object_destroy(linkedlist);
        }
        self->private->entryListArray[i] = NULL;
    }
}
/**
 * 清空HashMap的所有key的空间
 * @param self
 */
void HashMap_clearAllKeys(HashMap * self){
    HashMapEntryIterator* iterator = HashMap_getIterator(self);
    HashMapEntry* entry;
    while(HashMapEntryIterator_hasNext(iterator)){
        entry = HashMapEntryIterator_next(iterator);
        Object_destroy(HashMapEntry_getKey(entry));
    }
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(HashMap,Object,offsetof(HashMap,private), sizeof(__HashMap))
class_end
static void Class_init(Class1* class) {
    Class_registerHashMapEntryClazz();
    Class_registerHashMapEntryIteratorClazz();
}

static void Class_deinit(Class1* class) {
    Class_unregisterHashMapEntryClazz(isReflectable);
    Class_unregisterHashMapEntryIteratorClazz(isReflectable);
}

void HashMapClazz_fillFieldsMethods(){
        Class_fillDeclaredFields(HashMapClazz,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "size",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "capacity",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "loadFactor",FloatClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "modCount",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "entryListArray",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(HashMapClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  hash,READABLE_UNWRITABLE_PRIVATE,
                                  getEntry,READABLE_UNWRITABLE_PRIVATE,
                                  removeEntry,READABLE_UNWRITABLE_PRIVATE,
                                  resize,READABLE_UNWRITABLE_PRIVATE,
                                  putValue,READABLE_UNWRITABLE_PRIVATE,
                                  HashMap_get,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_newWith,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_new,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_containsKey,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_containsValue,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_remove,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_getIterator,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_put,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_getSize,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_getCapacity,READABLE_UNWRITABLE_PUBLIC,
                                  HashMap_clear,READABLE_UNWRITABLE_PUBLIC,NULL);

        Class_fillDeclaredFields(HashMapEntryClazz,
                                 "value",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "hash",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "key",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(HashMapEntryClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  HashMapEntry_new,READABLE_UNWRITABLE_PUBLIC,
                                  HashMapEntry_getKey,READABLE_UNWRITABLE_PUBLIC,
                                  HashMapEntry_getHash,READABLE_UNWRITABLE_PUBLIC,
                                  NULL);


        Class_fillDeclaredFields(HashMapEntryIteratorClazz,
                                 "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                                 "index",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "listIndex",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "map",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "count",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "listArray",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                                 "");
        Class_fillDeclaredMethods(HashMapEntryIteratorClazz,
                                  init,READABLE_UNWRITABLE_PRIVATE,
                                  deinit,READABLE_UNWRITABLE_PRIVATE,
                                  hash,READABLE_UNWRITABLE_PRIVATE,
                                  HashMapEntryIterator_new,READABLE_UNWRITABLE_PUBLIC,
                                  HashMapEntryIterator_hasNext,READABLE_UNWRITABLE_PUBLIC,
                                  HashMapEntryIterator_next,READABLE_UNWRITABLE_PUBLIC, NULL);

}