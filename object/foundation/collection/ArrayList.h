//
// Created by kenhuang on 2018/6/10.
//

#ifndef  ArrayList_H
#define  ArrayList_H
#include "../Object.h"


/***********************************************************************************************************************
 *  ArrayList
 **********************************************************************************************************************/

public_start(ArrayList,Object)
/* 公有属性 */
    struct __ArrayList* private;
public_end

/* 公有方法*/
ArrayList* ArrayList_new(ArrayList *self, unsigned int capacity);
void ArrayList_push(ArrayList* self, Object* element);
Object* ArrayList_pop(ArrayList* self) ;
unsigned int ArrayList_size(ArrayList* self) ;
void ArrayList_removeAll(ArrayList* self);
struct ArrayListIterator* ArrayList_getIterator(ArrayList *self) ;
Object* ArrayList_get(ArrayList* arrayList, size_t index) ;

/* 静态方法 */
/***********************************************************************************************************************
 *  ArrayListIterator
 **********************************************************************************************************************/

public_start(ArrayListIterator,Object)
/* 公有属性 */
    struct __ArrayListIterator* private;
public_end
/* 公有方法*/
ArrayListIterator* ArrayListIterator_new(ArrayListIterator *self, ArrayList *list);
Object* ArrayListIterator_next(ArrayListIterator* self);
bool ArrayListIterator_hasNext(ArrayListIterator* self);
/* 静态方法 */

#endif //ArrayList_H