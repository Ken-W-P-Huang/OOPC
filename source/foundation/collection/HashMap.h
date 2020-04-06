//
// Created by kenhuang on 2019-10-21.
//
#ifndef OOPC_HASHMAP_H
#define OOPC_HASHMAP_H
#include "../Object.h"
#include "LinkedList.h"
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)
/***********************************************************************************************************************
 *  HashMap
 **********************************************************************************************************************/
PUBLIC(HashMap,Object)
END

PUBLIC_STATIC(HashMap,Object)
#if defined(__POLYMORPHISM__)
    HashMap *(*init)(HashMap *self);
    HashMap *(*initWith)(HashMap *self, unsigned int initialCapacity, float loadFactor);
    Object* (*get)(HashMap *self,Object* key);
    unsigned int (*getSize)(const HashMap *const self);
    unsigned int (*getCapacity)(const HashMap *const self);
    bool (*containsKey)(HashMap *const map,Object * const key);
    bool (*containsValue)(HashMap *const map,Object * const Value);
    void (*put)(HashMap *const self, Object *const key, Object *const value);
    Object* (*remove)(HashMap *const self,Object* key);
    void (*clear)(HashMap * self);
#endif
END

HashMap *HashMap_init(HashMap *self);
HashMap *HashMap_initWith(HashMap *self, unsigned int initialCapacity, float loadFactor);
Object* HashMap_get(HashMap *self,Object* key);
unsigned int HashMap_getSize(HashMap * self);
bool HashMap_containsKey(HashMap * map,Object * key);
bool HashMap_containsValue(HashMap * map,Object *  Value);
void HashMap_put(HashMap * self, Object * key, Object * value);
Object* HashMap_remove(HashMap * self,Object* key);
void HashMap_clear(HashMap * self);

extern struct HashMapEntryClassType *const HashMapEntryClass;
extern struct HashMapEntryIteratorClassType *const HashMapEntryIteratorClass;
struct HashMapEntryIterator* HashMap_getIterator(HashMap * self);
bool HashMapEntryIterator_hasNext(struct HashMapEntryIterator *self);
struct HashMapEntry * HashMapEntryIterator_next(struct HashMapEntryIterator *self);
Object* HashMapEntry_getKey(struct HashMapEntry* self);
Object* HashMapEntry_getValue(struct HashMapEntry* self);
int HashMapEntry_getHash(struct HashMapEntry* self);
#endif
#endif //OOPC_HASHMAP_H
