#include "stdio.h"
#include "stdlib.h"
#define LINES 8 // 定义行数  
#define COLS 8 // 定义列数  
void pro(){
	int a=4, b=7;
	char buf[] = 'demo';
	printf("%s %d d,buf=%s",buf,a,b);
}

void visit(int Q[LINES])
{
    int i=0;
    printf("the result of eight queen: \n");
    while(i<LINES)
        {
            printf("(%d, %d)",i,Q[i]);
            i++;
        }
}
int canplace(int Q[],int k)
{
    int i;
    for(i=0; i<k; i++)
        {
            //if( (Q[k]==Q[i]) || ((Q[k]-k)==(Q[i]-i)) || ((Q[k]+k)==(Q[i]+i)) )//两个位置任意一行、一列、正斜向、反斜向只要有一个条件满足，此位置不可放皇后 
            if( (Q[k]==Q[i]) || (abs(Q[k]-Q[i])==abs(k-i)) )//换一种判断，判断正斜向反斜向只要想到数学的直线斜率为正负一就可以简化 
                return 0;//表示第k个皇后不能放
        }
    return 1;
}
int  EightQueen(int Q[LINES])
{
    int k = 1,i,find,temp;
    Q[0] = 0;

    while(k>0)
        {
            find = 0;//find标志，表示
            for(i=0; i < COLS; i++)
                {
                	//Q[k] = i 会出现死循环调不出来。回溯时下一个皇后找不到合适的列，又给上一个皇后找到上一个第一个合适的列。 
                   Q[k]++ ;
				   if((Q[k]<COLS)&&canplace(Q,k))
                        {
                            find = 1;
                            break;
                        }
                }
            if(find == 1)//第k+1个皇后找到合适的列
                {
                    if(k==7)//八个皇后都安置正确
                        {
                            visit(Q);
                            return 1;
                        }
                    else//查找下一个皇后的位置
                        {
                            k++;
                        }
                }
            else // 第K+1个皇后找不到合适的列，即它放置的位置与前k个皇后冲突，那么应该重新放置第k个皇后；
                {
					Q[k] = -1; //重新把当前的皇后位置设为-1，表示没有寻找。 
                    k--;
                }
        }
    if(k == 0)
        {
            printf("Sorry!");
            return 0;
        }

}

void main()
{
    int Q[LINES],i;
    for(i=0; i<LINES; i++)
        Q[i]=-1;//把所有的皇后位置设为-1，表示没有寻找

    EightQueen(Q);
}