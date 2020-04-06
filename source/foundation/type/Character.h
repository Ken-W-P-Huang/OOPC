//
// Created by kenhuang on 2019-10-23.
//

#ifndef OOPC_CHARACTER_H
#define OOPC_CHARACTER_H

#include "../Object.h"

PUBLIC(Character,Object)
END

PUBLIC_STATIC(Character,Object)
#if defined(__POLYMORPHISM__)
    Character *(*init)(Character *self,char character);
    char (*getValue)(Character *self);
#endif
END

Character *Character_init(Character *self,char character);
char Character_getValue(Character *self);
#endif //OOPC_CHARACTER_H
