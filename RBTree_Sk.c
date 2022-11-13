#include "RBTree_Sk.h"

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MAX_DIGITS 4
#define MAX_NODE 100
#define MAX_TMP 10000  //MAX_NODE^2


RBNode *root = NULL;
int g_TraversalNum=0;  //用于存储中序遍历的index


int colorOf(RBNode* node)
{
    return node == NULL ? RB_BLACK : node->color;
}

RBNode* parentOf(RBNode* node)
{
    return node != NULL ? node->parent : NULL;
}

RBNode* leftOf(RBNode *node)
{
    return node != NULL ? node->lchild : NULL;
}

RBNode* rightOf(RBNode *node)
{
    return node != NULL ? node->rchild : NULL;
}

void setColor(RBNode *node, int color)
{
    if(node != NULL)
    {
        node->color = color;
    }
}


RBNode* getNode(ElemType key)
{
    RBNode* node = root;

    while (node != NULL)
    {
        int cmp = key - node->key;
        if(cmp < 0)
        {
            node = node->lchild;
        }
        else if(cmp > 0)
        {
            node = node->rchild;
        }
        else
        {
            return node;
        }
        
    }

    return NULL;

}



/**
 * 围绕p左旋
 *       pf                    pf
 *      /                     /
 *     p                     pr(r)
 *    / \          ==>      / \
 *  pl  pr(r)              p   rr
 *     / \                / \
 *    rl  rr             pl  rl
 *
 * @param p
 */
void leftRotate(RBNode *p)
{
    
    if(p != NULL)
    {
        RBNode *r = p->rchild;

        if(r == NULL)
            return;

        p->rchild = r->lchild;

        if(r->lchild != NULL)
        {
            r->lchild->parent = p;
        }

        r->parent = p->parent;
        
        if(p->parent == NULL)
        {
            root = r;
        }
        else if (p->parent->lchild == p)
        {
            p->parent->lchild = r;
        }
        else if (p->parent->rchild == p)
        {
            p->parent->rchild = r;
        }
        
        r->lchild = p;
        p->parent = r;
    }

    return;
}



/**
 * 右旋
 *    pf                pf
 *     \                 \
 *      p             (l)pl
 *     / \      =>      /  \
 *(l)pl  pr            ll   p
 *   / \                   / \
 *  ll lr                 lr  pr
 *
 * @param p
 */
void rightRotate(RBNode *p)
{
    if(p != NULL)
    {
        RBNode *l = p->lchild;

        if(l == NULL)
            return;

        p->lchild = l->rchild;
        if(l->rchild != NULL)
        {
            l->rchild->parent =p;
        }

        l->parent = p->parent;
        if(p->parent == NULL)
        {
            root = l;
        }
        else if (p->parent->rchild == p)
        {
            p->parent->rchild = l;
        }
        else if (p->parent->lchild == p)
        {
            p->parent->lchild = l;
        }
        
        l->rchild = p;
        p->parent = l;
        
    }

    return;
}


/**
 * 找到指定节点的前驱节点，即找小于node节点的最大值
 * @param node
 */
RBNode *predecessor(RBNode *node)
{
    if(node == NULL)
    {
        return NULL;
    }
    else if (node->lchild != NULL)
    {
        RBNode *p = node->lchild;
        while (p->rchild != NULL)
        {
            p = p->rchild;
        }
        return p;
    }
    else
    {
        RBNode *p = node->parent;
        RBNode *ch = node;
        
        //应该缺少一个判断: 也就是node不能是最小的节点

        while (p != NULL && ch == p->lchild)
        {
            ch = p;
            p = p->parent;
        }
        
        return p;
    }

    return NULL;
}


/**
 * 找后继节点，即大于节点的最小值
 * @param node
 * @return
 */
RBNode *successor(RBNode *node)
{
    if(node == NULL)
    {
        return NULL;
    }
    else if (node->rchild != NULL)
    {
        RBNode *p = node->rchild;
        while(p->lchild != NULL)
        {
            p = p->lchild;
        }
        return p;
    }
    else
    {
        RBNode *p = node->parent;
        RBNode *ch = node;

        //应该缺少一个判断: 也就是node不能是最大的节点

        while(p != NULL && ch == p->rchild)
        {
            ch = p;
            p = p->parent;
        }

        return p;
    }
    
    return NULL;
}



//=========================红黑树的增加=======================================


//插入一个节点
void put(ElemType key, ElemValue value)
{
    RBNode *t = root;

    //如果是根节点  根节点一定是黑色
    if(t == NULL)
    {
        root = (RBNode*)malloc(sizeof(RBNode));
        root->key = key;
        root->value = value;
        root->color = RB_BLACK;
        root->parent = root->lchild = root->rchild = NULL;
        return ;
    }

    int cmp;

    //寻找插入位置
    //定义一个双亲指针
    RBNode *parent = NULL;
    
    //沿着跟节点寻找插入位置
    do
    {
        parent = t;

        if(key < t->key)
        {
            t = t->lchild;
        }
        else if (key > t->key)
        {
            t = t->rchild;
        }
        else
        {
            t->value = value;
            return;
        }

    } while (t!=NULL);
    
    //插入的节点一定是个红色节点
    RBNode *e = (RBNode*)malloc(sizeof(RBNode));
    e->key = key;
    e->value = value;
    e->color = RB_RED;
    e->parent = parent;
    e->lchild = e->rchild = NULL;

    // printf("e->key  %d\n", e->key);
    // printf("e->parent->key  %d\n", e->parent->key);
    
    if(e->key < parent->key)
    {
        //如果比较最终落在左子树，则直接将父节点左指针指向e
        parent->lchild = e;
    }
    else
    {
        //如果比较最终落在右子树，则直接将父节点右指针指向e
        parent->rchild = e;
    }
    
    //调整
    fixAfterPut(e);

}

/**
 * 1、2-3-4树：新增元素+2节点合并（节点中只有1个元素）=3节点（节点中有2个元素）
 *    红黑树：新增一个红色节点+黑色父亲节点=上黑下红（2节点）--------------------不要调整
 *
 * 2、2-3-4树：新增元素+3节点合并（节点中有2个元素）=4节点（节点中有3个元素）
 *    这里有4种小情况（左3，右3，还有2个左中右不需要调整）------左3，右3需要调整，其余2个不需要调整
 *    红黑树：新增红色节点+上黑下红=排序后中间节点是黑色，两边节点都是红色（3节点）
 *
 * 3、2-3-4树：新增一个元素+4节点合并=原来的4节点分裂，中间元素升级为父节点，新增元素与剩下的其中一个合并
 *    红黑树：新增红色节点+爷爷节点黑，父节点和叔叔节点都是红色=爷爷节点变红，父亲和叔叔变黑，如果爷爷是根节点，则再变黑
 *
 *
 * @param x
 */
void fixAfterPut(RBNode* x) 
{
    RBNode *y = NULL;

    //本质上就是父节点是黑色就不需要调整，对应情况就是2,3
    while(x != NULL && x != root && x->parent->color == RB_RED)
    {
        //1, x的父节点是爷爷节点的左孩子
        if(parentOf(x) == leftOf(parentOf(parentOf(x))))
        {
            //叔叔节点
            y = rightOf(parentOf(parentOf(x)));

            //第三种情况
            if(colorOf(y) == RB_RED)
            {
                setColor(parentOf(x), RB_BLACK);
                setColor(y, RB_BLACK);
                setColor(parentOf(parentOf(x)), RB_RED);

                //爷爷节点递归
                x = parentOf(parentOf(x));
            }
            else
            {
                //第二种情况
                if(x == rightOf(parentOf(x)))
                {
                    x = parentOf(x);
                    leftRotate(x);
                }
                //父亲变黑
                setColor(parentOf(x), RB_BLACK);
                //爷爷变红
                setColor(parentOf(parentOf(x)), RB_RED);
                //根据爷爷节点右旋转
                rightRotate(parentOf(parentOf(x)));
            }
        }
        else
        {
            //2, 跟第1种相反

            //叔叔节点
            y = leftOf(parentOf(parentOf(x)));

            //第三种情况
            if(colorOf(y)== RB_RED)
            {
                setColor(parentOf(x), RB_BLACK);
                setColor(y, RB_BLACK);
                setColor(parentOf(parentOf(x)), RB_RED);
                //爷爷节点递归
                x=parentOf(parentOf(x));
            }
            else
            {
                //第二种情况
                if(x == leftOf(parentOf(x)))
                {
                    x = parentOf(x);
                    rightRotate(x);
                }

                // printf("x->key %d\n", x->key);
                // printf("parentOf(x)->key %d\n", parentOf(x)->key);
                // printf("parentOf(parentOf(x))->key %d\n", parentOf(parentOf(x))->key);
                
                //父亲变黑
                setColor(parentOf(x), RB_BLACK);
                //爷爷变红
                setColor(parentOf(parentOf(x)), RB_RED);
                //根据爷爷节点左旋转
                leftRotate(parentOf(parentOf(x)));
            }
        }

    }

    root->color=RB_BLACK;

}





//=========================红黑树的删除=======================================

/**
 * 删除后调整
 * @param x
 */
void fixAfterRemove(RBNode* x)
{
    RBNode* rnode = NULL;

    //以下这个段while循环的核心是:
    //x是个黑色节点
    //向兄弟节点再借来一个黑色节点
    //如果是情况2——自己搞不定(这种情况, 替代节点一定是黑色的), 需要找兄弟借, 但是兄弟不借, 找父亲借,
    //那么第一次循环就结束了, 最后替代节点(黑色)会删掉, 保证了黑色平衡
    //如果是情况3——兄弟搞不定, 同时自损的情况的话
    //由于兄弟也是个黑色节点, 同时自损的话, 兄弟变成红色, 不平衡了, 少了个黑色, 
    //通过循环, 即x向兄弟借来一个黑色节点, 不断借黑色, 这样就可以保证整棵树平衡
    while (x != NULL && colorOf(x) == RB_BLACK)
    {
        //x是左孩子的情况
        if(x == leftOf(parentOf(x)))
        {
            //兄弟节点
            rnode = rightOf(parentOf(x));

            //判断此时兄弟节点是否是真正的兄弟节点
            if(colorOf(rnode) == RB_RED)
            {
                setColor(rnode, RB_BLACK);
                setColor(parentOf(x), RB_RED);
                leftRotate(parentOf(x));
                //找到真正的兄弟节点
                rnode = rightOf(parentOf(x));
            }

            //情况三，找兄弟借，兄弟没得借
            if(colorOf(leftOf(rnode))==RB_BLACK && colorOf(rightOf(rnode)) == RB_BLACK)
            {
                setColor(rnode, RB_RED);
                x = parentOf(x);
            }
            else
            {
                //情况二，找兄弟借，兄弟有的借
                //分2种小情况：兄弟节点本来是3节点或者是4节点的情况
                //也可以应该是借一个节点还是借两个节点这两种情况
                //对于4节点, 肯定是借两个节点情况, 一个左旋就可以了
                //对于3节点, 可以是借1个(左旋就可以了), 也可以借2个(右旋  再左旋)

                
                if(colorOf(rightOf(rnode)) == RB_BLACK)
                {
                    setColor(leftOf(rnode), RB_BLACK);
                    setColor(rnode, RB_RED);
                    rightRotate(rnode);
                    rnode = rightOf(parentOf(x));
                }
                
                
                setColor(rnode, colorOf(parentOf(x)));
                setColor(parentOf(x), RB_BLACK);
                setColor(rightOf(rnode), RB_BLACK);
                leftRotate(parentOf(x));
                x = root;
            }
            
        }
        else
        {
            //x是右孩子的情况
            //兄弟节点
            rnode = leftOf(parentOf(x));
            //判断此时兄弟节点是否是真正的兄弟节点
            if(colorOf(rnode) == RB_RED){
                setColor(rnode, RB_BLACK);
                setColor(parentOf(x),RB_RED);
                rightRotate(parentOf(x));
                //找到真正的兄弟节点
                rnode=leftOf(parentOf(x));
            }
            //情况三，找兄弟借，兄弟没得借
            if(colorOf(rightOf(rnode)) == RB_BLACK&& colorOf(leftOf(rnode)) == RB_BLACK){

                setColor(rnode,RB_RED);
                x=parentOf(x);
            }
            //情况二，找兄弟借，兄弟有的借
            else{
                //分2种小情况：兄弟节点本来是3节点或者是4节点的情况
                if(colorOf(leftOf(rnode)) == RB_BLACK)
                {
                    setColor(rightOf(rnode),RB_BLACK);
                    setColor(rnode,RB_RED);
                    leftRotate(rnode);
                    rnode=leftOf(parentOf(x));
                }
                setColor(rnode,colorOf(parentOf(x)));
                setColor(parentOf(x), RB_BLACK);
                setColor(leftOf(rnode),RB_BLACK);
                rightRotate(parentOf(x));
                x=root;
            }
        }
    }
   

   //情况1: 替代节点是红色，则直接染红，补偿删除的黑色节点，这样红黑树依然保持平衡
   setColor(x, RB_BLACK);

}







/**
 * 删除操作：
 * 删除节点不是真的直接去删除这个节点
 * 而是要去找替代节点, 用替代节点覆盖要删除的节点
 * 然后把这个替代节点给删掉
 * 
 * 
 * 也就是说, 对于删除一个红黑树的一个节点:
 * 1、删除叶子节点，直接删除
 * 2、删除的节点有一个子节点，那么用子节点来替代 
 * 3、如果删除的节点有2个子节点，此时需要找到前驱节点或者后继节点来替代
 * 
 * 以上三种类型, 最后都会转变成删除一个红黑树的叶子节点或者是删除叶子节点上面一层的一个节点:
 *   对于类型1, 就是删除它自己
 *   对于类型2, 这个节点肯定是叶子节点的上一层的一个节点, 肯定转变成用叶子节点替换它自己
 *   对于类型3, 找到的替代节点肯定是叶子节点或者是叶子节点上一层
 * 
 * 
 * 论证方式:
 * 红黑树等价于2-3-4树
 * 删除2-3-4树的一个节点
 * 找替代节点的时候, 因为2-3-4树是个满树, 最后的替代节点肯定找的要么是它自己, 要么就是2-3-4树的一个叶子节点
 * 2-3-4树的一个叶子节点就是红黑树的叶子节点或者是叶子节点上一层的一个节点
 * 
 * 
 * 
 * 删除红黑树的叶子节点或者是删除叶子节点上面一层的一个节点, 最终会转换成以下三种情况:
 * 
 * 情况1: 自己可以搞定——替代节点是红色的, 这种情况对应替代节点是2-3-4树的3节点或者是4节点
 * 
 * 情况2: 自己搞不定(这种情况, 替代节点一定是黑色的), 需要找兄弟借, 但是兄弟不借, 找父亲借, 
 *        然后父亲下来, 兄弟找一个去代替父亲
 *        这种情况, 分成两类: 兄弟节点对应的是2-3-4树的3节点
 *                           兄弟节点对应的是2-3-4树的4节点
 * 
 * 情况3: 自己搞不定(这种情况, 替代节点一定是黑色的), 需要找兄弟借, 兄弟也没有, 兄弟情同手足, 同时自损
 *                          兄弟节点对应的是2-3-4树的2节点
 * 
 * 
 *
 * @param node
 */
void deleteNode(RBNode* node)
{
    // 3, node节点有2个孩子
    if(node->lchild != NULL && node->rchild != NULL)
    {
        /**
         *  这里要注意，如果使用下面这个网站演示的话，此网站用的是前驱节点替代
         *  下面代码里我使用的是后继节点替代，删除节点后显示可能会和该网站不一致，
         *  但是这两种方法红黑树删除都是合法的
         *  （可以自行把前驱节点替代方案屏蔽放开，后继节点替代方案注释掉测试下）
         *
         *  https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
         */
        
        //后继节点替代
        RBNode* rep = successor(node);
        //前驱节点替代
        // RBNode* rep = predecessor(node);

        node->key = rep->key;
        node->value = rep->value;
        node = rep;
    }

    RBNode* replacement = node->lchild != NULL ? node->lchild : node->rchild;

    //2, 替代节点不能为空
    if(replacement != NULL)
    {
        //替代者的父指针指向原来node的父亲
        replacement->parent = node->parent;
        
        //node是根节点
        if(node->parent == NULL)
        {
            root = replacement;
        }
        else if (node == node->parent->lchild)
        {
            //node是左孩子, 所以替代者依然是左孩子
            node->parent->lchild = replacement;
        }
        else
        {
            //node是右孩子，所以替代者依然是右孩子
            node->parent->rchild = replacement;
        }
        
        //将node的左右孩子指针和父指针都指向null
        node->lchild = node->rchild = node->parent = NULL;
        
        //替换完之后需要调整平衡
        if(node->color == RB_BLACK)
        {
            //需要调整,这种情况一定是红色（替代节点一定是红色，此时只要变色）
            fixAfterRemove(replacement);
        }

        free(node);
        node = NULL;

    }
    else if(node->parent == NULL)
    {
        //删除的节点就是根节点
        root =  NULL;
    }
    else
    {
        //1, node节点是叶子节点, replacement为NULL
        //先调整, 再删除,  因为直接删除后, 就没有一个依据的节点进行调整了

        //先调整
        if(node->color == RB_BLACK)
        {
            fixAfterRemove(node);
        }

        //再删除
        if(node->parent != NULL)
        {
            if(node == node->parent->lchild)
            {
                node->parent->lchild = NULL;
            }
            else if(node == node->parent->rchild)
            {
                node->parent->rchild = NULL;
            }

            node->parent = node->lchild = node->rchild = NULL;
            free(node);
            node = NULL;
        }

    }

}



ElemValue RB_Remove(ElemType key)
{
    RBNode* node = getNode(key);

    if(node == NULL)
    {
        return RB_WRONGW_VALUE;
    }

    ElemValue oldValue = node->value;
    deleteNode(node);

    return oldValue;
}



//=========================有关展示的函数=======================================
int deepth(BTree root)
{
    if (root == NULL)
    {
        return 0;
    }

    int left=deepth(root->lchild);
    int right=deepth(root->rchild);
    return left>=right ? left+1:right+1;
}


void saveInTraversal(BTree root,int *term)
{
    if(root == NULL)
        return;
    else
    {
        saveInTraversal(root->lchild,term);
        term[g_TraversalNum++]=root->key;
        saveInTraversal(root->rchild,term);
    }
}


int findNum(int *term, int x, int n)
{
    for(int i=0;i<n;i++)
        if(term[i]==x)
            return i+1;
    return -1;
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


void Display(BTree root)
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
    
    // printf("deep %d\n", deep);


    g_TraversalNum = 0;
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
                    printf("%d(%s)",term[i]->key, term[i]->color == RB_BLACK ? "B":"R");
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

    free(N);
    N = NULL;
}





int main()
{
    printf("RBTree Sk \n");

    int i;


    //增加节点
    for(i=1; i<=10; i++)
    {
        put(i, i);
        
    }

    Display(root);


    //删除节点
    RB_Remove(6);
    
    Display(root);

    return 0;
}


