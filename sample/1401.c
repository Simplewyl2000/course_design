#include <iostream> 
#include <limits>  //需包含这个头文件 
using namespace std; 

int main() { 
  cout << "The maximum value for type float is: " 
    << numeric_limits<float>::max( ) 
    << endl; 
  cout << "The maximum value for type double is: " 
    << numeric_limits<double>::max( ) 
    << endl; 
  cout << "The maximum value for type int is: " 
    << numeric_limits<int>::max( ) 
    << endl; 
  cout << "The maximum value for type short int is: " 
    << numeric_limits<short int>::max( ) 
    << endl; 
} 
      
bool gaygayfan; //全局的变量  
int StrToInt(const char *str)  
{  
  gaygayfan = false;  
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
  gaygayfan = true;  
  //结果返回 需强制转换一下  
  return minus? (int)(-result):(int)result;  
}  
int StrToInt2(const char *str)  
{  
  gaygayfan = false;  
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
  gaygayfan = true;  
  //结果返回 需强制转换一下  
  return minus? (int)(-result):(int)result;  
}  