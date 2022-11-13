

// https://blog.csdn.net/weixin_40568729/article/details/88040141

//
//  fun.c
//  AVL数
//
//  Created by 川十 on 2019/2/20.
//  Copyright © 2019年 川十. All rights reserved.
//

#include "fun.h"
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MAX_DIGITS 4
#define MAX_NODE 100
#define MAX_TMP 10000  //MAX_NODE^2
int a=0;

void display(BTree root)
{
    int k=0;
    int j=1;
    int n=1;
    int deep=deepth(root);
    BTree N;
    N=(BTree)malloc(sizeof(BNode));
    N->key=0;
    N->lchild=NULL;
    N->rchild=NULL;
    N->height=0;
    BTree term[MAX_TMP]={NULL};
    term[0]=root;
    while(n!=deep+1)
    {
        if(term[k]->lchild!=NULL&&term[k]->lchild!=N)
            term[j++]=term[k]->lchild;
        else
            term[j++]=N;
        if(term[k]->rchild!=NULL&&term[k]->rchild!=N)
            term[j++]=term[k]->rchild;
        else
            term[j++]=N;
        k++;
        if(k==pow(2,n)-1)
        {
            for(int i=pow(2,n-1)-1;i<k;i++)
            {
                if(term[i]!=N)
                    printf("%d ",term[i]->key);
                else
                    printf("* ");
            }
            printf("\n");
            n++;
        }
    }
}

int deepth(BTree root)
{
    if (!root)
    {
        return 0;
    }
    int left=deepth(root->lchild);
    int right=deepth(root->rchild);
    return left>=right ? left+1:right+1;
}

BTree creatTree(BTree root, int n)
{
    srand((unsigned) time(NULL));
    root->rchild=NULL;
    root->lchild=NULL;
    root->height=0;
    root->key=abs(rand())%1000;
    int term[MAX_NODE]={0};
    int flag;
    term[0]=root->key;
    for(int i=1;i<n;i++)
    {
        flag=0;
        int num=abs(rand())%1000;
        for(int j=0;j<i;j++)
        {
            if(term[j]==num)
            {
                flag=1;
                i--;
            }
        }
        if(!flag)
        {
            root=insert(root, num);
            term[i]=num;
        }
    }
    
    return root;
}


BTree insert(BTree t,int x)
{
    if(!t)
    {
        t=(BTree)malloc(sizeof(BNode));
        t->key=x;
        t->lchild=NULL;
        t->rchild=NULL;
        t->height=0;
    }
    else if(x<t->key)
    {
        t->lchild=insert(t->lchild,x);
        if(height(t->lchild)-height(t->rchild)==2)
        {
            if(x<t->lchild->key)
                t=LL_rotation(t);
            else
                t=LR_rotation(t);
        }
    }
    else if(x>t->key)
    {
        t->rchild=insert(t->rchild,x);
        if(height(t->rchild)-height(t->lchild)==2)
        {
            if(x>t->rchild->key)
                t=RR_rotation(t);
            else
                t=RL_rotation(t);
        }
    }
    else
    {
        printf("不允许插入相同节点！\n");
    }
    t->height=MAX(height(t->lchild),height(t->rchild))+1;
    return t;
}

BTree LL_rotation(BTree T)
{
    BTree k2=T->lchild;
    T->lchild=k2->rchild;
    k2->rchild=T;
    T->height=MAX(height(T->rchild),height(T->lchild))+1;
    k2->height=MAX(height(T->rchild),height(T->lchild))+1;
    
    return k2;
}
BTree LR_rotation(BTree T)
{
    T->lchild=RR_rotation(T->lchild);
    T=LL_rotation(T);
    return T;
}
BTree RR_rotation(BTree T)
{
    BTree k2=T->rchild;
    T->rchild=k2->lchild;
    k2->lchild=T;
    T->height=MAX(height(T->rchild),height(T->lchild))+1;
    k2->height=MAX(height(T->rchild),height(T->lchild))+1;
    
    return k2;
}
BTree RL_rotation(BTree T)
{
    T->rchild=LL_rotation(T->rchild);
    T=RR_rotation(T);
    return T;
}


int height(BTree T)
{
    if(!T)
        return 0;
    else
        return T->height;
}

void preTraversal(BTree root)
{
    if(!root)
        return;
    else
    {
        printf("%d ",root->key);
        preTraversal(root->lchild);
        preTraversal(root->rchild);
    }
}

void inTraversal(BTree root)
{
    if(!root)
        return;
    inTraversal(root->lchild);
    printf("%d ",root->key);
    inTraversal(root->rchild);
}

void saveInTraversal(BTree root,int *term)
{
    if(!root)
        return;
    else
    {
        saveInTraversal(root->lchild,term);
        term[a++]=root->key;
        saveInTraversal(root->rchild,term);
    }
}

int count(BTree T,int num)
{
    if(!T)
        return num;
    else
    {
        num=count(T->lchild,num);
        num=count(T->rchild,num);
    }
    num++;
    return num;
}

void display2(BTree root)
{
    int absotiveDistance[2];//绝对距离，[1]表示上一个兄弟节点，[0]-[1]用于计算节点的相对距离
    int array[MAX_NODE]={0};//用于储存搜索二叉树的中序遍历序列
    int relativeDistance=0;//相对距离
    int leftLineNum=0;//节点左边需打印的下划线符号个数
    int rightLineNum=0;//节点右边需打印的下划线符号个数
    int rightLineNum2=0;//零时变量，用于储存需打印节点的上一个兄弟节点右边打印了的下划线符号个数，便于计算需移动位数。（relativeDistance-rightLineNum2）
    int vertiLineArray[MAX_NODE]={0};//竖线位置存储
    int vertiLineNum=0;//用于记录下一行需打印的竖线个数 
    
    //定义标志节点N，节点的子孩子为空时用于占位识别
    BTree N;
    N=(BTree)malloc(sizeof(BNode));
    N->key=-1;
    N->lchild=NULL;
    N->rchild=NULL;
    N->height=0;
    
    int deep=deepth(root);
    saveInTraversal(root, array);
    
    //临时变量，用于层序遍历
    int k=0;
    int j=1;
    int n=1;
    
    BTree term[MAX_TMP]={NULL};//指针数组，储存树节点，用于层序遍历
    term[0]=root;
    while(n!=deep+1)
    {
        absotiveDistance[0]=0;
        absotiveDistance[1]=0;
        if(term[k]->lchild!=NULL&&term[k]->lchild!=N)
            term[j++]=term[k]->lchild;
        else
            term[j++]=N;
        if(term[k]->rchild!=NULL&&term[k]->rchild!=N)
            term[j++]=term[k]->rchild;
        else
            term[j++]=N;
        k++;
        if(k==pow(2,n)-1)
        {
            rightLineNum2=0;
            vertiLineNum=0;
            for(int i=pow(2,n-1)-1;i<k;i++)
            {
                leftLineNum=0;
                rightLineNum=0;
                absotiveDistance[0]=findNum(array, term[i]->key, count(root, 0))*MAX_DIGITS;
                if(absotiveDistance[0]==-MAX_DIGITS)
                    continue;
                relativeDistance=absotiveDistance[0]-absotiveDistance[1];

                absotiveDistance[0]+=digits(term[i]->key)+2;

                absotiveDistance[1]=absotiveDistance[0];
                if(term[i]->lchild!=N&&term[i]->lchild!=NULL)
                {
                    leftLineNum=(absotiveDistance[0]-findNum(array, term[i]->lchild->key, count(root, 0))*MAX_DIGITS)-digits(term[i]->key)-4;
                    vertiLineArray[vertiLineNum++]=findNum(array, term[i]->lchild->key, count(root, 0))*MAX_DIGITS+2;
                }
                if(term[i]->rchild!=N&&term[i]->rchild!=NULL)
                {
                    rightLineNum=(findNum(array, term[i]->rchild->key, count(root, 0))*MAX_DIGITS-absotiveDistance[0])+2;
                    vertiLineArray[vertiLineNum++]=findNum(array, term[i]->rchild->key, count(root, 0))*MAX_DIGITS+1;
                }
                for(int m=0;m<(rightLineNum2==0?(relativeDistance-leftLineNum) : (relativeDistance-leftLineNum-rightLineNum2));m++)
                {
                    printf(" ");
                }
                
                for(int m=0;m<leftLineNum;m++)
                {
                    printf("_");
                }
               
                if(term[i]!=N)
                {
                    printf("(%d)",term[i]->key);
                }
                
                for(int m=0;m<rightLineNum;m++)
                    printf("_");
                 
                rightLineNum2=rightLineNum;
            }
            printf("\n");
            if(vertiLineNum)
            {
            for(int m=0;m<vertiLineNum;m++)
                {
                    for(int j=0;j<(m==0?vertiLineArray[m] : vertiLineArray[m]-vertiLineArray[m-1]);j++)
                        printf(" ");
                    printf("|");
                    vertiLineArray[m]++;
                }
            }
            printf("\n");
            n++;
        }
    }
}

int findNum(int *term, int x, int n)
{
    for(int i=0;i<n;i++)
        if(term[i]==x)
            return i+1;
    return -1;
}

int digits(int x)
{
    int num=0;
    while(x!=0)
    {
        x/=10;
        num++;
    }
    return num;
}


int main(int argc, const char * argv[]) {
    BTree T;
    T=(BTree)malloc(sizeof(BNode));
    T=creatTree(T, 20);
//    printf("%d\n",deepth(T));
//    printf("%d\n",count(T,0));
//    preTraversal(T);
//    inTraversal(T);
    printf("\n");
    display(T);
    printf("\n");
    display2(T);
}


