//
// Created by kenhuang on 2019-10-21.
//

#include "LinkedList.h"

typedef struct LinkedListNode{
    struct LinkedListNode * next;
    struct LinkedListNode * prev;
    Object * element;
} LinkedListNode;

PRIVATE(LinkedList)
    LinkedListNode* first;
    LinkedListNode* last;
    int size;
END

extern struct LinkedListIterator * LinkedListIterator_init(struct LinkedListIterator *self, struct LinkedListNode *node);
/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static LinkedListNode * newLinkedListNode(LinkedListNode *prev, Object *object,LinkedListNode *next) {
    LinkedListNode * self = malloc(sizeof(LinkedListNode));
    self->prev = prev;
    self->element = object;
    self->next = next;
    return self;
}

static void deinit(LinkedList *self) {
    if (self) {
        LinkedList_clear(self);
    }
}
/**
 * 返回指定位置的node
 * @param self
 * @param index
 * @return
 */
LinkedListNode* getNode(LinkedList* self,int index){
    if(index < (self ->p -> size >> 1)){
        LinkedListNode* node = self->p->first;
        for(int i = 0;i < index; i++) {
            node = (LinkedListNode*)node->next;
        }
        return  node;
    }else{
        LinkedListNode* node = self->p->last;
        for(int i = self ->p -> size -1; i > index; i--){
            node = (LinkedListNode*)node->prev;
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
    LinkedListNode* first = self->p->first;
    LinkedListNode* newNode = newLinkedListNode(NULL, element, first);
    self->p->first = newNode;
    if(first == NULL){
        self->p->last = newNode;
    }else{
        first->prev = (struct LinkedListNode*)newNode;
    }
    self ->p -> size++;
}

/**
 *
 * @param self
 * @param element
 */
static void linkLast(LinkedList* self,  Object* element){
    LinkedList_P* private = self->p;
    LinkedListNode* last = private->last;
    LinkedListNode* newNode = newLinkedListNode(last, element, NULL);
    private->last = newNode;
    if(last == NULL){
        private->first = newNode;
    }else{
        last-> next = (struct LinkedListNode*)newNode;
    }
    private -> size++;
}
/**
 *
 * @param self
 * @param element
 */
static void linkBefore(LinkedList* self,  Object* element, LinkedListNode* successor ){
    LinkedListNode* predecessor = (LinkedListNode*)successor->prev;
    LinkedListNode* newNode = newLinkedListNode(predecessor, element, successor);
    if(predecessor == NULL){
        self->p->first = newNode;
    }else{
        predecessor->next = (struct LinkedListNode*)newNode;
    }
    self ->p -> size++;
}

static void * unlinkFirst(LinkedList* self,LinkedListNode* first){
    Object * element = first->element;
    LinkedListNode* next = (LinkedListNode*)first->next;
    first->element = NULL;
    first->next = NULL;
    self->p->first = next;
    if(next == NULL){
        self->p->last = NULL;
    }else{
        next->prev = NULL;
    }
    self ->p -> size --;
    free(first);
    return element;
}
static void * unlinkLast(LinkedList* self,LinkedListNode* last){
    Object * element = last->element;
    LinkedListNode* prev = (LinkedListNode*)last->prev;
    last->element = NULL;
    last->prev = NULL;
    self->p->last = prev;
    if(prev == NULL){
        self->p->first = NULL;
    }else{
        prev->next = NULL;
    }
    self ->p -> size--;
    free(last);
    return  element;
}
static Object* unlinkNode(LinkedList* self,LinkedListNode* node){
    Object* element = node->element;
    LinkedListNode* next = node->next;
    LinkedListNode* prev = node->prev;
    if(prev == NULL){
        self->p->first = next;
    }else{
        prev->next = next;
        node->prev = NULL;
    }
    if(next == NULL){
        self->p->last =prev;
    }else{
        next->prev = prev;
        node->next = NULL;
    }
    node->element = NULL;
    self ->p -> size--;
    free(node);
    return  element;
}


/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
LinkedList *LinkedList_init(LinkedList *self) {
    if (Object_init((Object*)self)) {

    }
    return self;
}

struct LinkedListIterator* LinkedList_getIterator(LinkedList* self){
    return LinkedListIterator_init(alloc((ClassType*)LinkedListIteratorClass), self->p->first);
}

int LinkedList_getSize(LinkedList* self){
    return self->p->size;
}
int LinkedList_indexOf(LinkedList *self, Object *element){
    int index = 0;
    if(element == NULL){
        for(LinkedListNode* node =self->p->first; node != NULL; node = node->next){
            if(node->element == NULL){
                return  index;
            }
            index++;
        }
    }else{
        for(LinkedListNode* node = self->p->first;node != NULL; node = node->next){
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

Object* LinkedList_get(LinkedList* self, int index){
    if(index < self ->p -> size){
        return getNode(self,index)->element;
    }else{
        return NULL;
    }
}
Object* LinkedList_getFirst(LinkedList* self){
    return self->p->first->element;
}
Object* LinkedList_getLast(LinkedList* self){
    return self->p->last->element;
}
Object* LinkedList_removeFirst(LinkedList* self){
    LinkedListNode * first = self->p->first;
    return unlinkFirst(self,first);
}
Object* LinkedList_removeLast(LinkedList* self){
    LinkedListNode* last = self->p->last;
    return unlinkLast(self,last);
}

Object* LinkedList_removeWithIndex(LinkedList* self,int index){
    return LinkedList_removeFirst(self);
}


void LinkedList_push(LinkedList* self,Object *element){
    LinkedList_addFirst(self,element);
}
Object* LinkedList_pop(LinkedList* self){
    return LinkedList_removeFirst(self);
}

bool LinkedList_remove(LinkedList* self,Object* _element){
    Object* element = _element;
    if(self ->p -> size == 0 ){
        return false;
    }
    if(element == NULL){
        for(LinkedListNode* node = self->p->first; node != NULL; node = node->next){
            if(node->element == NULL){
                unlinkNode(self,node);
                return true;
            }
        }
    }else{
        for(LinkedListNode* node = self->p->first; node != NULL; node = node->next){
            if(element == node->element){
                unlinkNode(self,node);
                return true;
            }
        }
    }
    return  false;
}

Object* LinkList_set(LinkedList* self, int index,Object* element){
    Object* oldValue;
    if(index < self ->p -> size){
        LinkedListNode* node = getNode(self,index);
        oldValue = node->element;
        node->element = element;
        return oldValue;
    }
    return NULL;
}



void LinkedList_clear(LinkedList* self){
    if(self != NULL && self ->p -> size != 0 ){
        LinkedListNode *next,*node;
        for(node = self->p->first; node != NULL;){
            next = node->next;
            node->element = NULL;
            node->prev = NULL;
            node->next = NULL;
            free(node);
            node = next;
        }
        self->p->first = self->p->last = NULL;
        self ->p -> size = 0;
    }
}

/***********************************************************************************************************************
 *
 **********************************************************************************************************************/
STATIC_INIT(LinkedList)
    Class_fillDeclaredFields((ClassType *) LinkedListClass,
                             "first", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "last", PointerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "size", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) LinkedListClass,
                              newLinkedListNode, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              getNode, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              linkFirst, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              linkLast, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              linkBefore, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              unlinkFirst, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              unlinkLast, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              unlinkNode, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              LinkedList_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_getIterator, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_getSize, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_indexOf, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_contains, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_addFirst, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_addLast, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_add, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_get, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_getFirst, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_getLast, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_removeFirst, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_removeLast, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_removeWithIndex, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_push, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_pop, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_remove, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkList_set, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              LinkedList_clear, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
END


