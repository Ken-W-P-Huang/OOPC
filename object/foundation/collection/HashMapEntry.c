//
// Created by kenhuang on 18-6-9.
//

#include "HashMap.h"

private_start(HashMapEntry,Object)
/* 私有属性 */
    int hash;
    Object * key;

private_end
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object* init(Object* self,va_list* parms){
    return Class_getSuperClass(HashMapEntryClazz)->init(self,parms);
}
static Object* deinit(Object* self){
    return Class_getSuperClass(HashMapEntryClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
HashMapEntry *HashMapEntry_new(HashMapEntry *self, int hash, Object *key, Object *value) {
    if(!self){
        self = Class_newInstance(HashMapEntryClazz);
    }
    self->private->hash = hash;
    self->private->key = key;
    self->value = value;
    return  self;
}
Object* HashMapEntry_getKey(HashMapEntry* self){
    return self->private->key;
}
int HashMapEntry_getHash(HashMapEntry* self){
    return self->private->hash;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(HashMapEntry,Object,offsetof(HashMapEntry,private), sizeof(__HashMapEntry))
class_end
static void Class_init(Class1* class){
    if (isReflectable) {
        //Reflect
    }
}
static void Class_deinit(Class1* class){

}
