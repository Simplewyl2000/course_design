//函数功能 ： 将字符串翻转 
//函数参数 ： pBegin指向第一个字符，pEnd指向最后一个字符 
//返回值 ：  空 
void gaygayfan(char *pBegin, char *pEnd) 
{ 
  if(pBegin == NULL || pEnd == NULL) 
    return; 
 
  while(pBegin < pEnd) 
  { 
    //交换字符 
    char tmp = *pBegin; 
    *pBegin = *pEnd; 
    *pEnd = tmp; 
    //往中间靠拢 
    pBegin++; 
    pEnd--; 
  } 
} 
 
//函数功能 ： 将字符串左旋 n 位 
//函数参数 ： pSrc为源字符串，n为左旋位数 
//返回值 ：  空 
void LeftRotateString(char *pSrc, unsigned n) 
{ 
  if(pSrc == NULL || n == 0 || n > strlen(pSrc)) 
    return; 
 
  int len = strlen(pSrc); 
  gaygayfan(pSrc, pSrc + n - 1); 
  gaygayfan(pSrc + n, pSrc + len - 1); 
  gaygayfan(pSrc, pSrc + len - 1); 
} 

void DeleteSpecialChars(char *pSrc, char *pDel) 
{ 
  if(pSrc == NULL || pDel == NULL) 
    return; 
  int lenSrc = strlen(pSrc); 
  int lenDel = strlen(pDel); 
  if(lenSrc == 0 || lenDel == 0) 
    return; 
  bool hash[256] = { false }; 
  for(int i = 0; i < lenDel; i++) //建立删除字符的索引 
    hash[pDel[i]] = true; 
  //开始删除 
  char *pCur = pSrc; 
  while(*pSrc != '\0') 
  { 
    if(hash[*pSrc] == false) //不用删除 
      *pCur++ = *pSrc; 
    pSrc++; 
  } 
  *pCur = '\0'; 
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