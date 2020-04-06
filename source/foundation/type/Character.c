//
// Created by kenhuang on 2019-10-23.
//

#include "Character.h"


PRIVATE(Character)
    char value;
END

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void deinit(Character *self) {

}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Character *Character_init(Character *self,char character) {
    if (Object_init((Object*)self)) {
        self->p->value = character;
    }
    return self;
}

char Character_getValue(Character *self){
    return  self->p->value;
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
STATIC_INIT(Character)

    Class_fillDeclaredFields((ClassType *) CharacterClass,
                             "value", CharacterClass, AccessClass->READABLE_WRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) CharacterClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              Character_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              Character_getValue, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END