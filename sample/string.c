    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/***********************************************
*函数名称： fun
*创建时间： 2010.12.5
*描    述： 将s所指字符串中最后一次出现的与t1所指字符串相同的子串替换为t2所指字符串
*参    数： char * s,char * t1, char * t2,int *m
*返 回 值： w(数组w[]的首元素地址)
*局部变量： char w[30];
*           char temp[2];
*           char t1temp[2];
*           int i,j,k,l;
************************************************/
char * fun(char * s,char * t1, char * t2,int *m)    //m的作用和第一题一样，没m最后两个字符会乱码（想寻求其他方法）      
{
    char w[30];              //用来存放处理后的字符串
    char temp[2];       //用来存放从s所指字符串中截取的子串
    char t1temp[2];     //用来存放t1所指字符串
    int i,j,k,l;
    //向t1temp中存入t1所指字符串
    for (i=0; i<2;i++)
    {
        t1temp[i]=t1[i];
    }
    t1temp[2]='';
     
    //求m所指的值
    for (i=0;i<30;i++)
    {
        if (s[i]=='')
        {
            *m=i;
        }
    }
 
    //寻找相同时的最后一个字符的下标
    for (i=0; i<30; i++)
    {
        l=0;
        for (j=i; j<(i+2); j++,l++)  //截取长度为T的子串存到temp[]中
        {
            temp[l]=s[j];
        }
        temp[2]='';
 
        if (strcmp(t1temp,temp) == 0)
        {
            k=i;        //k记录相同时的最后一个字符的下标
        }
    }
 
    for (i=0; i<30; i++)  //赋值给w[]
    {
        j=0;
        if (i>=k && i<(k+2))      //在找到的k点处开始换值
        {
            w[i]=t2[j];             //换值
            j++;
        }
        else
        {
            w[i]=s[i];
        }
    }
     
    return w;
}
 
//主函数
void main()
{
    char s[30];
    char t1[2];
    char t2[2];
    int i;
    int m;
    char *p;
 
    p=NULL;
    printf("请输入字符串(20字符以内):");
    scanf("%s",s);
    printf("请输入将要被替换的子字符串(仅限两个字符):");
    scanf("%s",t1);
    printf("请输入将要用来替换的字字符串(仅限两个字符):");
    scanf("%s",t2);
    p=fun(s,t1,t2,&m);                 
    for (i=0; i<m; i++)              //将返回的值复制给数组以待输出
    {
        s[i]=p[i];
    }
     
    printf("结果为：");
    for (i=0; i<m; i++)              //输出结果
    {
        printf("%c",s[i]);
    }
    printf("30");
}
