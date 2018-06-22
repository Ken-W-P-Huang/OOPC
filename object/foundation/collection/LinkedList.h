//
// Created by kenhuang on 2018/6/5.
//

#ifndef  LinkedList_H
#define  LinkedList_H
#include "../Object.h"
/***********************************************************************************************************************
 *  LinkedList (本类不对Element做任何处理,同一个元素可以插入多次)
 **********************************************************************************************************************/
public_start(LinkedList,Object)
/* 公有属性 */
   struct __LinkedList* private;
public_end
/* 公共方法*/

LinkedList* LinkedList_new(LinkedList *self);
struct LinkedListIterator* LinkedList_getIterator(LinkedList* self);
void LinkedList_addFirst(LinkedList* self, Object* element);
void LinkedList_addLast(LinkedList* self,Object* element);
bool LinkedList_remove(LinkedList* self,Object* element);
void LinkedList_add(LinkedList *self, Object *element);
int LinkedList_indexOf(LinkedList *self, Object *element);
Object* LinkedList_get(LinkedList* self, unsigned index);
Object* LinkedList_getLast(LinkedList* self);
Object* LinkedList_getFirst(LinkedList* self);
void LinkedList_clear(LinkedList* self);
Object* LinkList_set(LinkedList* self, unsigned index,Object* element);
void LinkedList_destroyAllElements(LinkedList* self);
unsigned LinkedList_getSize(LinkedList* self);
Object* LinkedList_pop(LinkedList* self);
void LinkedList_push(LinkedList* self,Object *element);
/* 静态方法*/
void LinkedListClazz_fillFieldsMethods();
/***********************************************************************************************************************
 *  LinkedListNode
 **********************************************************************************************************************/
/* 类型声明*/
public_start(LinkedListNode,Object)
    Object * element;
    struct LinkedListNode * next;
    struct LinkedListNode * prev;
public_end
/* 公有方法*/
LinkedListNode* LinkedListNode_new(LinkedListNode *self, LinkedListNode *prev, Object *element, LinkedListNode *next);
/* 静态方法 */
/***********************************************************************************************************************
 *  LinkedListIterator
 **********************************************************************************************************************/
public_start(LinkedListIterator,Object)
/* 公有属性 */
   struct __LinkedListIterator* private;
public_end
/* 公有方法*/
LinkedListIterator *LinkedListIterator_new(LinkedListIterator *self, LinkedListNode *node);
bool LinkedListIterator_hasNext(LinkedListIterator *self);
Object *LinkedListIterator_next(LinkedListIterator *self);

/* 静态方法 */
#endif //LinkedList_H