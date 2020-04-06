//
// Created by kenhuang on 2019-11-12.
//
#include "ArrayList.h"
PUBLIC(ArrayListIterator,Object)
    unsigned  int cursor;
    ArrayList* list;
END
PRIVATE(ArrayListIterator)
END
PUBLIC_STATIC(ArrayListIterator,Object)
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(ArrayListIterator *self) {

}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
ArrayListIterator *ArrayListIterator_init(ArrayListIterator *self, ArrayList *list) {
    if (Object_init((Object*)self)) {
        self->list = list;
        self->cursor = 0;
    }
    return self;
}

bool ArrayListIterator_hasNext(ArrayListIterator* self) {
    return self->cursor < ArrayList_size(self->list);
}

Object* ArrayListIterator_next(ArrayListIterator* self) {
    return ArrayList_get(self->list, self->cursor++);
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(ArrayListIterator)
    Class_fillDeclaredFields((ClassType *) ArrayListIteratorClass,
                             "cursor", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "list", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) ArrayListIteratorClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              ArrayListIterator_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              ArrayListIterator_next, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              ArrayListIterator_hasNext, AccessClass->READABLE_UNWRITABLE_PUBLIC, NULL);
END
