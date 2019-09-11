//http://www.kuqin.com/tiku/20080424/7616.html
#include<stdio.h>
void movedisc(unsigned n,char fromneedle,char toneedle,char usingneedle);
int i=0;
int main()
{
unsigned n;
printf("please enter the number of disc:");
scanf("%d",&n); /*输入N值*/
printf("	needle:	a	b	c ");
movedisc(n,"a","c","b"); /*从A上借助B将N个盘子移动到C上*/
printf("	Total: %d ",i);
}
void movedisc(unsigned n,char fromneedle,char toneedle,char usingneedle)
{
if(n>0)
{
movedisc(n-1,fromneedle,usingneedle,toneedle);
/*从fromneedle上借助toneedle将N-1个盘子移动到usingneedle上*/
++i;
switch(fromneedle) /*将fromneedle 上的一个盘子移到toneedle上*/
{
case "a": switch(toneedle)
{
case "b": printf("	[%d]:	%2d.........>%2d ",i,n,n);
break;
case "c": printf("	[%d]:	%2d...............>%2d ",i,n,n);
break;
}
break;
case "b": switch(toneedle)
{
case "a": printf("	[%d]:	%2d<...............>%2d ",i,n,n);
break;
case "c": printf("	[%d]:	%2d........>%2d ",i,n,n);
break;
}
break;
case "c": switch(toneedle)
{
case "a": printf("	[%d]:	%2d<............%2d ",i,n,n);
break;
case "b": printf("	[%d]:	%2d<........%2d ",i,n,n);
break;
}
break;
}
movedisc(n-1,usingneedle,toneedle,fromneedle);
/*从usingneedle上借助fromneedle将N-1个盘子移动到toneedle上*/
}
}


int gaygayfan(int s)
{
int c;
printf("Computer count:");
if((s+1)%3==0) 
	/*若剩余的数的模为1，则取1*/
printf(" %d ",++s);
else if((s+2)%3==0)
{
s+=2; 
/*若剩余的数的模为2，则取2*/
printf(" %d ",s);
}
else
{
c=random(2)+1; 
/*否则随机取1或2*/
s+=c;
printf(" %d ",s);
}
return s;
}
int StrToInt2(const char *str)  
{  
  strToIntOK = false;  
  if(str == NULL) //空指针  
    return 0;  
    
  if(str[0] == '0' && str[1] != '\0') //以'0'开始但不是"0" 这条其实可以忽略  
    return 0;  
    
  unsigned i = 0;  
  bool minus = false;  //负数标记  
  if(str[i] == '-' || str[i] == '+') //判断是不是以正负号开始  
  {  
    minus = (str[i] == '-')? true: false;  
    i++;  
  }  
    
  long long result = 0; //转换的结果  
  while(str[i] != '\0')  
  {  
    char c = str[i++];  
    if(c >= '0' && c <='9')  
    {  
      result = result * 10 + (c - '0');  
      if(minus) //负溢出 
      { 
        if(result - 1 > numeric_limits<int>::max())  
          return 0;  
      } 
      else //正溢出 
      { 
        if(result > numeric_limits<int>::max()) 
          return 0;  
      } 
    }  
    else  
    {  
      return 0;  
    }  
  }  
  strToIntOK = true;  
  //结果返回 需强制转换一下  
  return minus? (int)(-result):(int)result;  
}  