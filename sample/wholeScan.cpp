// wholeScan.cpp : implementation file
//
#include "stdafx.h"
#include "VirScan.h"
#include "wholeScan.h"
#include <io.h>
#include <time.h>
#include <direct.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char *LogPath1="G:\\课 件\\毕业设计\\VirScan\\Debug\\VirusList.log";
char *ScanPath1; 
char CurrentDir1[MAX_PATH];
char ScanInfo1[100];                   //="扫描完毕! 共扫描M个文件, 发现N个病毒!";
unsigned long NumofFile1,NumofVirus1;  //需扫描的文件个数M以及扫描结果中的病毒个数N
unsigned long TotalFiles1;

char *FileSpecs[]={{"*.*"}};
HANDLE hMem1;	         //句柄：指向内存空间的一个指针
HWND ListWnd1, PrintWnd1, StartWnd1, StopWnd1;   //窗体句柄
CProgressCtrl *lpCtrlPr1; //进度条指针

char Drives[26][5];      //保存硬盘盘符信息 GetLogicalDriveStrings
int n=0;				  //记录硬盘的个数

HANDLE hThread1;          //扫描线程相关参数
DWORD dwThreadId1;
bool HaltThread1=false;
bool StopThread1=false;


void FindFile1(char *FileSpec[], int NumFileSpec);  //采用深度遍历算法进行目录遍历
void CountNum1(char *FileSpec[], int NumFileSpec);  //初始化计算扫描目录内的文件个数
UINT ThreadScan1(LPVOID lpVoid);  //扫描线程
int VirusScan1(struct _finddata_t *FindStruct);       //根据字符串匹配结果判断该文件是否有毒文件
void WriteList1(char Name[], char Dir[], char *Virus);//将扫描出的病毒的详细信息写入List显示
void WriteLog1(char Name[], char Dir[], char *Virus); //将扫描出的病毒的详细信息写入File记录历史

unsigned char Taiwan1[]={0x8A, 0x0E, 0x95, 0x00, 0x81, 0xE1, 0xFE, 0x00, 0xBA, 0x9E};
unsigned char Icelandic1[]={0x43, 0x81, 0x3F, 0x45, 0x58, 0x75, 0xF1, 0xB8, 0x00, 0x43};
unsigned char AIDS1[]={0x42, 0xE8, 0xEF, 0xFF, 0x8E, 0xD8, 0x2D, 0xCC};
unsigned char Klez1[]={0xA1, 0x00, 0x00, 0x00, 0x00, 0x50, 0x64, 0x89, 0x25, 0x00, 0x00, 0x00, 0x00, 0x83, 0xEC, 0x58, 0x53, 0x56, 0x57, 0x89};
unsigned char CIH1[]={0x55, 0x8D, 0x44, 0x24, 0xF8, 0x33, 0xDB, 0x64, 0x87, 0x03};

/////////////////////////////////////////////////////////////////////////////
// wholeScan dialog
wholeScan::wholeScan(CWnd* pParent /*=NULL*/)
	: CDialog(wholeScan::IDD, pParent)
{
	//{{AFX_DATA_INIT(wholeScan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void wholeScan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(wholeScan)
	DDX_Control(pDX, IDC_SCANPRO, m_file);
	DDX_Control(pDX, IDC_STOP, m_stop);
	DDX_Control(pDX, IDC_SCAN, m_scan);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP

	//设置List控件：当选中List控件中某行信息时此行将高亮显示
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	CRect rt;
	m_list.GetClientRect(&rt);
	//设置列
	m_list.InsertColumn(0,"文件名",LVCFMT_LEFT,rt.right/5);
	m_list.InsertColumn(1,"路径",LVCFMT_LEFT,3*rt.right/5);
	m_list.InsertColumn(2,"病毒名",LVCFMT_LEFT,rt.right/5);
	GetDlgItem (IDC_STOP)->EnableWindow(FALSE);   //初始化时设置停止扫描不可点击
	
	ListWnd1 = m_list.m_hWnd;
	PrintWnd1 = m_file.m_hWnd; 
	StartWnd1 = m_scan.m_hWnd;
	StopWnd1 = m_stop.m_hWnd; //MFC中的类都有这个变量m_hWnd,指向的是自己的句柄
	lpCtrlPr1 = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);   //进度条指针初始化

	//获取硬盘盘符，dwDrives格式为011111100"
	DWORD dwDrives = GetLogicalDrives(); 
	char volume='A'; 
	int i;

	while (dwDrives > 0) { 
		if(dwDrives%2==1){  //分享出单个盘符并保存在Drives数组中
			wsprintf(Drives[n],"%c%c%c%c",volume,':','\\','\\');
			n++;
		} 
		volume++; 
		dwDrives /= 2; 
	} 

	for(i=0;i<n;i++){        
		ScanPath1=Drives[i]; //strcpy(ScanPath1,Drives[i]);错误
		//printf("%s",ScanPath1);
		_chdir(ScanPath1);   //初始化计算扫描目录内的文件个数
		CountNum1(FileSpecs, sizeof(FileSpecs)/4);
	}
	//printf("%d",TotalFiles1);
	m_progress.SetRange(0,TotalFiles1); //设置进度条的滚动范围
	m_progress.SetPos(0);
	SetTimer(1,1000,NULL);   //设置进度条更新时钟
}

BEGIN_MESSAGE_MAP(wholeScan, CDialog)
	//{{AFX_MSG_MAP(wholeScan)
	ON_BN_CLICKED(IDC_SCAN, OnScan)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// wholeScan message handlers
void wholeScan::OnScan() 
{
	// TODO: Add your control notification handler code here
	char buf[9];
	GetDlgItem(IDC_SCAN)->GetWindowText(buf,9); //9 not 8
	if(!strcmp(buf,"开始扫描")||!strcmp(buf,"重新扫描")) {
		StopThread1=false;
		m_progress.SetPos(0);
		NumofFile1=NumofVirus1=0;
		GetDlgItem(IDC_STOP)->EnableWindow(TRUE); 
		GetDlgItem(IDC_SCAN)->SetWindowText("暂停扫描"); 		  
		m_list.DeleteAllItems();  //创建扫描线程
		hThread1=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadScan1,&m_file,0,&dwThreadId1);
		if(hThread1==NULL)
			AfxMessageBox("创建扫描线程失败!"); 
	}
	else if(!strcmp(buf,"暂停扫描")) {
		HaltThread1=true;
		GetDlgItem(IDC_STOP)->EnableWindow(TRUE);   
		GetDlgItem(IDC_SCAN)->SetWindowText("继续扫描"); 
		SuspendThread(hThread1);   //暂停扫描线程
		wsprintf(ScanInfo1,"扫描已暂停! 共扫描 %d 个文件, 发现 %d 个病毒!",NumofFile1,NumofVirus1);
		GetDlgItem(IDC_SCANPRO)->SetWindowText(ScanInfo1);
	}
	else if(!strcmp(buf,"继续扫描")) {
		HaltThread1=false;
		GetDlgItem(IDC_STOP)->EnableWindow(TRUE); 
		GetDlgItem(IDC_SCAN)->SetWindowText("暂停扫描");
		ResumeThread(hThread1);   //继续扫描线程
	}
}

void wholeScan::OnStop() 
{
	// TODO: Add your control notification handler code here
	StopThread1=true;
	m_progress.SetPos(0);
	GetDlgItem(IDC_STOP)->EnableWindow(FALSE);  
	GetDlgItem(IDC_SCAN)->SetWindowText("开始扫描"); 
	TerminateThread(hThread1,dwThreadId1);   //结束扫描线程
	wsprintf(ScanInfo1,"扫描已停止! 共扫描 %d 个文件, 发现 %d 个病毒!",NumofFile1,NumofVirus1);
	GetDlgItem(IDC_SCANPRO)->SetWindowText(ScanInfo1);
	NumofFile1=NumofVirus1=0;
}

void wholeScan::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}




//采用深度遍历算法进行目录遍历
void FindFile1(char *FileSpec[], int NumFileSpec)
{
	struct _finddata_t FindStruct; 	
	long hFind;                    
	int i;

	if(StopThread1) 
		return;

	for(i=0; i<NumFileSpec; i++){
		if((hFind = _findfirst(FileSpec[i], &FindStruct)) != -1){
			if(!(FindStruct.attrib & _A_SUBDIR)){
				NumofFile1++;
				NumofVirus1 += VirusScan1(&FindStruct);
				if(NumofFile1<=TotalFiles1)
					lpCtrlPr1->SetPos(NumofFile1);
			}
		}
		if(hFind != -1){
			while(_findnext(hFind, &FindStruct) == 0){
				if(!(FindStruct.attrib & _A_SUBDIR)){
					NumofFile1++;
					NumofVirus1 += VirusScan1(&FindStruct);
					if(NumofFile1<=TotalFiles1)
						lpCtrlPr1->SetPos(NumofFile1);
				}
				if(HaltThread1)
					break;
			}
			_findclose(hFind);
		}
	}
	if((hFind = _findfirst("*.*", &FindStruct)) == -1){
			_chdir("..");  //Now find the directories
			return;
	}
	do{
		if(((FindStruct.attrib & _A_SUBDIR) != 0) && (FindStruct.name[0] != '.')){
			if(!_chdir(FindStruct.name))
				FindFile1(FileSpec, NumFileSpec);
			if(HaltThread1)
				break;
		}
	}while(_findnext(hFind, &FindStruct) == 0);
	chdir("..");
	_findclose(hFind);
	return;
}


//初始化计算扫描目录内的文件个数
void CountNum1(char *FileSpec[], int NumFileSpec)
{
	struct _finddata_t FindStruct; 
	long hFind;
	int i;
	for(i=0; i<NumFileSpec; i++){
		if((hFind = _findfirst(FileSpec[i], &FindStruct)) != -1){
			if(!(FindStruct.attrib & _A_SUBDIR)){
				TotalFiles1++;
			}
		}
		if(hFind != -1){
			while(_findnext(hFind, &FindStruct) == 0){
				if(!(FindStruct.attrib & _A_SUBDIR)){
					TotalFiles1++;
				}
			}
			_findclose(hFind);
		}
	}
	if((hFind = _findfirst("*.*", &FindStruct)) == -1){
			_chdir("..");
			return;
	}
	do{
		if(((FindStruct.attrib & _A_SUBDIR) != 0) && (FindStruct.name[0] != '.')){
			if(!_chdir(FindStruct.name))
				CountNum1(FileSpec, NumFileSpec);
		}
	}while(_findnext(hFind, &FindStruct) == 0);
	chdir("..");
	_findclose(hFind);
	return;
}


//扫描线程
UINT ThreadScan1(LPVOID lpVoid)
{
	int i;
	for(i=0; i<=0; i++) { 
		ScanPath1=Drives[i];
		_chdir(ScanPath1); 	//依次进入某个盘符下的目录
		FindFile1(FileSpecs, sizeof(FileSpecs)/4); //递归检测目录内的所有文件
    }		
	//扫描完毕后的处理	
	wsprintf(ScanInfo1,"扫描完毕! 共扫描 %d 个文件, 发现 %d 个病毒!",NumofFile1,NumofVirus1);
	::SetWindowText(PrintWnd1,ScanInfo1); 
    ::SetWindowText(StartWnd1,"重新扫描");  
	::EnableWindow(StopWnd1, FALSE);
	return 0;
}



//根据字符串匹配结果判断该文件是否有毒文件
//PE文件结构： MZ文件头    --"MZ"
////          DOS插桩程序
////           PE 文件头   --IMAGE_NT_HEADERS STRUCT
////                       --   Signature dd                  字串"PE\0\0" (4字节)0x00004550H
////                       --   FileHeader IMAGE_FILE_HEADER  映像文件头 (14H字节)
////                       --   OptionalHeader IMAGE_OPTIONAL_HEADER32   可选映像头(OEOH字节)
////                       --   IMAGE_NT_HEADERS ENDS
////      Section table (节表) --IMAGE_SECTION_HEADER
////        Section n...... 
int VirusScan1(struct _finddata_t *FindStruct)
{
	HANDLE hFile;             //正在扫描的文件句柄
	DWORD ReadBytes; 
	int i=0;
	char *pMem;
	unsigned char pBytes[100];
	char TmpStr[MAX_PATH], PrintPath[MAX_PATH]; //打印正在扫描的文件路径
	
	PIMAGE_DOS_HEADER DosHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_FILE_HEADER FileHeader;
	PIMAGE_OPTIONAL_HEADER OptionalHeader;	
	PIMAGE_SECTION_HEADER Section;
	DWORD SkipDos, Hb, EntryPoint, pCodeBytes;

	ZeroMemory(PrintPath,MAX_PATH);
	_getcwd(CurrentDir1, 300);
	wsprintf(PrintPath, "%s%s\\%s", "正在扫描: ", CurrentDir1, FindStruct->name);
	::SetWindowText(PrintWnd1,PrintPath);   //输出目录路径

	if((hFile=CreateFile(FindStruct->name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_READONLY, NULL))==INVALID_HANDLE_VALUE){
		return 0;
	}
	ReadFile(hFile, TmpStr, sizeof(IMAGE_DOS_HEADER), &ReadBytes, NULL);//读取MS-DOS header
	if(ReadBytes == 0){     //TmpStr保存读入数据的缓冲区；ReadBytes指向实际读取字节数的指针
		CloseHandle(hFile);
		return 0;
	}
	DosHeader = (PIMAGE_DOS_HEADER)TmpStr;          //IMAGE_DOS_HEADER
	if((TmpStr[0] != 'M') && (TmpStr[1] != 'Z')){   
		CloseHandle(hFile);
		return 0;          //如果文件的前两个字节不是"MZ"，则说明非PE文件，退出
	}	

	SkipDos = DosHeader->e_lfanew;                  //用偏移位置e_lfanew来定位PE header
	SetFilePointer(hFile, SkipDos, 0, FILE_BEGIN);
	ReadFile(hFile, TmpStr, sizeof(IMAGE_NT_HEADERS), &ReadBytes, NULL);
	if(ReadBytes == 0){
		CloseHandle(hFile);
		return 0;
	}
	pNTHeader = (PIMAGE_NT_HEADERS)TmpStr;          //IMAGE_NT_HEADERS
	if(pNTHeader->Signature != 0x00004550){
		CloseHandle(hFile);
		return 0;          //如果在偏移位置处的四个字节不是"PE\0\0"，则说明非PE文件，退出
	}

	Hb = pNTHeader->OptionalHeader.SizeOfHeaders; //NT映像头+节表的大小，此值作为节表的起始位置
	hMem1 = (HANDLE)LocalAlloc(LMEM_MOVEABLE, Hb+100);//动态分配一块 Hb+100 大小的内存
	if(hMem1 == NULL){
		CloseHandle(hFile);
		return 0;
	}
	pMem = (PSTR) LocalLock(hMem1);    //对内存对象加锁，将其暂时固定在局部堆的某一位置
	SetFilePointer(hFile, SkipDos, 0, FILE_BEGIN);//重新定位至PE header读取Hb个字节保存到pMem缓冲区中
	ReadFile(hFile, pMem, Hb, &ReadBytes, NULL);     
	if(ReadBytes == 0){
		CloseHandle(hFile);
		LocalFree(hMem1);
		return 0;
	}
	pNTHeader = (PIMAGE_NT_HEADERS) pMem;
	//IMAGE_SECTION_HEADER
	Section = (PIMAGE_SECTION_HEADER)((DWORD) pNTHeader + 24 +(DWORD)(pNTHeader->FileHeader.SizeOfOptionalHeader)); 
	EntryPoint = pNTHeader->OptionalHeader.AddressOfEntryPoint; //程序开始执行位置RVA,一般病毒通过修改该值来指向自己的病毒体开始代码的

	//根据文件中节的个数循环
	for(i=0; i<pNTHeader->FileHeader.NumberOfSections; i++, Section++){ 
		if(Section->PointerToRawData == 0)  //本节原始数据在文件中的位置
			continue;
		if(((EntryPoint >= Section->VirtualAddress) && (EntryPoint <= (Section->VirtualAddress + Section->Misc.VirtualSize))) || (Section->Characteristics|0x20) ){
			//pCodeBytes病毒代码插入的位置
			pCodeBytes = EntryPoint - Section->VirtualAddress + Section->PointerToRawData; 
			break;
		}
	}

	//查找Taiwan1
	SetFilePointer(hFile, pCodeBytes, 0, FILE_BEGIN);
	ReadFile(hFile, pBytes, sizeof(Taiwan1), &ReadBytes, NULL);
	for(i=0; i<sizeof(Taiwan1); i++){
		if(Taiwan1[i]!=pBytes[i])
			break;
	}
	if(i==sizeof(Taiwan1)){
		WriteList1((char *)&FindStruct->name,CurrentDir1,"Taiwan");
		CloseHandle(hFile);
		LocalFree(hMem1);
		return 1;
	}

	//查找Icelandic1
	SetFilePointer(hFile, pCodeBytes, 0, FILE_BEGIN);
	ReadFile(hFile, pBytes, sizeof(Icelandic1), &ReadBytes, NULL);
	for(i=0; i<sizeof(Icelandic1); i++){
		if(Icelandic1[i]!=pBytes[i])
			break;
	}
	if(i==sizeof(Icelandic1)){
		WriteList1((char *)&FindStruct->name,CurrentDir1,"Icelandic");
		CloseHandle(hFile);
		LocalFree(hMem1);
		return 1;
	}

	//查找AIDS1
	SetFilePointer(hFile, pCodeBytes, 0, FILE_BEGIN);
	ReadFile(hFile, pBytes, sizeof(AIDS1), &ReadBytes, NULL);
	for(i=0; i<sizeof(AIDS1); i++){
		if(AIDS1[i]!=pBytes[i])
			break;
	}
	if(i==sizeof(AIDS1)){
		WriteList1((char *)&FindStruct->name,CurrentDir1,"AIDS");
		CloseHandle(hFile);
		LocalFree(hMem1);
		return 1;
	}

	//查找Klez1
	SetFilePointer(hFile, pCodeBytes+16, 0, FILE_BEGIN);
	ReadFile(hFile, pBytes, sizeof(Klez1), &ReadBytes, NULL);
	for(i=0; i<sizeof(Klez1); i++){
		if(Klez1[i]!=pBytes[i])
			break;
	}
	if(i==sizeof(Klez1)){
		WriteList1((char *)&FindStruct->name,CurrentDir1,"Klez");
		CloseHandle(hFile);
		LocalFree(hMem1);
		return 1;
	}
	
	//查找CIH1
	SetFilePointer(hFile, pCodeBytes, 0, FILE_BEGIN);
	ReadFile(hFile, pBytes, sizeof(CIH1), &ReadBytes, NULL);
	for(i=0; i<sizeof(CIH1); i++){
		if(CIH1[i]!=pBytes[i])
			break;
	}
	if(i==sizeof(CIH1)){
		WriteList1((char *)&FindStruct->name,CurrentDir1,"CIH");
		CloseHandle(hFile);
		LocalFree(hMem1);
		return 1;
	}

	CloseHandle(hFile);
	LocalFree(hMem1);
	return 0;
}


//将扫描出的病毒的详细信息写入List显示
void WriteList1(char Name[], char Dir[], char *Virus)
{
	LVITEM Lv;  //CListCtrl控件：LV_ITEM结构描述一个表项或一个子项，包含项的各种属性
	Lv.mask = LVIF_TEXT;
	Lv.pszText = Name;
	Lv.iItem = 0;
	Lv.iSubItem = 0;
	Lv.cchTextMax = strlen(Name);
	ListView_InsertItem(ListWnd1, &Lv);
	ListView_SetItemText(ListWnd1, 0, 1, Dir);
	ListView_SetItemText(ListWnd1, 0, 2, Virus);
	WriteLog1(Name,Dir,Virus); //写病毒检测日志文件
}


//将扫描出的病毒的详细信息写入File记录历史
void WriteLog1(char Name[], char Dir[], char *Virus)
{
	char pBuffer[200];
	char VirusPath[200];	

	CTime time=CTime::GetCurrentTime();  //得到系统当前时间并格式化输出  
	CString strTime=time.Format("%Y-%m-%d %H:%M:%S"); 
	strcpy(VirusPath,Dir);
	strcat(VirusPath,"\\");
	strcat(VirusPath,Name);
    wsprintf(pBuffer,"%s%s%s%s%s%s",Virus,"\t",VirusPath,"\t",strTime,"\n");//往缓冲区里写数据
	
	FILE *fp;
	fp=fopen(LogPath1,"a+");   //以添加的方式打开文件
	fprintf(fp,"%s",pBuffer); //重复扫描则可能重复添加Log，暂时不考虑这个BUG
	fclose(fp);
}
