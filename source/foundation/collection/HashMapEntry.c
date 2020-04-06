//
// Created by kenhuang on 2019-10-21.
//

#include "HashMap.h"
#if defined(__OVERRIDE__) || defined(__POLYMORPHISM__)

PRIVATE(HashMapEntry)
    int hash;
    Object * key;
    Object * value;
END

PUBLIC(HashMapEntry,Object)

END

PUBLIC_STATIC(HashMapEntry,Object)
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(HashMapEntry *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
HashMapEntry *HashMapEntry_init(HashMapEntry *self, int hash, Object *key, Object *value) {
    if (Object_init((Object*)self)) {
        self->p->hash = hash;
        self->p->key = key;
        self->p->value = value;
    }
    return self;
}


Object* HashMapEntry_getKey(HashMapEntry* self){
    return self->p->key;
}

Object* HashMapEntry_getValue(HashMapEntry* self){
    return self->p->value;
}
void HashMapEntry_setValue(HashMapEntry* self,Object* object){
    self->p->value = object;
}

int HashMapEntry_getHash(HashMapEntry* self){
    return self->p->hash;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(HashMapEntry)
    Class_fillDeclaredFields((ClassType *) HashMapEntryClass,
                             "hash", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "key", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "value", PointerClass, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) HashMapEntryClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              HashMapEntry_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMapEntry_getKey, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMapEntry_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              HashMapEntry_getHash, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END
#endif