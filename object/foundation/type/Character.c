//
// Created by kenhuang on 2018/6/13.
//

#include "Character.h"
#include "Pointer.h"

private_start(Character, Object)
/* 私有属性 */
char value;
private_end
/* 静态属性 */

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static void *init(void *self, va_list *parms) {
    return Class_getSuperClass(CharacterClazz)->init(self, parms);
}

static void *deinit(void *self) {
    return Class_getSuperClass(CharacterClazz)->deinit(self);
}
/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
Character* Character_new(Character*self,char character){
    if(!self){
        self = Class_newInstance(CharacterClazz);
    }
    self->private->value = character;
    return self;
}
char Character_getValue(Character *self){
    return self->private->value;
}
/***********************************************************************************************************************
 * 静态方法
 **********************************************************************************************************************/
class_start(Character, Object,offsetof(Character,private), sizeof(__Character))

class_end
static void Class_init(Class1* class) {
}

static void Class_deinit(Class1* class) {

}
void CharacterClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(CharacterClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "value",CharacterClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(CharacterClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              Character_new,READABLE_UNWRITABLE_PUBLIC,
                              Character_getValue,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}