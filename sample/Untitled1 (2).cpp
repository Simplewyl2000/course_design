#include "stdafx.h"  
  
#include <windows.h>     
#include "NoteBook.h"  
#include <windows.h>  
#include <Commdlg.h>  
  
static char g_szClassName[] = "记事本";    
static HINSTANCE g_hInst = NULL;    
  
#define IDC_MAIN_TEXT   1001     
  
BOOL LoadFile(HWND hEdit, LPSTR pszFileName)    
{    
    HANDLE hFile;    
    BOOL bSuccess = FALSE;    
  
    hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,    
        OPEN_EXISTING, 0, 0);    
    if(hFile != INVALID_HANDLE_VALUE)    
    {    
        DWORD dwFileSize;    
        dwFileSize = GetFileSize(hFile, NULL);    
        if(dwFileSize != 0xFFFFFFFF)    
        {    
            LPSTR pszFileText;    
            pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);    
            if(pszFileText != NULL)    
            {    
                DWORD dwRead;    
                if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))    
                {    
                    pszFileText[dwFileSize] = 0;     
                    if(SetWindowText(hEdit, pszFileText))    
                        bSuccess = TRUE;     
                }    
                GlobalFree(pszFileText);    
            }    
        }    
        CloseHandle(hFile);    
    }    
    return bSuccess;    
}    
  
BOOL SaveFile(HWND hEdit, LPSTR pszFileName)    
{    
    HANDLE hFile;    
    BOOL bSuccess = FALSE;    
  
    hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, 0,    
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);    
    if(hFile != INVALID_HANDLE_VALUE)    
    {    
        DWORD dwTextLength;    
        dwTextLength = GetWindowTextLength(hEdit);    
        if(dwTextLength > 0)    
        {    
            LPSTR pszText;    
            pszText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);    
            if(pszText != NULL)    
            {    
                if(GetWindowText(hEdit, pszText, dwTextLength + 1))    
                {    
                    DWORD dwWritten;    
                    if(WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))    
                        bSuccess = TRUE;    
                }    
                GlobalFree(pszText);    
            }    
        }    
        CloseHandle(hFile);    
    }    
    return bSuccess;    
}    
  
BOOL DoFileOpenSave(HWND hwnd, BOOL bSave)    
{    
    OPENFILENAME ofn;    
    char szFileName[MAX_PATH];    
  
    ZeroMemory(&ofn, sizeof(ofn));    
    szFileName[0] = 0;    
  
    ofn.lStructSize = sizeof(ofn);    
    ofn.hwndOwner = hwnd;    
    ofn.lpstrFilter = "文本文件 (*.txt)\0*.txt\0所有文件 (*.*)\0*.*\0\0";    
    ofn.lpstrFile = szFileName;    
    ofn.nMaxFile = MAX_PATH;    
    ofn.lpstrDefExt = "txt";    
  
    if(bSave)    
    {    
        ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |    
            OFN_OVERWRITEPROMPT;    
  
        if(GetSaveFileName(&ofn))    
        {    
            if(!SaveFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName))    
            {    
                MessageBox(hwnd, "保存文件失败", "错误信息",    
                    MB_OK | MB_ICONEXCLAMATION);    
                return FALSE;    
            }    
        }    
    }    
    else    
    {    
        ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;    
        if(GetOpenFileName(&ofn))    
        {    
            if(!LoadFile(GetDlgItem(hwnd, IDC_MAIN_TEXT), szFileName))    
            {    
                MessageBox(hwnd, "打开文件失败", "错误信息",    
                    MB_OK | MB_ICONEXCLAMATION);    
                return FALSE;    
            }    
        }    
    }    
    return TRUE;    
}    
  
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)    
{    
    switch(Message)    
    {    
    case WM_CREATE:    
        CreateWindow("EDIT", "",    
            WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE |    
            ES_WANTRETURN,    
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,    
            hwnd, (HMENU)IDC_MAIN_TEXT, g_hInst, NULL);    
  
        SendDlgItemMessage(hwnd, IDC_MAIN_TEXT, WM_SETFONT,    
            (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(TRUE, 0));    
        break;    
    case WM_SIZE:    
        if(wParam != SIZE_MINIMIZED)    
            MoveWindow(GetDlgItem(hwnd, IDC_MAIN_TEXT), 0, 0, LOWORD(lParam),    
            HIWORD(lParam), TRUE);    
        break;    
    case WM_SETFOCUS:    
        SetFocus(GetDlgItem(hwnd, IDC_MAIN_TEXT));    
        break;    
    case WM_COMMAND:    
        switch(LOWORD(wParam))    
        {    
        case ID_FILE_OPEN:  //菜单中的文件打开  
            DoFileOpenSave(hwnd, FALSE);    
            break;    
        case ID_FILE_SAVEAS: //菜单中的文件另存为   
            DoFileOpenSave(hwnd, TRUE);    
            break;    
        case IDM_EXIT:  //菜单中的退出  
            PostMessage(hwnd, WM_CLOSE, 0, 0);    
            break;    
        case IDM_ABOUT: //菜单中的关于  
            MessageBox (NULL, "一个Windows C 记事本程序" , "关于...", 0);    
        }    
        break;    
    case WM_CLOSE:    
        DestroyWindow(hwnd);    
        break;    
    case WM_DESTROY:    
        PostQuitMessage(0);    
        break;    
    default:    
        return DefWindowProc(hwnd, Message, wParam, lParam);    
    }    
    return 0;    
}    
  
  
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,    
    LPSTR lpCmdLine, int nCmdShow)    
{    
    WNDCLASSEX WndClass;    
    HWND hwnd;    
    MSG Msg;    
  
    g_hInst = hInstance;    
  
    WndClass.cbSize        = sizeof(WNDCLASSEX);    
    WndClass.style         = 0;    
    WndClass.lpfnWndProc   = WndProc;    
    WndClass.cbClsExtra    = 0;    
    WndClass.cbWndExtra    = 0;    
    WndClass.hInstance     = g_hInst;    
    WndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);    
    WndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);    
    WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);    
    WndClass.lpszMenuName  = MAKEINTRESOURCE(IDC_NOTEBOOK); //加载菜单资源，菜单资源ID为 IDC_NOTEBOOK   
    WndClass.lpszClassName = g_szClassName;    
    WndClass.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);    
  
    if(!RegisterClassEx(&WndClass))    
    {    
        MessageBox(0, "注册窗口失败", "错误信息",    
            MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);    
        return 0;    
    }    
  
    hwnd = CreateWindowEx(    
        WS_EX_CLIENTEDGE,    
        g_szClassName,    
        "记事本程序",    
        WS_OVERLAPPEDWINDOW,    
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,    
        NULL, NULL, g_hInst, NULL);    
  
    if(hwnd == NULL)    
    {    
        MessageBox(0, "创建窗口失败", "错误信息",    
            MB_ICONEXCLAMATION | MB_OK | MB_SYSTEMMODAL);    
        return 0;    
    }    
  
    ShowWindow(hwnd, nCmdShow);    
    UpdateWindow(hwnd);    
  
    while(GetMessage(&Msg, NULL, 0, 0))    
    {    
        TranslateMessage(&Msg);    
        DispatchMessage(&Msg);    
    }    
    return Msg.wParam;    
}    
