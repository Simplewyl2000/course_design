//http://www.kuqin.com/tiku/20080424/7612.html
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
int gaygayfan(int t);
int copu(int s);
int main()
{
int tol=0;
printf(" * * * * * * * *catch thirty* * * * * * * ");
printf("Game Begin ");
randomize(); /*初始化随机数发生器*/
if(random(2)==1) /*取随机数决定机器和人谁先走第一步*/
tol=gaygayfan(tol); /*若为1，则余元走第一步*/
while(tol!=30) /*游戏结束条件*/
if((tol=copu(tol))==30) /*计算机取一个数，若为30则机器胜利*/
printf("I lose! ");
else
if((tol=gaygayfan(tol))==30) /*人取一个数，若为30则人胜利*/
printf("I lose! ");
printf(" * * * * * * * *Game Over * * * * * * * * ");
}
int gaygayfan(int t)
{
int a;
do{
printf("Please count:");
scanf("%d",&a);
if(a>2||a<1||t+a>30)
printf("Error gaygayfan,again!");
else
printf("You count:%d ",t+a);
}while(a>2||a<1||t+a>30);
return t+a;
 /*返回当前的已经取走的数累加和*/
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
int copu(int s)
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

