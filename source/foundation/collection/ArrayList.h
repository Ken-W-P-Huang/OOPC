//
// Created by kenhuang on 2019-10-21.
//

#ifndef OOPC_ARRAYLIST_H
#define OOPC_ARRAYLIST_H
#include "../Object.h"
PUBLIC(ArrayList,Object)

END

PUBLIC_STATIC(ArrayList,Object)
#if defined(__POLYMORPHISM__)
    ArrayList *(*init)(ArrayList *self,unsigned int capacity);
    void (*push)(ArrayList* self, Object* element);
    Object* (*pop)(ArrayList* self) ;
    unsigned int (*size)(ArrayList* self) ;
    void (*removeAll)(ArrayList* self);
    struct ArrayListIterator* (*getIterator)(ArrayList *self) ;
    Object* (*get)(ArrayList* arrayList, size_t index) ;
#endif
END
ArrayList *ArrayList_init(ArrayList *self,unsigned int capacity);
void ArrayList_push(ArrayList* self, Object* element);
Object* ArrayList_pop(ArrayList* self) ;
unsigned int ArrayList_size(ArrayList* self) ;
void ArrayList_removeAll(ArrayList* self);
struct ArrayListIterator* ArrayList_getIterator(ArrayList *self) ;
Object* ArrayList_get(ArrayList* arrayList, size_t index) ;

extern struct ArrayListIteratorClassType *const ArrayListIteratorClass;
Object* ArrayListIterator_next(struct ArrayListIterator* self);
bool ArrayListIterator_hasNext(struct ArrayListIterator* self);

#endif //OOPC_ARRAYLIST_H
