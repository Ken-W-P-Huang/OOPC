//
// Created by kenhuang on 2018/6/8.
//

#ifndef  HashMap_H
#define  HashMap_H

#include "../Object.h"
#include "LinkedList.h"
/***********************************************************************************************************************
 *  HashMap
 **********************************************************************************************************************/
/* 类型声明*/
public_start(HashMap,Object)
struct __HashMap* private;
public_end

public_start(HashMapEntryIterator,Object)
    struct __HashMapEntryIterator* private;
public_end
/* 公有方法*/
HashMap *HashMap_new(HashMap *self);
HashMap *HashMap_newWith(HashMap *self, unsigned int initialCapacity, float loadFactor);
Object* HashMap_get(HashMap *self,Object* key);
unsigned int HashMap_getSize(const HashMap *const self);
unsigned int HashMap_getCapacity(const HashMap *const self);
bool HashMap_ContainsKey(HashMap *const map,Object * const key);
bool HashMap_ContainsValue(HashMap *const map,Object * const Value);
void HashMap_put(HashMap *const self, Object *const key, Object *const value);
Object* HashMap_remove(HashMap *const self,Object* key);
void HashMap_clear(HashMap * self);
HashMapEntryIterator* HashMap_getIterator(HashMap *const self);
void HashMap_clearAllKeys(HashMap *self);
/* 静态方法*/
void HashMapClazz_fillFieldsMethods();
/***********************************************************************************************************************
 *  HashMapEntryIterator
 **********************************************************************************************************************/

/* 公有方法*/
HashMapEntryIterator *HashMapEntryIterator_new(HashMapEntryIterator *self, HashMap *map, LinkedList **linkedListArray);
bool HashMapEntryIterator_hasNext(HashMapEntryIterator *self);
struct HashMapEntry * HashMapEntryIterator_next(HashMapEntryIterator *self);
#endif //HashMap_H

/***********************************************************************************************************************
 *  HashMapEntry
 **********************************************************************************************************************/
public_start(HashMapEntry,Object)
    Object * value;
    struct __HashMapEntry* private;
public_end
/* 公有方法*/
HashMapEntry *HashMapEntry_new(HashMapEntry *self, int hash, Object *key, Object *value);
Object* HashMapEntry_getKey(HashMapEntry* self);
int HashMapEntry_getHash(HashMapEntry* self);