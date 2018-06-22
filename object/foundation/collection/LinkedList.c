//
// Created by kenhuang on 2018/6/5.
//

#include "LinkedList.h"
#include "../type/Pointer.h"
#include "../type/Integer.h"

private_start(LinkedList,Object)
/*  私有属性 */
    LinkedListNode* first;
    LinkedListNode* last;
    unsigned size;
private_end
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static Object* init(Object * _self, va_list *parms) {
    return Class_getSuperClass(LinkedListClazz)->init(_self,parms);;
}

static Object* deinit(Object* _self) {
    LinkedList_clear((LinkedList*)Class_getSuperClass(LinkedListClazz)->deinit(_self));
    return _self;
}
/**
 * 返回指定位置的node
 * @param self
 * @param index
 * @return
 */
LinkedListNode* getNode(LinkedList* self,unsigned index){
    if(index < (self -> private -> size >> 1)){
        LinkedListNode* node = self->private->first;
        for(int i = 0;i < index; i++) {
            node = node->next;
        }
        return  node;
    }else{
        LinkedListNode* node = self->private->last;
        for(int i = self -> private -> size -1; i > index; i--){
            node = node->prev;
        }
        return node;
    }
}

/**
 *
 * @param self
 * @param element
 */
static void linkFirst(LinkedList* self, Object* element){
    LinkedListNode* first = self->private->first;
    LinkedListNode* newNode = LinkedListNode_new(NULL, NULL, element, first);
    self->private->first = newNode;
    if(first == NULL){
        self->private->last = newNode;
    }else{
        first->prev = newNode;
    }
    self -> private -> size++;
}

/**
 *
 * @param self
 * @param element
 */
static void linkLast(LinkedList* self,  Object* element){
    __LinkedList* private = self->private;
    LinkedListNode* last = private->last;
    LinkedListNode* newNode = LinkedListNode_new(NULL, last, element, NULL);
    private->last = newNode;
    if(last == NULL){
        private->first = newNode;
    }else{
        last-> next = newNode;
    }
    private -> size++;
}
/**
 *
 * @param self
 * @param element
 */
static void linkBefore(LinkedList* self,  Object* element, LinkedListNode* successor ){
    LinkedListNode* predecessor = successor->prev;
    LinkedListNode* newNode = LinkedListNode_new(NULL, predecessor, element, successor);
    if(predecessor == NULL){
        self->private->first = newNode;
    }else{
        predecessor->next = newNode;
    }
    self -> private -> size++;
}

static void * unlinkFirst(LinkedList* self,LinkedListNode* first){
    Object * element = first->element;
    LinkedListNode* next = first->next;
    first->element = NULL;
    first->next = NULL;
    self->private->first = next;
    if(next == NULL){
        self->private->last = NULL;
    }else{
        next->prev = NULL;
    }
    self -> private -> size --;
    Object_destroy((Object*)first);
    return element;
}
static void * unlinkLast(LinkedList* self,LinkedListNode* last){
    Object * element = last->element;
    LinkedListNode* prev = last->prev;
    last->element = NULL;
    last->prev = NULL;
    self->private->last = prev;
    if(prev == NULL){
        self->private->first = NULL;
    }else{
        prev->next = NULL;
    }
    self -> private -> size--;
    free(last);
    return  element;
}
static Object* unlinkNode(LinkedList* self,LinkedListNode* node){
    Object* element = node->element;
    LinkedListNode* next = node->next;
    LinkedListNode* prev = node->prev;
    if(prev == NULL){
        self->private->first = next;
    }else{
        prev->next = next;
        node->prev = NULL;
    }
    if(next == NULL){
        self->private->last =prev;
    }else{
        next->prev = prev;
        node->next = NULL;
    }
    node->element = NULL;
    self -> private -> size--;
    Object_destroy((Object *)node);
    return  element;
}

/***********************************************************************************************************************
 *  公共方法
 **********************************************************************************************************************/

LinkedListIterator* LinkedList_getIterator(LinkedList* self){
    return LinkedListIterator_new(NULL, self->private->first);
}

unsigned LinkedList_getSize(LinkedList* self){
    return self->private->size;
}
int LinkedList_indexOf(LinkedList *self, Object *element){
    unsigned index = 0;
    if(element == NULL){
        for(LinkedListNode* node =self->private->first; node != NULL; node = node->next){
            if(node->element == NULL){
                return  index;
            }
            index++;
        }
    }else{
        for(LinkedListNode* node = self->private->first;node != NULL; node = node->next){
            if(element == node->element){
                return  index;
            }
            index++;
        }
    }
    return  -1;
}
bool LinkedList_contains(LinkedList* self,Object* object){
    return LinkedList_indexOf(self,object) != -1;
}
void LinkedList_addFirst(LinkedList* self, Object* element){
    linkFirst(self,element);
}
void LinkedList_addLast(LinkedList* self,Object* element){
    linkLast(self,element);
}
void LinkedList_add(LinkedList *self, Object *element){
    linkLast(self,element);
}

Object* LinkedList_get(LinkedList* self, unsigned index){
    if(index < self -> private -> size){
        return getNode(self,index)->element;
    }else{
        return NULL;
    }
}
Object* LinkedList_getFirst(LinkedList* self){
    return self->private->first->element;
}
Object* LinkedList_getLast(LinkedList* self){
    return self->private->last->element;
}
Object* LinkedList_removeFirst(LinkedList* self){
    LinkedListNode * first = self->private->first;
    return unlinkFirst(self,first);
}
Object* LinkedList_removeLast(LinkedList* self){
    LinkedListNode* last = self->private->last;
    return unlinkLast(self,last);
}

void LinkedList_push(LinkedList* self,Object *element){
    LinkedList_addFirst(self,element);
}
Object* LinkedList_pop(LinkedList* self){
    return LinkedList_removeFirst(self);
}

bool LinkedList_remove(LinkedList* self,Object* _element){
    Object* element = _element;
    if(self -> private -> size == 0 ){
        return false;
    }
    if(element == NULL){
        for(LinkedListNode* node = self->private->first; node != NULL; node = node->next){
            if(node->element == NULL){
                unlinkNode(self,node);
                return true;
            }
        }
    }else{
        for(LinkedListNode* node = self->private->first; node != NULL; node = node->next){
            if(element == node->element){
                unlinkNode(self,node);
                return true;
            }
        }
    }
    return  false;
}

Object* LinkList_setNodeWith(LinkedList* self, unsigned index,Object* element){
    Object* oldValue;
    if(index < self -> private -> size){
        LinkedListNode* node = getNode(self,index);
        oldValue = node->element;
        node->element = element;
        return oldValue;
    }
    return NULL;
}
void LinkedList_destroyAllElements(LinkedList* self){
    Object* element;
    if(self != NULL){
        for (int i = 0; i <self -> private -> size ; ++i) {
            element = LinkList_setNodeWith(self,i,NULL);
            Object_destroy(element);
        }
    }
}

void LinkedList_clear(LinkedList* self){
   if(self != NULL && self -> private -> size != 0 ){
       for(LinkedListNode* node = self->private->first; node != NULL;){
           LinkedListNode* next = node->next;
           node->element = NULL;
           node->prev = NULL;
           node->next = NULL;
           Object_destroy((Object*)node);
           node = next;
       }
       self->private->first = self->private->last = NULL;
       self -> private -> size = 0;
   }
}
LinkedList* LinkedList_new(LinkedList *self){
    if(!self){
        self = (LinkedList *) Class_newInstance(LinkedListClazz);
    }
    return self ;
}
LinkedListIterator*  LinkedList_iterator(LinkedList* self) {
    return LinkedListIterator_new(NULL, self->private->first);
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
class_start(LinkedList,Object,offsetof(LinkedList,private), sizeof(__LinkedList))
class_end
static void Class_init(Class1* class) {
    Class_registerLinkedListIteratorClazz();
    Class_registerLinkedListNodeClazz();
}

static void Class_deinit(Class1* class) {
    Class_unregisterLinkedListIteratorClazz();
    Class_unregisterLinkedListNodeClazz();
}
void LinkedListClazz_fillFieldsMethods(){
    Class_fillDeclaredFields(LinkedListClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "first",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "last",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "size",IntegerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(LinkedListClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              getNode,READABLE_UNWRITABLE_PRIVATE,
                              linkFirst,READABLE_UNWRITABLE_PRIVATE,
                              linkLast,READABLE_UNWRITABLE_PRIVATE,
                              linkBefore,READABLE_UNWRITABLE_PRIVATE,
                              unlinkFirst,READABLE_UNWRITABLE_PRIVATE,
                              unlinkLast,READABLE_UNWRITABLE_PRIVATE,
                              unlinkNode,READABLE_UNWRITABLE_PRIVATE,
                              LinkedList_getIterator,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_getSize,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_indexOf,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_contains,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_addFirst,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_addLast,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_add,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_get,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_getFirst,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_getLast,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_removeFirst,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_removeLast,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_push,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_remove,READABLE_UNWRITABLE_PUBLIC,
                              LinkList_setNodeWith,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_destroyAllElements,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_clear,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_new,READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_iterator,READABLE_UNWRITABLE_PUBLIC,
                              NULL);


    Class_fillDeclaredFields(LinkedListNodeClazz,
                             "element",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "next",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "prev",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "");
    Class_fillDeclaredMethods(LinkedListNodeClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              LinkedListNode_new,READABLE_UNWRITABLE_PUBLIC,
                              NULL);


    Class_fillDeclaredFields(LinkedListIteratorClazz,
                             "private",PointerClazz,READABLE_UNWRITABLE_PUBLIC,
                             "currentNode",PointerClazz,READABLE_UNWRITABLE_PRIVATE,
                             "");
    Class_fillDeclaredMethods(LinkedListIteratorClazz,
                              init,READABLE_UNWRITABLE_PRIVATE,
                              deinit,READABLE_UNWRITABLE_PRIVATE,
                              LinkedListIterator_new,READABLE_UNWRITABLE_PUBLIC,
                              LinkedListIterator_hasNext,READABLE_UNWRITABLE_PUBLIC,
                              LinkedListIterator_next,READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}

