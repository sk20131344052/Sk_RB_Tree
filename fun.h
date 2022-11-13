//
//  fun.h
//  AVL数
//
//  Created by 川十 on 2019/2/20.
//  Copyright © 2019年 川十. All rights reserved.
//

#ifndef fun_h
#define fun_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef int ElemType;
typedef struct node
{
    ElemType key;
    struct node  *lchild, *rchild;
    int height;
}BNode,*BTree;

void display(BTree root);
void preTraversal(BTree root);
void inTraversal(BTree root);
void saveInTraversal(BTree root,int *term);
int digits(int x);
int findNum(int *term, int x, int n);
int count(BTree T,int num);
int deepth(BTree root);
BTree insert(BTree t,int x);
int height(BTree T);
BTree creatTree(BTree root, int n);
void display2(BTree T);
BTree LL_rotation(BTree T);
BTree LR_rotation(BTree T);
BTree RR_rotation(BTree T);
BTree RL_rotation(BTree T);

#endif /* fun_h */
