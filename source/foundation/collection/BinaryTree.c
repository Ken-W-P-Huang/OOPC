//
// Created by kenhuang on 2019-10-21.
//

#include "BinaryTree.h"
#include "../interface/Comparator.h"

typedef struct BinaryTreeNode{
    Object* data;
    struct BinaryTreeNode* left;
    struct BinaryTreeNode* right;
    struct BinaryTreeNode* parent;
} BinaryTreeNode;

typedef struct _BinaryTree {
    BinaryTreeNode* root;
    /*
     * 这里使用的是匿名接口，直接用接口即可。
     * 负值  object1 < object2
     * 0    object1 == object2
     * 正值  object1 > object2
     */
    Comparator* comparator;
} _BinaryTree;

/***********************************************************************************************************************
 *  私有方法
 **********************************************************************************************************************/
static bool isBare(BinaryTreeNode *self){
    return self->left == NULL && self->right;
}

static void clear(BinaryTreeNode* node){
    if (node != NULL) {
        if (node->left != NULL) {
            clear((BinaryTreeNode *)node->left);
            node->left = NULL;
        }
        if (node->right != NULL) {
            clear((BinaryTreeNode *)node->right);
            node->right = NULL;
        }
        if (isBare(node)) {
            destroy((Object*)node);
        }
    }
}

static void deinit(BinaryTree *self) {
    BinaryTree_clear(self);
}

/***********************************************************************************************************************
 *  公有方法
 **********************************************************************************************************************/
BinaryTree *BinaryTree_init(BinaryTree *self,Comparator* comparator) {
    if (Object_init((Object*)self)) {
        self->p->comparator = comparator;
    }
    return self;
}

void BinaryTree_clear(BinaryTree *self) {
    clear(self->p->root);
    self->p->root = NULL;
}

void BinaryTree_insertNode(BinaryTree *self, BinaryTreeNode* node){
    int code;
    struct _BinaryTree* private = self->p;
    BinaryTreeNode* parent = private->root;
    if (parent == NULL) {
        private->root = node;
        node->parent = private->root;
        return;
    }

    while (true){
        code = private->comparator->compare(parent->data, node->data);
        if (code >= 0) {
            if (parent->left == NULL) {
                parent->left = node;
                node->parent = parent;
                return;
            }else{
                parent = parent->left;
            }
        } else {
            if (parent->right == NULL) {
                parent->right = node;
                node->parent = parent;
                return;
            }else {
                parent = parent->right;
            }
        }
    }
}

void BinaryTree_insert(BinaryTree *self, Object* data){
    BinaryTreeNode* node = malloc(sizeof(BinaryTreeNode));
    node->data = data;
    BinaryTree_insertNode(self,node);
}

BinaryTreeNode* BinaryTree_getNode(BinaryTree *self, void* data){
    int code;
    struct _BinaryTree* private = self->p;
    BinaryTreeNode* parent = private->root;
    BinaryTreeNode* node = NULL;
    do {
        code = private->comparator->compare(parent->data, data);
        if (code > 0) {
            parent = parent->left;
        }else if (code < 0) {
            parent = parent->right;
        } else {
            node = parent;
        }
    }while (!isBare(parent));
    return  node;
}

void BinaryTree_deleteNode(BinaryTree *self, void* data){
    BinaryTreeNode* node = BinaryTree_getNode(self,data);
    if (node == NULL) {
        return;
    }else{
        BinaryTreeNode* parent = node->parent;
        BinaryTreeNode* rightSubTreeParent = NULL;
        BinaryTreeNode** p ;
        if (parent->left == node) {
            p = &(parent->left);
        }else{
            p = &(parent-> right);
        }
        if (node->left == NULL) {
            *p = node-> right;
            return;
        }else{
            *p = node->left;
            rightSubTreeParent = *p;
        }
        //将右子树放到最右端
        while (rightSubTreeParent->right != NULL) {
            rightSubTreeParent = rightSubTreeParent->right;
        }
        rightSubTreeParent->right = node->right;
    }
    destroy((Object*)node);
}
/***********************************************************************************************************************
 *  静态方法
 **********************************************************************************************************************/
static void clinit();
static BinaryTreeClassType BinaryTreeClazz[] = {
{
                NULL,
                "BinaryTree",
                sizeof(BinaryTree),
                sizeof(_BinaryTree),
                offsetof(BinaryTree, p),
                sizeof(BinaryTreeClassType),
#if defined(__CLASS_ATTRIBUTE__)
                REAL,
                false,
#endif
                clinit,
                (void (*)(Object *)) deinit,
}
};
BinaryTreeClassType *const BinaryTreeClass = BinaryTreeClazz;
static void clinit() {
    Class_fillDeclaredFields((ClassType *) BinaryTreeClass,
                             "root", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             "comparator", IntegerClass, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                             NULL);
    Class_fillDeclaredMethods((ClassType *) BinaryTreeClass,
                              deinit, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              clear, AccessClass->READABLE_UNWRITABLE_PRIVATE,
                              BinaryTree_init, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              BinaryTree_clear, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              BinaryTree_insertNode, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              BinaryTree_insert, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              BinaryTree_getNode, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              BinaryTree_deleteNode, AccessClass->READABLE_UNWRITABLE_PUBLIC,
                              NULL);
}