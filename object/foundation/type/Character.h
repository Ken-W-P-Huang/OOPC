//
// Created by kenhuang on 2018/6/13.
//

#ifndef  Character_H
#define  Character_H

#include "../Object.h"
public_start(Character, Object)
/* 公有属性 */
    struct __Character* private;
public_end
/* 公有方法*/
Character* Character_new(Character*self,char character);
char Character_getValue(Character *self);

/* 静态方法 */
void CharacterClazz_fillFieldsMethods();

#endif //Character_H