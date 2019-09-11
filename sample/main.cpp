/* Linear Table On Sequence Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2

typedef int status; 
typedef char telemtype; 

/*-------------*/
#define MAX_NUM_OF_TREES 5//本程序最多可以管理的树数 
#define MAX_NUM_OF_NODES 10//本程序每棵树最多节点数 
#define N -1//节点初始值 

typedef struct bitnode {
	int num;
	telemtype data;
	struct bitnode *lchild,*rchild;
}bitnode,*bitree;
/*--------------*/

status InitBiTree(bitree &T);
status CreateBiTree(bitree &T,telemtype (* definition)());
status DestroyBiTree(bitree &T);
status ClearBiTree(bitree &T);
telemtype definition();
status PreOrderTraverse(bitree T,status (* visit)(bitree T,int j));
status InOrderTraverse(bitree T,status (* visit)(bitree T,int j));
status PostOrderTraverse(bitree T,status (* visit)(bitree T,int j));
status LevelOrderTraverse(bitree T,status (* visit)(bitree T,int j));
status Value0(bitree T,int j,status (* visit)(bitree T,int j));
telemtype Value(bitree T,bitnode* e);
status visit(bitree T,int j);//多功能遍历：j若为-1，则全部遍历；若为正整数，则只输出编号num为j的节点 
status BiTreeEmpty(bitree T);
int BiTreeDepth(bitree T);
status Assign(bitree T,int j,telemtype value);
status InsertChild(bitree T,int p,int LR,bitree c);
status DeleteChild(bitree T,int p,int LR);
bitnode *Parent(bitree T,int e);
bitnode *LeftChild(bitree T,int e);
bitnode *RightChild(bitree T,int e);
bitnode *LeftSibling(bitree T,int e);
bitnode *RightSibling(bitree T,int e);

#define MaxSize 100
//栈 
typedef bitnode* SElemType;       /* 栈和队列的结点类型，用于存放树结点 */
typedef struct {
  SElemType elem[MaxSize];
  int top;
}SqStack;       /* 栈 */
void InitStack(SqStack *pS);      /* 初始化栈,开始时栈为空 */
int Push(SqStack *pS,SElemType e);     /* 进栈 */
int Pop(SqStack *pS,SElemType *pe);   /* 出栈 */

//队列 
typedef struct {
  SElemType elem[MaxSize];
  int front,rear;
}SqQueue;      /* 队列 */
void InitQueue(SqQueue* pQ); /* 初始化队 */
int EnQueue(SqQueue* pQ,SElemType e);/* 入队 */
int DeQueue(SqQueue* pQ,SElemType* pe);      /* 出队 */


/*--------------------------------------------*/
int main(void){
  bitree T[MAX_NUM_OF_TREES];
  int op=1;
  int num_of_trees=0;
  bitnode* e;
  int i;
  int j;
  static int a;
  a=0;
  
  
  while(op){
	system("cls");	printf("\n\n");
	printf("      Menu for BiTree On Sequence Structure\n");
	printf("-------------------------------------------------\n");
	printf("    	  1. InitBiTree         7. Value\n");
	printf("    	  2. DestroyBiTree      8. root\n");
	printf("    	  3. CreateBiTree       9. Assign\n");
	printf("    	  4. ClearBiTree        10. Parent\n");
	printf("    	  5. BiTreeEmpty        11. LeftChild\n");
	printf("    	  6. BiTreeDepth        12. RightChild\n");
	printf("    	  13. LeftSibling       14. RightSibling \n");
	printf("    	  15. InsertChild       16. DeleteChild \n");
	printf("    	  17. PreOrderTraverse  18. InOrderTraverse \n");
	printf("          19. PostOrderTraverse 20. LevelOrderTraverse\n");
	printf("    	  0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("    请选择你的操作[0~20]:");
	scanf("%d",&op);
    switch(op){
	   case 1:
		 if(num_of_trees < MAX_NUM_OF_TREES && InitBiTree(T[num_of_trees]) == OK){
		 	printf("初始化%d号二叉树成功！\n",num_of_trees);
		 	num_of_trees++;
		 }
         else
		 	printf("初始化二叉树失败！\n");
		 getchar();getchar();
		 break;
	case 2:
	   	 printf("请输入需要删除的二叉树编号：");
	   	 scanf("%d",&i); 
	   	 if(i>=0&&i<num_of_trees){
		 	if(DestroyBiTree(T[i])==OK)
				printf("\n%d号二叉树已经删除！\n",i);
			else
				 printf("\n%d号二叉树删除失败！\n",i);
		 }
         else
		 	printf("\n%d号二叉树未找到！\n",i,j); 
		 getchar();getchar();
		 break;
	case 3:
	   	if(num_of_trees < MAX_NUM_OF_TREES && CreateBiTree(T[num_of_trees],definition) == OK){
		 	printf("创建%d号二叉树成功！\n",num_of_trees);
		 	num_of_trees++;
	   	 } 
	   	 else
		 	printf("创建二叉树失败！\n");     
		 getchar();getchar();
		 break;
	case 4:
		 printf("请输入需要清空的二叉树编号：");
	   	 scanf("%d",&i); 
	   	 if(i>=0&&i<num_of_trees){
		 	if(ClearBiTree(T[i])==OK)
				printf("\n%d号二叉树已经清空！\n",i);
			else
				 printf("\n%d号二叉树不为空！\n",i);
		 }
         else
		 	printf("\n%d号二叉树清空失败！\n",i,j); 
		 getchar();getchar();
		 break;
	case 5:
		 printf("请输入需要判空的二叉树编号：");
	   	 scanf("%d",&i);
		 if(i>=0&&i<num_of_trees){
		 	if(BiTreeEmpty(T[i])==TRUE)
				printf("\n%d号二叉树为空！\n",i);
			else
				 printf("\n%d号二叉树不为空！\n",i);
		 }
         else
		 	printf("\n%d号二叉树判空失败！\n",i,j); 
		 getchar();getchar();
		 break;
	case 6:
	   	 printf("请输入需要查找深度的二叉树编号：");
	   	 scanf("%d",&i);
		 if(i>=0&&i<num_of_trees)
		 	printf("\n%d号二叉树深度：%d\n",i,BiTreeDepth(T[i]));
         else
		 	printf("\n%d号二叉树深度查找失败！\n",i); 
		 getchar();getchar();
		 break;
	case 7:
		 printf("请输入需要获得节点的二叉树编号：");
	   	 scanf("%d",&i);
	   	 printf("请输入需要获得的节点e编号num：");
	   	 scanf("%d",&j);
		 if(i>=0&&i<num_of_trees){
		 		printf("\n%d号二叉树编号num为%d的节点的data值为:",i,j);
		 		Value0(T[i],j,visit);
		 		printf("\n");
		 }
         else
		 	printf("\n%d号二叉树编号num为%d的节点的data值获取失败！\n",i,j); 
		 getchar();getchar();
		 break;
	case 8:
		 printf("请输入需要获得根节点的二叉树编号：");
	   	 scanf("%d",&i);
	   	 j=0; 
		 if(i>=0&&i<num_of_trees){
		 		printf("\n%d号二叉树的根节点的data值为:",i);
		 		Value0(T[i],j,visit);
		 		printf("\n");
		 }
         else
		 	printf("\n%d号二叉树的根节点的data值获取失败！\n",i); 
		 getchar();getchar();
		 break;
	case 9:
	   	 telemtype value; 
		 printf("请输入需要赋值的二叉树编号：");
	   	 scanf("%d",&i);
	   	 printf("请输入需要赋值的节点e编号num：");
	   	 scanf("%d",&j); 
	   	 getchar();
	   	 printf("请输入需要赋的值value：");
	   	 scanf("%c",&value); 
		 if(i>=0&&i<num_of_trees){
		 	    Assign(T[i],j,value);
		 		printf("赋值成功！");
		 		printf("\n");
		 }
         else
		 	printf("\n赋值失败！\n"); 
		 getchar();getchar();
		 break;
	case 10:
		 printf("请输入二叉树编号：");
	   	 scanf("%d",&i);
	   	 printf("请输入需要获得父节点的节点e编号num：");
	   	 scanf("%d",&j); 
		 if(i>=0&&i<num_of_trees){
		 		printf("\n%d号二叉树编号num为%d的父节点的data值为:",i,j);
		 		printf("%c",Parent(T[i],j)->data);
		 		printf("\n");
		 }
         else
		 	printf("\n%d号二叉树编号num为%d的父节点的data值获取失败！\n",i,j); 
		 getchar();getchar();
		 break;
		 
	case 11:
		 printf("请输入二叉树编号：");
	   	 scanf("%d",&i);
	   	 printf("请输入需要获得左孩子的节点e编号num：");
	   	 scanf("%d",&j); 
		 if(i>=0&&i<num_of_trees){
		 		printf("\n%d号二叉树编号num为%d的左孩子的data值为:",i,j);
		 		printf("%c",LeftChild(T[i],j)->data);
		 		printf("\n");
		 }
         else
		 	printf("\n%d号二叉树编号num为%d的左孩子的data值获取失败！\n",i,j); 
		 getchar();getchar();
		 break;
	case 12:   
		 printf("请输入二叉树编号：");
	   	 scanf("%d",&i);
	   	 printf("请输入需要获得右孩子的节点e编号num：");
	   	 scanf("%d",&j); 
		 if(i>=0&&i<num_of_trees){
		 		printf("\n%d号二叉树编号num为%d的右孩子的data值为:",i,j);
		 		printf("%c",RightChild(T[i],j)->data);
		 		printf("\n");
		 }
         else
		 	printf("\n%d号二叉树编号num为%d的右孩子的data值获取失败！\n",i,j); 
		 getchar();getchar();
		 break;
		 
	case 13:   
		 printf("请输入二叉树编号：");
	   	 scanf("%d",&i);
	   	 printf("请输入需要获得左兄弟的节点e编号num：");
	   	 scanf("%d",&j); 
		 if(i>=0&&i<num_of_trees){
		 		printf("\n%d号二叉树编号num为%d的左兄弟的data值为:",i,j);
		 		printf("%c",LeftSibling(T[i],j)->data);
		 		printf("\n");
		 }
         else
		 	printf("\n%d号二叉树编号num为%d的左兄弟的data值获取失败！\n",i,j); 
		 getchar();getchar();
		 break;
		 
	case 14:   
		 printf("请输入二叉树编号：");
	   	 scanf("%d",&i);
	   	 printf("请输入需要获得右兄弟的节点e编号num：");
	   	 scanf("%d",&j); 
		 if(i>=0&&i<num_of_trees){
		 		printf("\n%d号二叉树编号num为%d的右兄弟的data值为:",i,j);
		 		printf("%c",RightSibling(T[i],j)->data);
		 		printf("\n");
		 }
         else
		 	printf("\n%d号二叉树编号num为%d的右兄弟的data值获取失败！\n",i,j); 
		 getchar();getchar();
		 break;
	case 15:
	   	int k; 
	   	int l;
	   	printf("请输入被插入的二叉树编号：");
	   	scanf("%d",&i);
	   	printf("请输入需要插入的节点e编号num：");
	   	 scanf("%d",&j);
	   	printf("插入到左侧（0）还是右侧（1）？");
	   	 scanf("%d",&l);
	   	printf("请输入插入的二叉树编号：");
	   	scanf("%d",&k);
	   	if(i>=0&&i<num_of_trees && k>=0&& k<num_of_trees && InsertChild(T[i],j,l,T[k])==OK)
		 	printf("\n%d号二叉树被插入完毕！\n",i);
         else
		 	printf("\n%d号二叉树被插入失败！\n",i); 
		 getchar();getchar();
		 break;
	case 16:
	   	int k2; 
	   	int l2;
	   	printf("请输入被删除子树的二叉树编号：");
	   	scanf("%d",&i);
	   	printf("请输入需要删除的节点e编号num：");
	   	 scanf("%d",&j);
	   	printf("删除左侧（0）还是右侧（1）？");
	   	 scanf("%d",&l2);
	   	if(i>=0&&i<num_of_trees && k>=0&& k<num_of_trees && DeleteChild(T[i],j,l2)==OK)
		 	printf("\n%d号二叉树被删除完毕！\n",i);
         else
		 	printf("\n%d号二叉树被删除失败！\n",i); 
		 getchar();getchar();
		 break;
	case 17:
	   	 printf("请输入需要先序遍历的二叉树编号：");
	   	 scanf("%d",&i);
		 if(i>=0&&i<num_of_trees && PreOrderTraverse(T[i],visit)==OK)
		 	printf("\n%d号二叉树先序遍历完毕！\n",i);
         else
		 	printf("\n%d号二叉树先序遍历失败！\n",i); 
		 getchar();getchar();
		 break;
	case 18:
	   	 printf("请输入需要中序遍历的二叉树编号：");
	   	 scanf("%d",&i);
		 if(i>=0&&i<num_of_trees && InOrderTraverse(T[i],visit)==OK)
		 	printf("\n%d号二叉树中序遍历完毕！\n",i);
         else
		 	printf("\n%d号二叉树中序遍历失败！\n",i); 
		 getchar();getchar();
		 break;
	case 19:
	   	 printf("请输入需要后序遍历的二叉树编号：");
	   	 scanf("%d",&i);
		 if(i>=0&&i<num_of_trees && PostOrderTraverse(T[i],visit)==OK)
		 	printf("\n%d号二叉树后序遍历完毕！\n",i);
         else
		 	printf("\n%d号二叉树后序遍历失败！\n",i); 
		 getchar();getchar();
		 break;
	case 20:
	   	 printf("请输入需要按层遍历的二叉树编号：");
	   	 scanf("%d",&i);
		 if(i>=0&&i<num_of_trees && LevelOrderTraverse(T[i],visit)==OK)
		 	printf("\n%d号二叉树按层遍历完毕！\n",i);
         else
		 	printf("\n%d号二叉树按层遍历失败！\n",i); 
		 getchar();getchar();
		 break;
	   case 0:
         break;
	}//end of switch
  }//end of while
  printf("欢迎下次再使用本系统！\n");
}//end of main()

status InitBiTree(bitree &T){

    
    T=(bitree)malloc(sizeof(bitnode));//创建根节点
    T->lchild=NULL;
    T->rchild=NULL;
    return OK;
}

status DestroyBiTree(bitree &T)
{
    if (ClearBiTree(T->lchild)==OK)
    {
        free(T);
        T=NULL;//将二叉树清空后释放根结点
    }
    return OK;
}

status ClearBiTree(bitree &T)
{
    if (!T)
        return OK;
    else
    {
        ClearBiTree(T->lchild);
        ClearBiTree(T->rchild);
        free(T);//清空根结点，使用递归算法依次清空左右子树
        T = NULL;
    }
    return OK;
}

status CreateBiTree(bitree &T,telemtype (* definition)()){
	telemtype i;
	static int a;
	i=definition();
	if(i=='/') T=NULL;
	else{
		if(!(T=(bitnode *)malloc(sizeof(bitnode)))) exit(OVERFLOW); 
		T->data=i;
		T->num=a++;
		CreateBiTree(T->lchild,definition);
		CreateBiTree(T->rchild,definition);
	}
	return OK;
}

telemtype definition(){
	telemtype i;
	printf("请输入节点元素值，以/为空节点：");
	getchar();
	scanf("%c",&i);
	return i;
}

int BiTreeDepth(bitree T)//若T为空，则返回0；否则比较T的左子树和右子树深度的大小（即递归调用该算法求解），选取其中大的值，自增1后返回。
{
   if (T==NULL) return 0;
   else {
   	if(BiTreeDepth(T->lchild)>BiTreeDepth(T->lchild))
       return BiTreeDepth(T->lchild)+1;
       else return BiTreeDepth(T->lchild)+1;
     }
}


status PreOrderTraverse(bitree T,status (* visit)(bitree T,int j)){
	if(T){
		if(visit(T,-1))
			if(PreOrderTraverse(T->lchild,visit))
				if(PreOrderTraverse(T->rchild,visit))
					return OK;
		return ERROR;
	}
	else return OK;
}

status InOrderTraverse(bitree T,status (* visit)(bitree T,int j)){
	if(T){
		if(InOrderTraverse(T->lchild,visit))
			if(visit(T,-1))
				if(InOrderTraverse(T->rchild,visit))
					return OK;
		return ERROR;
	}
	else return OK;
}

status PostOrderTraverse(bitree T,status (* visit)(bitree T,int j)){
	if(T){
		if(PostOrderTraverse(T->lchild,visit))
			if(PostOrderTraverse(T->rchild,visit))
				if(visit(T,-1))
					return OK;
		return ERROR;
	}
	else return OK;
}

status LevelOrderTraverse(bitree T,status (* visit)(bitree T,int j)){/*采用非递归算法，利用队列解决问题*/
  SqQueue  q;
  bitree p;
  p=T;
  InitQueue(&q);
  EnQueue(&q,p);
  while(!(q.rear==q.front)) { /* 当队列不空 */
    DeQueue(&q,&p);
    visit(p,-1);

    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return OK; 
}

status Value0(bitree T,int j,status (* visit)(bitree T,int j)){/*先序遍历*/ 
	if(T){
		if(visit(T,j))
			if(Value0(T->lchild,j,visit))
				if(Value0(T->rchild,j,visit))
					return OK;
		return ERROR;
	}
	else return OK;
}

status visit(bitree T,int j){
	if(j==-1){/* j为-1直接打印 */ 
		printf("%c ",T->data);
	} 
	else if(T->num==j){/*j不为-1只打印num==j的结点data值 */ 
		printf("%c ",T->data);
	}
	return OK;
}

status BiTreeEmpty(bitree T)
{
    if((T->lchild==NULL)&&(T->rchild==NULL))
        return TRUE;
    return FALSE;
}

status Assign(bitree T,int e,telemtype value){
	SqQueue  q;
	bitree p;
  p=T;
  InitQueue(&q);
  EnQueue(&q,p);
  while(!(q.rear==q.front)) { /* 当队列不空 */
    DeQueue(&q,&p);
    if(p->num==e)
        p->data=value; 
	
    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return OK; 
}

bitnode *Parent(bitree T,int e){
	SqQueue  q;
	bitree p;
  p=T;
  InitQueue(&q);
  EnQueue(&q,p);
  e=e/2;/* 双亲结点的num值是孩子结点的一半取下整 */
  while(!(q.rear==q.front)) { 
    DeQueue(&q,&p);
    if(p->num==e)
    	return p;
	
    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return NULL; 
}

bitnode *LeftChild(bitree T,int e){
	SqQueue  q;
	bitree p;
  p=T;
  InitQueue(&q);
  EnQueue(&q,p);
  while(!(q.rear==q.front)) { 
    DeQueue(&q,&p);
    if(p->num==e)
    	return p->lchild;
	
    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return NULL; 
}

bitnode *RightChild(bitree T,int e){
	SqQueue  q;
	bitree p;
  p=T;
  InitQueue(&q);
  EnQueue(&q,p);
  while(!(q.rear==q.front)) { 
    DeQueue(&q,&p);
    if(p->num==e)
    	return p->rchild;
	
    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return NULL; 
}

bitnode *LeftSibling(bitree T,int e){
	SqQueue  q;
	bitree p;
  p=T;
  InitQueue(&q);
  EnQueue(&q,p);
  e=e/2;/* 先找他们共同的双亲 */
  while(!(q.rear==q.front)) { 
    DeQueue(&q,&p);
    if(p->num==e)
    	return p->lchild;
	
    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return NULL; 
}

bitnode *RightSibling(bitree T,int e){
	SqQueue  q;
	bitree p;
  p=T;
  InitQueue(&q);
  EnQueue(&q,p);
  e=e/2;
  while(!(q.rear==q.front)) { 
    DeQueue(&q,&p);
    if(p->num==e)
    	return p->rchild;
	
    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return NULL; 
}

status InsertChild(bitree T,int e,int LR,bitree c){
	SqQueue q;
	bitree p;
	p=T;
	InitQueue(&q);
	EnQueue(&q,p);
	while(!(q.rear==q.front)) { 
	DeQueue(&q,&p);
    if(p->num==e){
    	if(LR==0)
    	{
    		c->rchild=p->lchild;
    		p->lchild=c;
    	}
    	else
    	{
    		c->rchild=p->rchild;
    		p->rchild=c;
    	}
    	
    }
	
    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return OK;
}

status DeleteChild(bitree T,int e,int LR){
	SqQueue q;
	bitree p;
	p=T;
	InitQueue(&q);
	EnQueue(&q,p);
	while(!(q.rear==q.front)) { 
	DeQueue(&q,&p);
    if(p->num==e){
    	if(LR==0)
    	{
    		free(p->lchild);
    		p->lchild=NULL;
    	}
    	else
    	{
    		free(p->rchild);
    		p->rchild=NULL;
    	}
    	
    }
	
    if(p->lchild!=NULL)
      EnQueue(&q,p->lchild);

    if(p->rchild!=NULL)
       EnQueue(&q,p->rchild);
  }
  return OK;	
}

void InitStack(SqStack *pS)         /* 初始化栈,开始时栈为空 */
{
  pS->top=0; /* top指向栈顶的上一个元素 */
}

int Push(SqStack *pS,SElemType e)     /* 进栈 */
{
  if (pS->top==MaxSize-1)   /* 栈满 */
    return 0;

  pS->elem[pS->top]=e;
  pS->top=pS->top+1;
  return 1;
}

int Pop(SqStack *pS,SElemType *pe)    /* 出栈 */
{
  if (pS->top==0)  /* 栈空 */
    return 0;

  pS->top = pS->top - 1;
  *pe = pS->elem[pS->top];
  return 1;
}

/*队列（循环队列）的定义及基本操作*/

void InitQueue(SqQueue* pQ)   /* 初始化队列,开始时队列为空 */
{
  pQ->front=pQ->rear=0;
}

int EnQueue(SqQueue* pQ,SElemType e)       /* 进队 */
{
  if ((pQ->rear+1)%MaxSize == pQ->front) /* 队满 */
    return 0;
  pQ->elem[pQ->rear] = e;
  pQ->rear = (pQ->rear+1)%MaxSize;
  return 1;
}

int DeQueue(SqQueue* pQ,SElemType* pe)      /* 出队 */
{
  if (pQ->rear == pQ->front)    /* 队空 */
    return 0;
  *pe = pQ->elem[pQ->front];
  pQ->front = (pQ->front+1)%MaxSize;
  return 1;
}
