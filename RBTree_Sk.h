
#ifndef RBTree_Sk_H
#define RBTree_Sk_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


typedef int ElemType;
typedef int ElemValue;

#define RB_BLACK 0
#define RB_RED   1

#define RB_WRONGW_VALUE -1000000



typedef struct node
{
    ElemType key;       
    ElemValue value;

    struct node *parent;
    struct node  *lchild, *rchild;

    int color;  //0表示黑色 1表示红色
    
    int height;
}BNode,*BTree,RBNode;


RBNode* getNode(ElemType key);

void leftRotate(RBNode *p); //左旋
void rightRotate(RBNode *p); //右旋
RBNode *predecessor(RBNode *node); //找到指定节点的前驱节点
RBNode *successor(RBNode *node);   //找到指定节点的后继节点
void put(ElemType key, ElemValue value); //插入红黑树节点
void fixAfterPut(RBNode* x);  //红黑树调整插入的节点


int deepth(BTree root);
void saveInTraversal(BTree root,int *term);
int findNum(int *term, int x, int n);
int count(BTree T,int num);
int digits(int x);
void Display(BTree root);

ElemValue RB_Remove(ElemType key);


#endif