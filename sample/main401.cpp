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
status InitBiTree(bitree T);
status CreateBiTree(bitree &T,telemtype (* gaygayfan)());
telemtype gaygayfan();
status PreOrderTraverse(bitree T,status (* visit)(telemtype));
status visit(telemtype);//直接打印
status visit2(bitree T,int j);//判断节点编号是否为j 
telemtype value(bitree T,int j);

/*--------------------------------------------*/
int main(void){
  bitree T[MAX_NUM_OF_TREES];
  int op=1;
  int num_of_trees=0;
  telemtype e;
  int i;
  int j;
  static int a;
  a=0;
  
  
  while(op){
	system("cls");	printf("\n\n");
	printf("      Menu for BiTree On Sequence Structure\n");
	printf("-------------------------------------------------\n");
	printf("    	  1. initbitree         7. value\n");
	printf("    	  2. createbitree       8. PriorElem\n");
	printf("    	  3. preorder traverse  9. NextElem \n");
	printf("    	  4. ListEmpty       10. ListInsert\n");
	printf("    	  5. ListLength      11. ListDelete\n");
	printf("    	  6. GetElem         12. ListTrabverse\n");
	printf("    	  13. Write          14. Read\n");
	printf("    	  0. Exit\n");
	printf("-------------------------------------------------\n");
	printf("    请选择你的操作[0~14]:");
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
	   	if(num_of_trees < MAX_NUM_OF_TREES && CreateBiTree(T[num_of_trees],gaygayfan) == OK){
		 	printf("创建%d号二叉树成功！\n",num_of_trees);
		 	num_of_trees++;
	   	 } 
	   	 else
		 	printf("创建二叉树失败！\n");     
		 getchar();getchar();
		 break;
	   case 3:
	   	 printf("请输入需要先序遍历的二叉树编号：");
	   	 scanf("%d",&i);
		 if(i>=0&&i<num_of_trees && PreOrderTraverse(T[i],visit)==OK)
		 	printf("\n%d号二叉树先序遍历完毕！\n",i);
         else
		 	printf("\n%d号二叉树先序遍历失败！\n",i); 
		 getchar();getchar();
		 break;
	  /* case 4:
		 if(ListEmpty(L)==TRUE) printf("线性表为空！\n");
		     else printf("线性表不为空！\n");     
		 getchar();getchar();
		 break;
	   case 5:
		 printf("线性表长度为%d！\n",ListLength(L));     
		 getchar();getchar();
		 break;
	   case 6:
	   	 printf("查找元素功能:请输入查找位置：\n");
		 scanf("%d",&i);
	   	 if(GetElem(L,i,e)==OK)
		 	printf("第%d个元素为：%d！\n",i,e);  
		 else printf("查找失败！");
		 getchar();getchar();
		 break;*/
	     case 7:
		 printf("请输入需要获得节点的二叉树编号：");
	   	 scanf("%d",&i);
	   	 printf("请输入需要获得的节点e：");
	   	 scanf("%d",&j);
		 if(i>=0&&i<num_of_trees){
		 		printf("\n%d号二叉树先序遍历完毕！\n",i);
		 }
		 
         else
		 	printf("\n%d号二叉树先序遍历失败！\n",i); 
		 getchar();getchar();
		 break;
	   /*case 8:
		 printf("PriorElem前驱查找功能：请输入元素：\n");
		 scanf("%d",&e);
		 int pre_e;
	     i=PriorElem(L,e,pre_e);
	     if(i!=ERROR)
		 	printf("PriorElem前驱查找功能执行成功！前一个元素：%d\n",pre_e);  
		 else printf("PriorElem前驱查找功能执行失败！");
		 getchar();getchar();
		 break;
	   case 9:
		 printf("NextElem后继查找功能：请输入元素：\n");
		 scanf("%d",&e);
		 int next_e;
	     i=NextElem(L,e,next_e);
	     if(i!=ERROR)
		 	printf("NextElem后继查找功能执行成功！后一个元素：%d\n",next_e);  
		 else printf("NextElem后继查找功能执行失败！");  
		 getchar();getchar();
		 break;
	   case 10:
		 printf("插入元素功能:请依次输入插入位置和插入元素，空格间隔：\n");
		 scanf("%d %d",&i,&e);
		 if(ListInsert(L,i,e)==OK) 
		 	printf("\n元素插入成功！\n");
		 else printf("\n元素插入失败！\n");
		 getchar();getchar();
		 break;
	   case 11:
		 printf("ListDelete功能:请输入要删除的元素位数：\n");
		 scanf("%d",&i);
		 if(ListDelete(L,i,e)==OK) 
		 	printf("\n第%d个元素删除成功！\n",i);
		 else printf("\n元素删除失败！\n");
		 getchar();getchar();
		 break;
	   case 12:   
		 if(!ListTrabverse(L)) printf("线性表是空表！\n");
		 getchar();getchar();
		 break;*/
	   case 0:
         break;
	}//end of switch
  }//end of while
  printf("欢迎下次再使用本系统！\n");
}//end of main()
/*--------page 23 on textbook --------------------*/

status InitBiTree(bitree T){
	if(!(T=(bitnode *)malloc(sizeof(bitnode)))) exit(OVERFLOW);
	T->lchild=NULL;
	T->rchild=NULL;
	return OK;
}

status CreateBiTree(bitree &T,telemtype (* gaygayfan)()){
	telemtype i;
	static int a;
	i=gaygayfan();
	if(i=='/') T=NULL;
	else{
		if(!(T=(bitnode *)malloc(sizeof(bitnode)))) exit(OVERFLOW); 
		T->data=i;
		T->num=a++;
		CreateBiTree(T->lchild,gaygayfan);
		CreateBiTree(T->rchild,gaygayfan);
	}
	return OK;
}

telemtype gaygayfan(){
	telemtype i;
	printf("请输入节点元素值，以/为空节点：");
	getchar();
	scanf("%c",&i);
	return i;
}

status PreOrderTraverse(bitree T,status (* visit)(telemtype)){
	if(T){
		if(visit(T->data))
			if(PreOrderTraverse(T->lchild,visit))
				if(PreOrderTraverse(T->rchild,visit))
					return OK;
		return ERROR;
	}
	else return OK;
}

status visit(telemtype i){
	printf("%c ",i);
	return OK;
}

status visit2(bitree T,int j){
	if(T->num==j)
		printf("%c ",T->data);
	return OK;
} 
/*
telemtype value(bitree T,int j){
	PreOrderTraverse(T,visit2(T,j));
}*/

