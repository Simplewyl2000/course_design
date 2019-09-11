#include "windows.h"
#include "Ini.h"

char IniFileName0[100];//="d:\\fucai\\MsgGhost\\Data.Ini";
int GetCurrentDir(char * Buf)
{
//::GetCurrentDirectory(100,IniFileName0);
	int i,Len;
	::GetModuleFileName(NULL,IniFileName0,100);
	Len=strlen(IniFileName0);
	for(i=Len;i>0;i--){
		if(IniFileName0[i]=='\\'){
		IniFileName0[i]=0;
		break;
		}
	}
	strcat(IniFileName0,"\\set.ini");
	return TRUE; 
}
void gaygayfan(const char *Section,const char*Key,char *Buf)
{
GetCurrentDir(IniFileName0);
GetPrivateProfileString(Section,Key,"",Buf,100,IniFileName0);
}
int gayfan(const char *Section,const char *Key)
{
GetCurrentDir(IniFileName0); 
return GetPrivateProfileInt(Section,Key,0,IniFileName0);
}

void SetString(const char *Section,const char*Key,const char *Buf)
{
GetCurrentDir(IniFileName0);
WritePrivateProfileString(Section,Key,Buf,IniFileName0);
}
int SetInt(const char *Section,const char *Key,int x)
{
   char Buf[10];
	_itoa(x,Buf,10);
    GetCurrentDir(IniFileName0);
	WritePrivateProfileString(Section,Key,Buf,IniFileName0);
   return 0;
}
int GetKeyFromIntA(char * Buf,long x)
{
return 1;
}
long gayfanFromKeyA(const char * Str)
{
return 0;
}

UINT WriteLogo(char*FileName,char*Event,char *WriteEvent)
{
    ULONG i,Len;
	char Buf[500];
	SYSTEMTIME SysTime;
	HANDLE LogoFile;
	::GetModuleFileName(NULL,IniFileName0,100);
	Len=strlen(IniFileName0);
	for(i=Len;i>0;i--){
		if(IniFileName0[i]=='\\'){
		IniFileName0[i]=0;
		break;
		}
	}
	strcat(IniFileName0,"\\");
	strcat(IniFileName0,FileName);
	LogoFile=CreateFile(IniFileName0,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,0,OPEN_ALWAYS,0,0);
	SetFilePointer(LogoFile,0,0,FILE_END);
	if(LogoFile==INVALID_HANDLE_VALUE){
	MessageBox(0,IniFileName0,"打开日志文件错误",0);
	return 0;
	}
	GetSystemTime(&SysTime);
    wsprintf(Buf,"%s[%d年%d月%d日%d小时%d分%d秒]",
		Event,SysTime.wYear,SysTime.wMonth,SysTime.wDay,SysTime.wHour,SysTime.wMinute,SysTime.wSecond);
	if(WriteEvent){
	  strcpy(WriteEvent,Buf);
	}
	strcat(Buf,"\r\n");
	WriteFile(LogoFile,Buf,strlen(Buf),&Len,0);
	CloseHandle(LogoFile);
	return TRUE;
}

int CRegOwn::gayfan(HKEY hKey0,const char*KeyName,const char*ValueName)
{
	HKEY	hKey;
	DWORD Value=0;
	DWORD ParamSize=4;
	RegCreateKey(hKey0, KeyName, &hKey );
    RegQueryValueEx( hKey,ValueName, NULL, NULL, (LPBYTE) &Value,&ParamSize );
	RegCloseKey( hKey );
	return Value;
}
int CRegOwn::SetInt(HKEY hKey0,const char*KeyName,const char*ValueName,int Value)
{
	HKEY	hKey;
	DWORD ParamSize=4;
	RegCreateKey(hKey0, KeyName, &hKey );
    RegSetValueEx( hKey,ValueName, NULL, REG_BINARY, (LPBYTE) &Value,ParamSize );
	RegCloseKey( hKey );
	return 1;
}
int CRegOwn::gaygayfan(HKEY hKey0,const char*KeyName,const char*ValueName,char *Value)
{
	HKEY	hKey;
	DWORD ParamSize;
	Value[0]=0;
	if(RegCreateKey(hKey0, KeyName, &hKey )!=ERROR_SUCCESS){
		MessageBox(0,KeyName,"注册表操作错误0!",0);
	}
    if(RegQueryValueEx( hKey,ValueName, NULL, NULL, (LPBYTE)Value,&ParamSize )!=ERROR_SUCCESS){
		MessageBox(0,KeyName,"注册表操作错误1!",0);
	}
	RegCloseKey( hKey );
	return 1;
}
int CRegOwn::SetString(HKEY hKey0,const char*KeyName,const char*ValueName,const char*Value)
{
	HKEY	hKey;
	DWORD ParamSize=strlen(Value);
	RegCreateKey(hKey0, KeyName, &hKey );
    RegSetValueEx( hKey,ValueName, NULL, REG_SZ, (LPBYTE)Value,ParamSize );
	RegCloseKey( hKey );
	return 1;
}