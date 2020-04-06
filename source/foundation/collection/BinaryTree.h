//
// Created by kenhuang on 2019-10-21.
//

#ifndef OOPC_BINARYTREE_H
#define OOPC_BINARYTREE_H

#include "../Object.h"
#include "../interface/Comparator.h"

typedef struct {
    Object _;
    struct _BinaryTree *p;
} BinaryTree;

typedef struct {
    ObjectClassType _;
#if defined(__POLYMORPHISM__)
    BinaryTree * (*init)(BinaryTree *self,Comparator*comparator);
    void  (*insert)(BinaryTree *self, Object* data);
    void  (*clear)(BinaryTree *self);
#endif
} BinaryTreeClassType;
extern BinaryTreeClassType *const BinaryTreeClass;
BinaryTree *BinaryTree_init(BinaryTree *self,Comparator* comparator);
void BinaryTree_insert(BinaryTree *self, Object* data);
void BinaryTree_clear(BinaryTree *self);
#endif //OOPC_BINARYTREE_H
