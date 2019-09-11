#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
   
#define N 30  
/*********************************************** 
*函数名称： fun 
*创建时间： 2010.12.6 
*描    述： 将s所指的字符串中ASCII值为奇数的字符删除，串中剩余字符形成一个新串放在t所指的数组中。 
*参    数： char * s,int * m 
*返 回 值： return t,(t为指向数组的指针) 
*局部变量： char * t; 
*           char temp[N]; 
*           int i,j=0; 
************************************************/  
   
char * fun(char * s,int * m)  
{  
    char * t;  
    char temp[N];                   //临时的数组，用来存放t所指的数组  
    int i,j=0;  
       
    t=temp;  
    for (i=0; i<N; i++)  
    {  
        if (s[i]%2==0 )  
        {  
            temp[j]=s[i];           //如果是偶数，赋值给t所指的数组  
            j++;     
            if (s[i]=='')         //找到m的值先要找出输入了多少个字符  
            {  
                break;  
            }  
        }  
    }  
    *m=j;                           //找到m的值即输出结果的长度  
    t[j]='';  
    return t;  
}  
   
//主函数  
void main()  
{  
    char s[N];  
    int i;  
    int m;   
    char *p;  
   
    p=NULL;  
    printf("请输入字符串(30字符以内):n");  
    scanf("%s",s);  
    p=fun(s,&m);                    //刚开始没定义出这个m来现限制指针p所指数组的长度就出现了后面两个字符乱码  
    for (i=0; i<m; i++)              //将返回的值复制给数组以待输出  
    {  
        s[i]=p[i];  
    }  
       
    printf("结果为：");  
    for (i=0; i<m; i++)              //输出结果  
    {  
        printf("%c",s[i]);  
    }  
    printf("n");  
}  