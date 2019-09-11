//////////////////////////////////////////////////////////////////////////////
//
//  Detours Test Program (tracereg.cpp of tracereg.dll)
//
//  Microsoft Research Detours Package, Version 2.1.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
#define _WIN32_WINNT        0x0400
#define WIN32
#define NT

#define DBG_TRACE   0

#include <windows.h>
#include <stdio.h>
#include "detours.h"
#include "syelog.h"

#define PULONG_PTR          PVOID
#define PLONG_PTR           PVOID
#define ULONG_PTR           PVOID
#define ENUMRESNAMEPROCA    PVOID
#define ENUMRESNAMEPROCW    PVOID
#define ENUMRESLANGPROCA    PVOID
#define ENUMRESLANGPROCW    PVOID
#define ENUMRESTYPEPROCA    PVOID
#define ENUMRESTYPEPROCW    PVOID
#define STGOPTIONS          PVOID

//////////////////////////////////////////////////////////////////////////////
#pragma warning(disable:4127)   // Many of our asserts are constants.

#define ASSERT_ALWAYS(x)   \
    do {                                                        \
    if (!(x)) {                                                 \
            AssertMessage(#x, __FILE__, __LINE__);              \
            DebugBreak();                                       \
    }                                                           \
    } while (0)

#ifndef NDEBUG
#define ASSERT(x)           ASSERT_ALWAYS(x)
#else
#define ASSERT(x)
#endif

#define UNUSED(c)       (c) = (c)
#define ARRAYOF(x)      (sizeof(x)/sizeof(x[0]))

//////////////////////////////////////////////////////////////////////////////
static HMODULE s_hInst = NULL;
static HMODULE s_hDetouredDll = NULL;
static CHAR s_szDllPath[MAX_PATH];
static CHAR s_szDetouredDllPath[MAX_PATH];

BOOL ProcessEnumerate();
BOOL InstanceEnumerate(HINSTANCE hInst);

VOID _PrintEnter(PCSTR psz, ...);
VOID _PrintExit(PCSTR psz, ...);
VOID _Print(PCSTR psz, ...);

VOID AssertMessage(CONST PCHAR pszMsg, CONST PCHAR pszFile, ULONG nLine);

//////////////////////////////////////////////////////////////////////////////
//
extern "C" {
    HANDLE (WINAPI *
            Real_CreateFileW)(LPCWSTR a0,
                              DWORD a1,
                              DWORD a2,
                              LPSECURITY_ATTRIBUTES a3,
                              DWORD a4,
                              DWORD a5,
                              HANDLE a6)
        = CreateFileW;

    BOOL (WINAPI *
          Real_WriteFile)(HANDLE hFile,
                          LPCVOID lpBuffer,
                          DWORD nNumberOfBytesToWrite,
                          LPDWORD lpNumberOfBytesWritten,
                          LPOVERLAPPED lpOverlapped)
        = WriteFile;
    BOOL (WINAPI *
          Real_FlushFileBuffers)(HANDLE hFile)
        = FlushFileBuffers;
    BOOL (WINAPI *
          Real_CloseHandle)(HANDLE hObject)
        = CloseHandle;

    BOOL (WINAPI *
          Real_WaitNamedPipeW)(LPCWSTR lpNamedPipeName, DWORD nTimeOut)
        = WaitNamedPipeW;
    BOOL (WINAPI *
          Real_SetNamedPipeHandleState)(HANDLE hNamedPipe,
                                        LPDWORD lpMode,
                                        LPDWORD lpMaxCollectionCount,
                                        LPDWORD lpCollectDataTimeout)
        = SetNamedPipeHandleState;

    DWORD (WINAPI *
           Real_GetCurrentProcessId)(VOID)
        = GetCurrentProcessId;
    VOID (WINAPI *
          Real_GetSystemTimeAsFileTime)(LPFILETIME lpSystemTimeAsFileTime)
        = GetSystemTimeAsFileTime;

    VOID (WINAPI *
          Real_InitializeCriticalSection)(LPCRITICAL_SECTION lpSection)
        = InitializeCriticalSection;
    VOID (WINAPI *
          Real_EnterCriticalSection)(LPCRITICAL_SECTION lpSection)
        = EnterCriticalSection;
    VOID (WINAPI *
          Real_LeaveCriticalSection)(LPCRITICAL_SECTION lpSection)
        = LeaveCriticalSection;
}

//////////////////////////////////////////////////////////////////////////////
//

BOOL (WINAPI * Real_CopyFileExA)(LPCSTR a0,
                                 LPCSTR a1,
                                 LPPROGRESS_ROUTINE a2,
                                 LPVOID a3,
                                 LPBOOL a4,
                                 DWORD a5)
    = CopyFileExA;

BOOL (WINAPI * Real_CopyFileExW)(LPCWSTR a0,
                                 LPCWSTR a1,
                                 LPPROGRESS_ROUTINE a2,
                                 LPVOID a3,
                                 LPBOOL a4,
                                 DWORD a5)
    = CopyFileExW;

BOOL (WINAPI * Real_CreateDirectoryExW)(LPCWSTR a0,
                                        LPCWSTR a1,
                                        LPSECURITY_ATTRIBUTES a2)
    = CreateDirectoryExW;

BOOL (WINAPI * Real_CreateDirectoryW)(LPCWSTR a0,
                                      LPSECURITY_ATTRIBUTES a1)
    = CreateDirectoryW;

BOOL (WINAPI * Real_CreateProcessW)(LPCWSTR lpApplicationName,
                                    LPWSTR lpCommandLine,
                                    LPSECURITY_ATTRIBUTES lpProcessAttributes,
                                    LPSECURITY_ATTRIBUTES lpThreadAttributes,
                                    BOOL bInheritHandles,
                                    DWORD dwCreationFlags,
                                    LPVOID lpEnvironment,
                                    LPCWSTR lpCurrentDirectory,
                                    LPSTARTUPINFOW lpStartupInfo,
                                    LPPROCESS_INFORMATION lpProcessInformation)
    = CreateProcessW;

BOOL (WINAPI * Real_DeleteFileA)(LPCSTR a0)
    = DeleteFileA;

BOOL (WINAPI * Real_DeleteFileW)(LPCWSTR a0)
    = DeleteFileW;

HANDLE (WINAPI * Real_FindFirstFileExA)(LPCSTR a0,
                                        FINDEX_INFO_LEVELS a1,
                                        LPVOID a2,
                                        FINDEX_SEARCH_OPS a3,
                                        LPVOID a4,
                                        DWORD a5)
    = FindFirstFileExA;

HANDLE (WINAPI * Real_FindFirstFileExW)(LPCWSTR a0,
                                        FINDEX_INFO_LEVELS a1,
                                        LPVOID a2,
                                        FINDEX_SEARCH_OPS a3,
                                        LPVOID a4,
                                        DWORD a5)
    = FindFirstFileExW;

DWORD (WINAPI * Real_GetFileAttributesW)(LPCWSTR a0)
    = GetFileAttributesW;

DWORD (WINAPI * Real_GetModuleFileNameW)(HMODULE a0,
                                         LPWSTR a1,
                                         DWORD a2)
    = GetModuleFileNameW;

DWORD (WINAPI * Real_GetModuleFileNameA)(HMODULE a0,
                                         LPSTR a1,
                                         DWORD a2)
    = GetModuleFileNameA;

FARPROC (WINAPI * Real_GetProcAddress)(struct HINSTANCE__* a0,
                                       LPCSTR a1)
    = GetProcAddress;

HMODULE (WINAPI * Real_LoadLibraryExW)(LPCWSTR a0,
                                       HANDLE a1,
                                       DWORD a2)
    = LoadLibraryExW;

BOOL (WINAPI * Real_MoveFileA)(LPCSTR a0,
                               LPCSTR a1)
    = MoveFileA;

BOOL (WINAPI * Real_MoveFileExA)(LPCSTR a0,
                                 LPCSTR a1,
                                 DWORD a2)
    = MoveFileExA;

BOOL (WINAPI * Real_MoveFileExW)(LPCWSTR a0,
                                 LPCWSTR a1,
                                 DWORD a2)
    = MoveFileExW;

BOOL (WINAPI * Real_MoveFileW)(LPCWSTR a0,
                               LPCWSTR a1)
    = MoveFileW;

HFILE (WINAPI * Real_OpenFile)(LPCSTR a0,
                               struct _OFSTRUCT* a1,
                               UINT a2)
    = OpenFile;

LONG (WINAPI * Real_RegCreateKeyExA)(HKEY a0,
                                     LPCSTR a1,
                                     DWORD a2,
                                     LPSTR a3,
                                     DWORD a4,
                                     REGSAM a5,
                                     LPSECURITY_ATTRIBUTES a6,
                                     PHKEY a7,
                                     LPDWORD a8)
    = RegCreateKeyExA;

LONG (WINAPI * Real_RegCreateKeyExW)(HKEY a0,
                                     LPCWSTR a1,
                                     DWORD a2,
                                     LPWSTR a3,
                                     DWORD a4,
                                     REGSAM a5,
                                     LPSECURITY_ATTRIBUTES a6,
                                     PHKEY a7,
                                     LPDWORD a8)
    = RegCreateKeyExW;

LONG (WINAPI * Real_RegDeleteKeyA)(HKEY a0,
                                   LPCSTR a1)
    = RegDeleteKeyA;

LONG (WINAPI * Real_RegDeleteKeyW)(HKEY a0,
                                   LPCWSTR a1)
    = RegDeleteKeyW;

LONG (WINAPI * Real_RegDeleteValueA)(HKEY a0,
                                     LPCSTR a1)
    = RegDeleteValueA;


LONG (WINAPI * Real_RegDeleteValueW)(HKEY a0,
                                     LPCWSTR a1)
    = RegDeleteValueW;

LONG (WINAPI * Real_RegEnumKeyExA)(HKEY a0,
                                   DWORD a1,
                                   LPSTR a2,
                                   LPDWORD a3,
                                   LPDWORD a4,
                                   LPSTR a5,
                                   LPDWORD a6,
                                   struct _FILETIME* a7)
    = RegEnumKeyExA;

LONG (WINAPI * Real_RegEnumKeyExW)(HKEY a0,
                                   DWORD a1,
                                   LPWSTR a2,
                                   LPDWORD a3,
                                   LPDWORD a4,
                                   LPWSTR a5,
                                   LPDWORD a6,
                                   struct _FILETIME* a7)
    = RegEnumKeyExW;

LONG (WINAPI * Real_RegEnumValueA)(HKEY a0,
                                   DWORD a1,
                                   LPSTR a2,
                                   LPDWORD a3,
                                   LPDWORD a4,
                                   LPDWORD a5,
                                   LPBYTE a6,
                                   LPDWORD a7)
    = RegEnumValueA;

LONG (WINAPI * Real_RegEnumValueW)(HKEY a0,
                                   DWORD a1,
                                   LPWSTR a2,
                                   LPDWORD a3,
                                   LPDWORD a4,
                                   LPDWORD a5,
                                   LPBYTE a6,
                                   LPDWORD a7)
    = RegEnumValueW;

LONG (WINAPI * Real_RegOpenKeyExA)(HKEY a0,
                                   LPCSTR a1,
                                   DWORD a2,
                                   REGSAM a3,
                                   PHKEY a4)
    = RegOpenKeyExA;

LONG (WINAPI * Real_RegOpenKeyExW)(HKEY a0,
                                   LPCWSTR a1,
                                   DWORD a2,
                                   REGSAM a3,
                                   PHKEY a4)
    = RegOpenKeyExW;

LONG (WINAPI * Real_RegQueryInfoKeyA)(HKEY a0,
                                      LPSTR a1,
                                      LPDWORD a2,
                                      LPDWORD a3,
                                      LPDWORD a4,
                                      LPDWORD a5,
                                      LPDWORD a6,
                                      LPDWORD a7,
                                      LPDWORD a8,
                                      LPDWORD a9,
                                      LPDWORD a10,
                                      struct _FILETIME* a11)
    = RegQueryInfoKeyA;

LONG (WINAPI * Real_RegQueryInfoKeyW)(HKEY a0,
                                      LPWSTR a1,
                                      LPDWORD a2,
                                      LPDWORD a3,
                                      LPDWORD a4,
                                      LPDWORD a5,
                                      LPDWORD a6,
                                      LPDWORD a7,
                                      LPDWORD a8,
                                      LPDWORD a9,
                                      LPDWORD a10,
                                      struct _FILETIME* a11)
    = RegQueryInfoKeyW;

LONG (WINAPI * Real_RegQueryValueExA)(HKEY a0,
                                      LPCSTR a1,
                                      LPDWORD a2,
                                      LPDWORD a3,
                                      LPBYTE a4,
                                      LPDWORD a5)
    = RegQueryValueExA;

LONG (WINAPI * Real_RegQueryValueExW)(HKEY a0,
                                      LPCWSTR a1,
                                      LPDWORD a2,
                                      LPDWORD a3,
                                      LPBYTE a4,
                                      LPDWORD a5)
    = RegQueryValueExW;

LONG (WINAPI * Real_RegSetValueExA)(HKEY a0,
                                    LPCSTR a1,
                                    DWORD a2,
                                    DWORD a3,
                                    const BYTE* a4,
                                    DWORD a5)
    = RegSetValueExA;

LONG (WINAPI * Real_RegSetValueExW)(HKEY a0,
                                    LPCWSTR a1,
                                    DWORD a2,
                                    DWORD a3,
                                    const BYTE* a4,
                                    DWORD a5)
    = RegSetValueExW;

HFILE (WINAPI * Real__lcreat)(LPCSTR a0,
                              int a1)
    = _lcreat;

HFILE (WINAPI * Real__lopen)(LPCSTR a0,
                             int a1)
    = _lopen;

/////////////////////////////////////////////////////////////
// Detours
//
BOOL WINAPI Mine_WaitNamedPipeW(LPCWSTR lpNamedPipeName, DWORD nTimeOut)
{
    return Real_WaitNamedPipeW(lpNamedPipeName, nTimeOut);
}

BOOL WINAPI Mine_CloseHandle(HANDLE hObject)
{
    return Real_CloseHandle(hObject);
}

VOID WINAPI Mine_GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime)
{
    Real_GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
}

BOOL WINAPI Mine_SetNamedPipeHandleState(HANDLE hNamedPipe,
                                            LPDWORD lpMode,
                                            LPDWORD lpMaxCollectionCount,
                                            LPDWORD lpCollectDataTimeout)
{
    return Real_SetNamedPipeHandleState(hNamedPipe,
                                        lpMode,
                                        lpMaxCollectionCount,
                                        lpCollectDataTimeout);
}

BOOL WINAPI Mine_WriteFile(HANDLE hFile,
                           LPCVOID lpBuffer,
                           DWORD nNumberOfBytesToWrite,
                           LPDWORD lpNumberOfBytesWritten,
                           LPOVERLAPPED lpOverlapped)
{
    return Real_WriteFile(hFile,
                          lpBuffer,
                          nNumberOfBytesToWrite,
                          lpNumberOfBytesWritten,
                          lpOverlapped);
}

BOOL WINAPI Mine_CreateProcessW(LPCWSTR lpApplicationName,
                                LPWSTR lpCommandLine,
                                LPSECURITY_ATTRIBUTES lpProcessAttributes,
                                LPSECURITY_ATTRIBUTES lpThreadAttributes,
                                BOOL bInheritHandles,
                                DWORD dwCreationFlags,
                                LPVOID lpEnvironment,
                                LPCWSTR lpCurrentDirectory,
                                LPSTARTUPINFOW lpStartupInfo,
                                LPPROCESS_INFORMATION lpProcessInformation)
{
    _PrintEnter("CreateProcessW(%ls,%ls,%p,%p,%x,%x,%p,%ls,%p,%p)\n",
                lpApplicationName,
                lpCommandLine,
                lpProcessAttributes,
                lpThreadAttributes,
                bInheritHandles,
                dwCreationFlags,
                lpEnvironment,
                lpCurrentDirectory,
                lpStartupInfo,
                lpProcessInformation);

    _Print("Calling DetourCreateProcessWithDllW(,%hs)\n", s_szDllPath);

    BOOL rv = 0;
    __try {
        rv = DetourCreateProcessWithDllW(lpApplicationName,
                                         lpCommandLine,
                                         lpProcessAttributes,
                                         lpThreadAttributes,
                                         bInheritHandles,
                                         dwCreationFlags,
                                         lpEnvironment,
                                         lpCurrentDirectory,
                                         lpStartupInfo,
                                         lpProcessInformation,
                                         s_szDetouredDllPath,
                                         s_szDllPath,
                                         Real_CreateProcessW);
    } __finally {
        _PrintExit("CreateProcessW(,,,,,,,,,) -> %x\n", rv);
    };
    return rv;
}

//
//////////////////////////////////////////////////////////////////////////////

BOOL WINAPI Mine_CopyFileExA(LPCSTR a0,
                             LPCSTR a1,
                             LPPROGRESS_ROUTINE a2,
                             LPVOID a3,
                             LPBOOL a4,
                             DWORD a5)
{
    _PrintEnter("CopyFileExA(%hs,%hs,%p,%p,%p,%x)\n", a0, a1, a2, a3, a4, a5);

    BOOL rv = 0;
    __try {
        rv = Real_CopyFileExA(a0, a1, a2, a3, a4, a5);
    } __finally {
        _PrintExit("CopyFileExA(,,,,,) -> %x\n", rv);
    };
    return rv;
}

BOOL WINAPI Mine_CopyFileExW(LPCWSTR a0,
                             LPCWSTR a1,
                             LPPROGRESS_ROUTINE a2,
                             LPVOID a3,
                             LPBOOL a4,
                             DWORD a5)
{
    _PrintEnter("CopyFileExW(%ls,%ls,%p,%p,%p,%x)\n", a0, a1, a2, a3, a4, a5);

    BOOL rv = 0;
    __try {
        rv = Real_CopyFileExW(a0, a1, a2, a3, a4, a5);
    } __finally {
        _PrintExit("CopyFileExW(,,,,,) -> %x\n", rv);
    };
    return rv;
}

BOOL WINAPI Mine_CreateDirectoryExW(LPCWSTR a0,
                                    LPCWSTR a1,
                                    LPSECURITY_ATTRIBUTES a2)
{
    _PrintEnter("CreateDirectoryExW(%ls,%ls,%p)\n", a0, a1, a2);

    BOOL rv = 0;
    __try {
        rv = Real_CreateDirectoryExW(a0, a1, a2);
    } __finally {
        _PrintExit("CreateDirectoryExW(,,) -> %x\n", rv);
    };
    return rv;
}

BOOL WINAPI Mine_CreateDirectoryW(LPCWSTR a0,
                                  LPSECURITY_ATTRIBUTES a1)
{
    _PrintEnter("CreateDirectoryW(%ls,%p)\n", a0, a1);

    BOOL rv = 0;
    __try {
        rv = Real_CreateDirectoryW(a0, a1);
    } __finally {
        _PrintExit("CreateDirectoryW(,) -> %x\n", rv);
    };
    return rv;
}

HANDLE WINAPI Mine_CreateFileW(LPCWSTR a0,
                               DWORD a1,
                               DWORD a2,
                               LPSECURITY_ATTRIBUTES a3,
                               DWORD a4,
                               DWORD a5,
                               HANDLE a6)
{
    _PrintEnter(NULL);
    HANDLE rv = 0;
    __try {
        rv = Real_CreateFileW(a0, a1, a2, a3, a4, a5, a6);
    } __finally {
        _PrintExit("CreateFileW(%ls,%x,%x,%p,%x,%x,%p) -> %p\n",
                   a0, a1, a2, a3, a4, a5, a6, rv);
    };
    return rv;
}

BOOL WINAPI Mine_DeleteFileA(LPCSTR a0)
{
    _PrintEnter("DeleteFileA(%hs)\n", a0);

    BOOL rv = 0;
    __try {
        rv = Real_DeleteFileA(a0);
    } __finally {
        _PrintExit("DeleteFileA() -> %x\n", rv);
    };
    return rv;
}

BOOL WINAPI Mine_DeleteFileW(LPCWSTR a0)
{
    _PrintEnter("DeleteFileW(%ls)\n", a0);

    BOOL rv = 0;
    __try {
        rv = Real_DeleteFileW(a0);
    } __finally {
        _PrintExit("DeleteFileW() -> %x\n", rv);
    };
    return rv;
}

HANDLE WINAPI Mine_FindFirstFileExA(LPCSTR a0,
                                    FINDEX_INFO_LEVELS a1,
                                    LPVOID a2,
                                    FINDEX_SEARCH_OPS a3,
                                    LPVOID a4,
                                    DWORD a5)
{
    _PrintEnter("FindFirstFileExA(%hs,%p,%p,%x,%p,%x)\n", a0, a1, a2, a3, a4, a5);

    HANDLE rv = 0;
    __try {
        rv = Real_FindFirstFileExA(a0, a1, a2, a3, a4, a5);
    } __finally {
        _PrintExit("FindFirstFileExA(,,,,,) -> %p\n", rv);
    };
    return rv;
}

HANDLE WINAPI Mine_FindFirstFileExW(LPCWSTR a0,
                                    FINDEX_INFO_LEVELS a1,
                                    LPVOID a2,
                                    FINDEX_SEARCH_OPS a3,
                                    LPVOID a4,
                                    DWORD a5)
{
    _PrintEnter(NULL);

    HANDLE rv = 0;
    __try {
        rv = Real_FindFirstFileExW(a0, a1, a2, a3, a4, a5);
    } __finally {
        _PrintExit("FindFirstFileExW(%ls,%x,%p,%x,%p,%x) -> %p\n",
                   a0, a1, a2, a3, a4, a5, rv);
    };
    return rv;
}

DWORD WINAPI Mine_GetFileAttributesW(LPCWSTR a0)
{
    _PrintEnter(NULL);

    DWORD rv = 0;
    __try {
        rv = Real_GetFileAttributesW(a0);
    } __finally {
        _PrintExit("GetFileAttributesW(%ls) -> %x\n", a0, rv);
    };
    return rv;
}

DWORD WINAPI Mine_GetModuleFileNameW(HMODULE a0, LPWSTR a1, DWORD a2)
{
    _PrintEnter("GetModuleFileNameW(%p,%p,%x)\n", a0, a1, a2);
    DWORD rv = 0;
    __try {
        rv = Real_GetModuleFileNameW(a0, a1, a2);
    } __finally {
        _PrintExit("GetModuleFileNameW(%p,%p:%ls,%p) -> %p\n", a0, a1, a1, a2, rv);
    };
    return rv;
}

FARPROC WINAPI Mine_GetProcAddress(HINSTANCE a0,
                                   LPCSTR a1)
{
    WCHAR wzModule[MAX_PATH] = L"";
    PWCHAR pwzModule = wzModule;
    if (Real_GetModuleFileNameW(a0, wzModule, ARRAYOF(wzModule)) != 0) {
        if ((pwzModule = wcsrchr(wzModule, '\\')) == NULL) {
            if ((pwzModule = wcsrchr(wzModule, ':')) == NULL) {
                pwzModule = wzModule;
            }
            else {
                pwzModule++;                            // Skip ':'
            }
        }
        else {
            pwzModule++;                                // Skip '\\'
        }
    }
    else {
        wzModule[0] = '\0';
    }

    _PrintEnter(NULL);
    FARPROC rv = 0;
    __try {
        rv = Real_GetProcAddress(a0, a1);
    } __finally {
        if (pwzModule[0] == 0) {
            _PrintExit("GetProcAddress(%p,%hs) -> %p\n", a0, a1, rv);
        }
        else {
            _PrintExit("GetProcAddress(%p:%ls,%hs) -> %p\n", a0, pwzModule, a1, rv);
        }
    };
    return rv;
}

HMODULE WINAPI Mine_LoadLibraryExW(LPCWSTR a0,
                                   HANDLE a1,
                                   DWORD a2)
{
    _PrintEnter("LoadLibraryExW(%ls,%p,%x)\n", a0, a1, a2);

    HMODULE rv = 0;
    __try {
        rv = Real_LoadLibraryExW(a0, a1, a2);
    } __finally {
        _PrintExit("LoadLibraryExW(,,) -> %p\n", rv);
        if (rv) {
            InstanceEnumerate(rv);
        }
    };
    return rv;
}

BOOL WINAPI Mine_MoveFileA(LPCSTR a0,
                           LPCSTR a1)
{
    _PrintEnter("MoveFileA(%hs,%hs)\n", a0, a1);

    BOOL rv = 0;
    __try {
        rv = Real_MoveFileA(a0, a1);
    } __finally {
        _PrintExit("MoveFileA(,) -> %x\n", rv);
    };
    return rv;
}

BOOL WINAPI Mine_MoveFileExA(LPCSTR a0,
                             LPCSTR a1,
                             DWORD a2)
{
    _PrintEnter("MoveFileExA(%hs,%hs,%x)\n", a0, a1, a2);

    BOOL rv = 0;
    __try {
        rv = Real_MoveFileExA(a0, a1, a2);
    } __finally {
        _PrintExit("MoveFileExA(,,) -> %x\n", rv);
    };
    return rv;
}

BOOL WINAPI Mine_MoveFileExW(LPCWSTR a0,
                             LPCWSTR a1,
                             DWORD a2)
{
    _PrintEnter("MoveFileExW(%ls,%ls,%x)\n", a0, a1, a2);

    BOOL rv = 0;
    __try {
        rv = Real_MoveFileExW(a0, a1, a2);
    } __finally {
        _PrintExit("MoveFileExW(,,) -> %x\n", rv);
    };
    return rv;
}

BOOL WINAPI Mine_MoveFileW(LPCWSTR a0,
                           LPCWSTR a1)
{
    _PrintEnter("MoveFileW(%ls,%ls)\n", a0, a1);

    BOOL rv = 0;
    __try {
        rv = Real_MoveFileW(a0, a1);
    } __finally {
        _PrintExit("MoveFileW(,) -> %x\n", rv);
    };
    return rv;
}

HFILE WINAPI Mine_OpenFile(LPCSTR a0,
                           LPOFSTRUCT a1,
                           UINT a2)
{
    _PrintEnter("OpenFile(%hs,%p,%x)\n", a0, a1, a2);

    HFILE rv = 0;
    __try {
        rv = Real_OpenFile(a0, a1, a2);
    } __finally {
        _PrintExit("OpenFile(,,) -> %p\n", rv);
    };
    return rv;
}

LONG WINAPI Mine_RegCreateKeyExA(HKEY a0,
                                 LPCSTR a1,
                                 DWORD a2,
                                 LPSTR a3,
                                 DWORD a4,
                                 REGSAM a5,
                                 LPSECURITY_ATTRIBUTES a6,
                                 PHKEY a7,
                                 LPDWORD a8)
{
    _PrintEnter("RegCreateKeyExA(%p,%hs,%x,%hs,%x,%x,%p,%p,%p)\n", a0, a1, a2, a3, a4, a5, a6, a7, a8);

    LONG rv = 0;
    __try {
        rv = Real_RegCreateKeyExA(a0, a1, a2, a3, a4, a5, a6, a7, a8);
    } __finally {
        _PrintExit("RegCreateKeyExA(,,,,,,,,) -> %x\n", rv);
    };
    return rv;
}

LONG WINAPI Mine_RegCreateKeyExW(HKEY a0,
                                 LPCWSTR a1,
                                 DWORD a2,
                                 LPWSTR a3,
                                 DWORD a4,
                                 REGSAM a5,
                                 LPSECURITY_ATTRIBUTES a6,
                                 PHKEY a7,
                                 LPDWORD a8)
{
    _PrintEnter(NULL);
    LONG rv = 0;
    __try {
        rv = Real_RegCreateKeyExW(a0, a1, a2, a3, a4, a5, a6, a7, a8);
    } __finally {
        _PrintExit("RegCreateKeyExW(%p,%ls,%x,%ls,%x,%x,%p,%p,%p) -> %x\n",
                   a0, a1, a2, a3, a4, a5, a6, a7, a8, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegDeleteKeyA(HKEY a0,
                               LPCSTR a1)
{
    _PrintEnter(NULL);
    LONG rv = 0;
    __try {
        rv = Real_RegDeleteKeyA(a0, a1);
    } __finally {
        _PrintExit("RegDeleteKeyA(%p,%hs) -> %x\n", a0, a1, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegDeleteKeyW(HKEY a0,
                               LPCWSTR a1)
{
    _PrintEnter(NULL);
    LONG rv = 0;
    __try {
        rv = Real_RegDeleteKeyW(a0, a1);
    } __finally {
        _PrintExit("RegDeleteKeyW(%p,%ls) -> %x\n", a0, a1, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegDeleteValueA(HKEY a0,
                                 LPCSTR a1)
{
    _PrintEnter("RegDeleteValueA(%p,%hs)\n", a0, a1);

    LONG rv = 0;
    __try {
        rv = Real_RegDeleteValueA(a0, a1);
    } __finally {
        _PrintExit("RegDeleteValueA(,) -> %x\n", rv);
    };
    return rv;
}

LONG WINAPI Mine_RegDeleteValueW(HKEY a0,
                                 LPCWSTR a1)
{
    _PrintEnter("RegDeleteValueW(%p,%ls)\n", a0, a1);

    LONG rv = 0;
    __try {
        rv = Real_RegDeleteValueW(a0, a1);
    } __finally {
        _PrintExit("RegDeleteValueW(,) -> %x\n", rv);
    };
    return rv;
}

LONG WINAPI Mine_RegEnumKeyExA(HKEY a0,
                               DWORD a1,
                               LPSTR a2,
                               LPDWORD a3,
                               LPDWORD a4,
                               LPSTR a5,
                               LPDWORD a6,
                               LPFILETIME a7)
{
    _PrintEnter("RegEnumKeyExA(%p,%x,%p,%p,%p,%hs,%p,%p)\n", a0, a1, a2, a3, a4, a5, a6, a7);

    LONG rv = 0;
    __try {
        rv = Real_RegEnumKeyExA(a0, a1, a2, a3, a4, a5, a6, a7);
    } __finally {
        _PrintExit("RegEnumKeyExA(,,%hs,,,%hs,,) -> %x\n", a2, a5, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegEnumKeyExW(HKEY a0,
                               DWORD a1,
                               LPWSTR a2,
                               LPDWORD a3,
                               LPDWORD a4,
                               LPWSTR a5,
                               LPDWORD a6,
                               struct _FILETIME* a7)
{
    _PrintEnter("RegEnumKeyExW(%p,%x,%p,%p,%p,%ls,%p,%p)\n", a0, a1, a2, a3, a4, a5, a6, a7);

    LONG rv = 0;
    __try {
        rv = Real_RegEnumKeyExW(a0, a1, a2, a3, a4, a5, a6, a7);
    } __finally {
        _PrintExit("RegEnumKeyExW(,,%ls,,,%ls,,) -> %x\n", a2, a5, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegEnumValueA(HKEY a0,
                               DWORD a1,
                               LPSTR a2,
                               LPDWORD a3,
                               LPDWORD a4,
                               LPDWORD a5,
                               LPBYTE a6,
                               LPDWORD a7)
{
    _PrintEnter("RegEnumValueA(%p,%x,%p,%p,%p,%p,%p,%p)\n", a0, a1, a2, a3, a4, a5, a6, a7);

    LONG rv = 0;
    __try {
        rv = Real_RegEnumValueA(a0, a1, a2, a3, a4, a5, a6, a7);
    } __finally {
        _PrintExit("RegEnumValueA(,,%hs,,,,,) -> %x\n", a2, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegEnumValueW(HKEY a0,
                               DWORD a1,
                               LPWSTR a2,
                               LPDWORD a3,
                               LPDWORD a4,
                               LPDWORD a5,
                               LPBYTE a6,
                               LPDWORD a7)
{
    _PrintEnter("RegEnumValueW(%p,%x,%p,%p,%p,%p,%p,%p)\n", a0, a1, a2, a3, a4, a5, a6, a7);

    LONG rv = 0;
    __try {
        rv = Real_RegEnumValueW(a0, a1, a2, a3, a4, a5, a6, a7);
    } __finally {
        _PrintExit("RegEnumValueW(,,%ls,,,,,) -> %x\n", a2, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegOpenKeyExA(HKEY a0,
                               LPCSTR a1,
                               DWORD a2,
                               REGSAM a3,
                               PHKEY a4)
{
    _PrintEnter(NULL);

    LONG rv = 0;
    __try {
        rv = Real_RegOpenKeyExA(a0, a1, a2, a3, a4);
    } __finally {
        _PrintExit("RegOpenKeyExA(%p,%hs,%x,%x,%p) -> %x\n",
                   a0, a1, a2, a3, a4, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegOpenKeyExW(HKEY a0,
                               LPCWSTR a1,
                               DWORD a2,
                               REGSAM a3,
                               PHKEY a4)
{
    _PrintEnter(NULL);

    LONG rv = 0;
    __try {
        rv = Real_RegOpenKeyExW(a0, a1, a2, a3, a4);
    } __finally {
        _PrintExit("RegOpenKeyExW(%p,%ls,%x,%x,%p) -> %x\n",
                   a0, a1, a2, a3, a4, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegQueryInfoKeyA(HKEY a0,
                                  LPSTR a1,
                                  LPDWORD a2,
                                  LPDWORD a3,
                                  LPDWORD a4,
                                  LPDWORD a5,
                                  LPDWORD a6,
                                  LPDWORD a7,
                                  LPDWORD a8,
                                  LPDWORD a9,
                                  LPDWORD a10,
                                  LPFILETIME a11)
{
    _PrintEnter("RegQueryInfoKeyA(%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p)\n",
                a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);

    LONG rv = 0;
    __try {
        rv = Real_RegQueryInfoKeyA(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
    } __finally {
        _PrintExit("RegQueryInfoKeyA(,%hs,,,,,,,,,,) -> %x\n", a1, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegQueryInfoKeyW(HKEY a0,
                                  LPWSTR a1,
                                  LPDWORD a2,
                                  LPDWORD a3,
                                  LPDWORD a4,
                                  LPDWORD a5,
                                  LPDWORD a6,
                                  LPDWORD a7,
                                  LPDWORD a8,
                                  LPDWORD a9,
                                  LPDWORD a10,
                                  LPFILETIME a11)
{
    _PrintEnter("RegQueryInfoKeyW(%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p,%p)\n",
                a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);

    LONG rv = 0;
    __try {
        rv = Real_RegQueryInfoKeyW(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
    } __finally {
        _PrintExit("RegQueryInfoKeyW(,%ls,,,,,,,,,,) -> %x\n", a1, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegQueryValueExA(HKEY a0,
                                  LPCSTR a1,
                                  LPDWORD a2,
                                  LPDWORD a3,
                                  LPBYTE a4,
                                  LPDWORD a5)
{
    _PrintEnter(NULL);

    LONG rv = 0;
    __try {
        rv = Real_RegQueryValueExA(a0, a1, a2, a3, a4, a5);
    } __finally {
        _PrintExit("RegQueryValueExA(%p,%hs,%p,%p,%p,%p) -> %x\n",
                   a0, a1, a2, a3, a4, a5, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegQueryValueExW(HKEY a0,
                                  LPCWSTR a1,
                                  LPDWORD a2,
                                  LPDWORD a3,
                                  LPBYTE a4,
                                  LPDWORD a5)
{
    _PrintEnter(NULL);
    LONG rv = 0;
    __try {
        rv = Real_RegQueryValueExW(a0, a1, a2, a3, a4, a5);
    } __finally {
        _PrintExit("RegQueryValueExW(%p,%ls,%p,%p,%p,%p) -> %x\n",
                    a0, a1, a2, a3, a4, a5, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegSetValueExA(HKEY a0,
                                LPCSTR a1,
                                DWORD a2,
                                DWORD a3,
                                BYTE* a4,
                                DWORD a5)
{
    _PrintEnter(NULL);
    LONG rv = 0;
    __try {
        rv = Real_RegSetValueExA(a0, a1, a2, a3, a4, a5);
    } __finally {
        _PrintExit("RegSetValueExA(%p,%hs,%x,%x,%p,%x) -> %x\n",
                   a0, a1, a2, a3, a4, a5, rv);
    };
    return rv;
}

LONG WINAPI Mine_RegSetValueExW(HKEY a0,
                                   LPCWSTR a1,
                                   DWORD a2,
                                   DWORD a3,
                                   BYTE* a4,
                                   DWORD a5)
{
    _PrintEnter(NULL);
    LONG rv = 0;
    __try {
        rv = Real_RegSetValueExW(a0, a1, a2, a3, a4, a5);
    } __finally {
        _PrintExit("RegSetValueExW(%p,%ls,%x,%x,%p,%x) -> %x\n",
                   a0, a1, a2, a3, a4, a5, rv);
    };
    return rv;
}

HFILE WINAPI Mine__lcreat(LPCSTR a0, int a1)
{
    _PrintEnter(NULL);
    HFILE rv = 0;
    __try {
        rv = Real__lcreat(a0, a1);
    } __finally {
        _PrintExit("_lcreat(%hs,%x) -> %p\n", a0, a1, rv);
    };
    return rv;
}

HFILE WINAPI Mine__lopen(LPCSTR a0, int a1)
{
    _PrintEnter(NULL);
    HFILE rv = 0;
    __try {
        rv = Real__lopen(a0, a1);
    } __finally {
        _PrintEnter("_lopen(%hs,%x) -> %p\n", a0, a1, rv);
    };
    return rv;
}

/////////////////////////////////////////////////////////////
// AttachDetours
//
PCHAR DetRealName(PCHAR psz)
{
    PCHAR pszBeg = psz;
    // Move to end of name.
    while (*psz) {
        psz++;
    }
    // Move back through A-Za-z0-9 names.
    while (psz > pszBeg &&
           ((psz[-1] >= 'A' && psz[-1] <= 'Z') ||
            (psz[-1] >= 'a' && psz[-1] <= 'z') ||
            (psz[-1] >= '0' && psz[-1] <= '9'))) {
        psz--;
    }
    return psz;
}

VOID DetAttach(PVOID *ppbReal, PVOID pbMine, PCHAR psz)
{
    LONG l = DetourAttach(ppbReal, pbMine);
    if (l != 0) {
        Syelog(SYELOG_SEVERITY_NOTICE,
               "Attach failed: `%s': error %d\n", DetRealName(psz), l);
    }
}

VOID DetDetach(PVOID *ppbReal, PVOID pbMine, PCHAR psz)
{
    LONG l = DetourDetach(ppbReal, pbMine);
    if (l != 0) {
        Syelog(SYELOG_SEVERITY_NOTICE,
               "Detach failed: `%s': error %d\n", DetRealName(psz), l);
    }
}

#define ATTACH(x,y)   DetAttach(x,y,#x)
#define DETACH(x,y)   DetDetach(x,y,#x)

LONG AttachDetours(VOID)
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    ATTACH(&(PVOID&)Real_CloseHandle, (PVOID)Mine_CloseHandle);
    ATTACH(&(PVOID&)Real_CopyFileExA, Mine_CopyFileExA);
    ATTACH(&(PVOID&)Real_CopyFileExW, Mine_CopyFileExW);
    ATTACH(&(PVOID&)Real_CreateDirectoryExW, Mine_CreateDirectoryExW);
    ATTACH(&(PVOID&)Real_CreateDirectoryW, Mine_CreateDirectoryW);
    ATTACH(&(PVOID&)Real_CreateFileW, Mine_CreateFileW);
    ATTACH(&(PVOID&)Real_CreateProcessW, (PVOID)Mine_CreateProcessW);
    ATTACH(&(PVOID&)Real_DeleteFileA, Mine_DeleteFileA);
    ATTACH(&(PVOID&)Real_DeleteFileW, Mine_DeleteFileW);
    ATTACH(&(PVOID&)Real_FindFirstFileExA, Mine_FindFirstFileExA);
    ATTACH(&(PVOID&)Real_FindFirstFileExW, Mine_FindFirstFileExW);
    ATTACH(&(PVOID&)Real_GetFileAttributesW, Mine_GetFileAttributesW);
    ATTACH(&(PVOID&)Real_GetModuleFileNameW, Mine_GetModuleFileNameW);
    ATTACH(&(PVOID&)Real_GetProcAddress, Mine_GetProcAddress);
    ATTACH(&(PVOID&)Real_GetSystemTimeAsFileTime, (PVOID)Mine_GetSystemTimeAsFileTime);
    ATTACH(&(PVOID&)Real_LoadLibraryExW, Mine_LoadLibraryExW);
    ATTACH(&(PVOID&)Real_MoveFileA, Mine_MoveFileA);
    ATTACH(&(PVOID&)Real_MoveFileExA, Mine_MoveFileExA);
    ATTACH(&(PVOID&)Real_MoveFileExW, Mine_MoveFileExW);
    ATTACH(&(PVOID&)Real_MoveFileW, Mine_MoveFileW);
    ATTACH(&(PVOID&)Real_OpenFile, Mine_OpenFile);
    ATTACH(&(PVOID&)Real_RegCreateKeyExA, Mine_RegCreateKeyExA);
    ATTACH(&(PVOID&)Real_RegCreateKeyExW, Mine_RegCreateKeyExW);
    ATTACH(&(PVOID&)Real_RegDeleteKeyA, Mine_RegDeleteKeyA);
    ATTACH(&(PVOID&)Real_RegDeleteKeyW, Mine_RegDeleteKeyW);
    ATTACH(&(PVOID&)Real_RegDeleteValueA, Mine_RegDeleteValueA);
    ATTACH(&(PVOID&)Real_RegDeleteValueW, Mine_RegDeleteValueW);
    ATTACH(&(PVOID&)Real_RegEnumKeyExA, Mine_RegEnumKeyExA);
    ATTACH(&(PVOID&)Real_RegEnumKeyExW, Mine_RegEnumKeyExW);
    ATTACH(&(PVOID&)Real_RegEnumValueA, Mine_RegEnumValueA);
    ATTACH(&(PVOID&)Real_RegEnumValueW, Mine_RegEnumValueW);
    ATTACH(&(PVOID&)Real_RegOpenKeyExA, Mine_RegOpenKeyExA);
    ATTACH(&(PVOID&)Real_RegOpenKeyExW, Mine_RegOpenKeyExW);
    ATTACH(&(PVOID&)Real_RegQueryInfoKeyA, Mine_RegQueryInfoKeyA);
    ATTACH(&(PVOID&)Real_RegQueryInfoKeyW, Mine_RegQueryInfoKeyW);
    ATTACH(&(PVOID&)Real_RegQueryValueExA, Mine_RegQueryValueExA);
    ATTACH(&(PVOID&)Real_RegQueryValueExW, Mine_RegQueryValueExW);
    ATTACH(&(PVOID&)Real_RegSetValueExA, Mine_RegSetValueExA);
    ATTACH(&(PVOID&)Real_RegSetValueExW, Mine_RegSetValueExW);
    ATTACH(&(PVOID&)Real_SetNamedPipeHandleState, (PVOID)Mine_SetNamedPipeHandleState);
    ATTACH(&(PVOID&)Real_WaitNamedPipeW, (PVOID)Mine_WaitNamedPipeW);
    ATTACH(&(PVOID&)Real_WriteFile, (PVOID)Mine_WriteFile);
    ATTACH(&(PVOID&)Real__lcreat, Mine__lcreat);
    ATTACH(&(PVOID&)Real__lopen, Mine__lopen);

    return DetourTransactionCommit();
}

LONG DetachDetours(VOID)
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    DETACH(&(PVOID&)Real_CloseHandle, (PVOID)Mine_CloseHandle);
    DETACH(&(PVOID&)Real_CopyFileExA, Mine_CopyFileExA);
    DETACH(&(PVOID&)Real_CopyFileExW, Mine_CopyFileExW);
    DETACH(&(PVOID&)Real_CreateDirectoryExW, Mine_CreateDirectoryExW);
    DETACH(&(PVOID&)Real_CreateDirectoryW, Mine_CreateDirectoryW);
    DETACH(&(PVOID&)Real_CreateFileW, Mine_CreateFileW);
    DETACH(&(PVOID&)Real_CreateProcessW, (PVOID)Mine_CreateProcessW);
    DETACH(&(PVOID&)Real_DeleteFileA, Mine_DeleteFileA);
    DETACH(&(PVOID&)Real_DeleteFileW, Mine_DeleteFileW);
    DETACH(&(PVOID&)Real_FindFirstFileExA, Mine_FindFirstFileExA);
    DETACH(&(PVOID&)Real_FindFirstFileExW, Mine_FindFirstFileExW);
    DETACH(&(PVOID&)Real_GetFileAttributesW, Mine_GetFileAttributesW);
    DETACH(&(PVOID&)Real_GetModuleFileNameW, Mine_GetModuleFileNameW);
    DETACH(&(PVOID&)Real_GetProcAddress, Mine_GetProcAddress);
    DETACH(&(PVOID&)Real_GetSystemTimeAsFileTime, (PVOID)Mine_GetSystemTimeAsFileTime);
    DETACH(&(PVOID&)Real_LoadLibraryExW, Mine_LoadLibraryExW);
    DETACH(&(PVOID&)Real_MoveFileA, Mine_MoveFileA);
    DETACH(&(PVOID&)Real_MoveFileExA, Mine_MoveFileExA);
    DETACH(&(PVOID&)Real_MoveFileExW, Mine_MoveFileExW);
    DETACH(&(PVOID&)Real_MoveFileW, Mine_MoveFileW);
    DETACH(&(PVOID&)Real_OpenFile, Mine_OpenFile);
    DETACH(&(PVOID&)Real_RegCreateKeyExA, Mine_RegCreateKeyExA);
    DETACH(&(PVOID&)Real_RegCreateKeyExW, Mine_RegCreateKeyExW);
    DETACH(&(PVOID&)Real_RegDeleteKeyA, Mine_RegDeleteKeyA);
    DETACH(&(PVOID&)Real_RegDeleteKeyW, Mine_RegDeleteKeyW);
    DETACH(&(PVOID&)Real_RegDeleteValueA, Mine_RegDeleteValueA);
    DETACH(&(PVOID&)Real_RegDeleteValueW, Mine_RegDeleteValueW);
    DETACH(&(PVOID&)Real_RegEnumKeyExA, Mine_RegEnumKeyExA);
    DETACH(&(PVOID&)Real_RegEnumKeyExW, Mine_RegEnumKeyExW);
    DETACH(&(PVOID&)Real_RegEnumValueA, Mine_RegEnumValueA);
    DETACH(&(PVOID&)Real_RegEnumValueW, Mine_RegEnumValueW);
    DETACH(&(PVOID&)Real_RegOpenKeyExA, Mine_RegOpenKeyExA);
    DETACH(&(PVOID&)Real_RegOpenKeyExW, Mine_RegOpenKeyExW);
    DETACH(&(PVOID&)Real_RegQueryInfoKeyA, Mine_RegQueryInfoKeyA);
    DETACH(&(PVOID&)Real_RegQueryInfoKeyW, Mine_RegQueryInfoKeyW);
    DETACH(&(PVOID&)Real_RegQueryValueExA, Mine_RegQueryValueExA);
    DETACH(&(PVOID&)Real_RegQueryValueExW, Mine_RegQueryValueExW);
    DETACH(&(PVOID&)Real_RegSetValueExA, Mine_RegSetValueExA);
    DETACH(&(PVOID&)Real_RegSetValueExW, Mine_RegSetValueExW);
    DETACH(&(PVOID&)Real_SetNamedPipeHandleState, (PVOID)Mine_SetNamedPipeHandleState);
    DETACH(&(PVOID&)Real_WaitNamedPipeW, (PVOID)Mine_WaitNamedPipeW);
    DETACH(&(PVOID&)Real_WriteFile, (PVOID)Mine_WriteFile);
    DETACH(&(PVOID&)Real__lcreat, Mine__lcreat);
    DETACH(&(PVOID&)Real__lopen, Mine__lopen);

    return DetourTransactionCommit();
}
//
//////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////// Logging System.
//
static BOOL s_bLog = 1;
static LONG s_nTlsIndent = -1;
static LONG s_nTlsThread = -1;
static LONG s_nThreadCnt = 0;

VOID _PrintEnter(const CHAR *psz, ...)
{
    DWORD dwErr = GetLastError();

    LONG nIndent = 0;
    LONG nThread = 0;
    if (s_nTlsIndent >= 0) {
        nIndent = (LONG)(LONG_PTR)TlsGetValue(s_nTlsIndent);
        TlsSetValue(s_nTlsIndent, (PVOID)(LONG_PTR)(nIndent + 1));
    }
    if (s_nTlsThread >= 0) {
        nThread = (LONG)(LONG_PTR)TlsGetValue(s_nTlsThread);
    }

    if (s_bLog && psz) {
        CHAR szBuf[1024];
        PCHAR pszBuf = szBuf;
        LONG nLen = (nIndent > 0) ? (nIndent < 35 ? nIndent * 2 : 70) : 0;
        *pszBuf++ = (CHAR)('0' + ((nThread / 100) % 10));
        *pszBuf++ = (CHAR)('0' + ((nThread / 10) % 10));
        *pszBuf++ = (CHAR)('0' + ((nThread / 1) % 10));
        *pszBuf++ = ' ';
        while (nLen-- > 0) {
            *pszBuf++ = ' ';
        }

        va_list  args;
        va_start(args, psz);

        while ((*pszBuf++ = *psz++) != 0) {
            // Copy characters.
        }
        SyelogV(SYELOG_SEVERITY_INFORMATION,
                szBuf, args);

        va_end(args);
    }
    SetLastError(dwErr);
}

VOID _PrintExit(const CHAR *psz, ...)
{
    DWORD dwErr = GetLastError();

    LONG nIndent = 0;
    LONG nThread = 0;
    if (s_nTlsIndent >= 0) {
        nIndent = (LONG)(LONG_PTR)TlsGetValue(s_nTlsIndent) - 1;
        ASSERT(nIndent >= 0);
        TlsSetValue(s_nTlsIndent, (PVOID)(LONG_PTR)nIndent);
    }
    if (s_nTlsThread >= 0) {
        nThread = (LONG)(LONG_PTR)TlsGetValue(s_nTlsThread);
    }

    if (s_bLog && psz) {
        CHAR szBuf[1024];
        PCHAR pszBuf = szBuf;
        LONG nLen = (nIndent > 0) ? (nIndent < 35 ? nIndent * 2 : 70) : 0;
        *pszBuf++ = (CHAR)('0' + ((nThread / 100) % 10));
        *pszBuf++ = (CHAR)('0' + ((nThread / 10) % 10));
        *pszBuf++ = (CHAR)('0' + ((nThread / 1) % 10));
        *pszBuf++ = ' ';
        while (nLen-- > 0) {
            *pszBuf++ = ' ';
        }

        va_list  args;
        va_start(args, psz);

        while ((*pszBuf++ = *psz++) != 0) {
            // Copy characters.
        }
        SyelogV(SYELOG_SEVERITY_INFORMATION,
                szBuf, args);

        va_end(args);
    }
    SetLastError(dwErr);
}

VOID _Print(const CHAR *psz, ...)
{
    DWORD dwErr = GetLastError();

    LONG nIndent = 0;
    LONG nThread = 0;
    if (s_nTlsIndent >= 0) {
        nIndent = (LONG)(LONG_PTR)TlsGetValue(s_nTlsIndent);
    }
    if (s_nTlsThread >= 0) {
        nThread = (LONG)(LONG_PTR)TlsGetValue(s_nTlsThread);
    }

    if (s_bLog && psz) {
        CHAR szBuf[1024];
        PCHAR pszBuf = szBuf;
        LONG nLen = (nIndent > 0) ? (nIndent < 35 ? nIndent * 2 : 70) : 0;
        *pszBuf++ = (CHAR)('0' + ((nThread / 100) % 10));
        *pszBuf++ = (CHAR)('0' + ((nThread / 10) % 10));
        *pszBuf++ = (CHAR)('0' + ((nThread / 1) % 10));
        *pszBuf++ = ' ';
        while (nLen-- > 0) {
            *pszBuf++ = ' ';
        }

        va_list  args;
        va_start(args, psz);

        while ((*pszBuf++ = *psz++) != 0) {
            // Copy characters.
        }
        SyelogV(SYELOG_SEVERITY_INFORMATION,
                szBuf, args);

        va_end(args);
    }

    SetLastError(dwErr);
}

VOID AssertMessage(CONST PCHAR pszMsg, CONST PCHAR pszFile, ULONG nLine)
{
    Syelog(SYELOG_SEVERITY_FATAL,
           "ASSERT(%s) failed in %s, line %d.\n", pszMsg, pszFile, nLine);
}

VOID NullExport()
{
}

//////////////////////////////////////////////////////////////////////////////
//
PIMAGE_NT_HEADERS NtHeadersForInstance(HINSTANCE hInst)
{
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hInst;
    __try {
        if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
            SetLastError(ERROR_BAD_EXE_FORMAT);
            return NULL;
        }

        PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((PBYTE)pDosHeader +
                                                          pDosHeader->e_lfanew);
        if (pNtHeader->Signature != IMAGE_NT_SIGNATURE) {
            SetLastError(ERROR_INVALID_EXE_SIGNATURE);
            return NULL;
        }
        if (pNtHeader->FileHeader.SizeOfOptionalHeader == 0) {
            SetLastError(ERROR_EXE_MARKED_INVALID);
            return NULL;
        }
        return pNtHeader;
    } __except(EXCEPTION_EXECUTE_HANDLER) {
    }
    SetLastError(ERROR_EXE_MARKED_INVALID);

    return NULL;
}

BOOL InstanceEnumerate(HINSTANCE hInst)
{
    WCHAR wzDllName[MAX_PATH];

    PIMAGE_NT_HEADERS pinh = NtHeadersForInstance(hInst);
    if (pinh && Real_GetModuleFileNameW(hInst, wzDllName, ARRAYOF(wzDllName))) {
        Syelog(SYELOG_SEVERITY_INFORMATION,
               "### %08lx: %-43.43ls %08x\n",
               hInst, wzDllName, pinh->OptionalHeader.CheckSum);
        return TRUE;
    }
    return FALSE;
}

BOOL ProcessEnumerate()
{
    Syelog(SYELOG_SEVERITY_INFORMATION,
           "######################################################### Binaries\n");
    for (HINSTANCE hInst = NULL; (hInst = DetourEnumerateModules(hInst)) != NULL;) {
        InstanceEnumerate(hInst);
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
// DLL module information
//
BOOL ThreadAttach(HMODULE hDll)
{
    (void)hDll;

    if (s_nTlsIndent >= 0) {
        TlsSetValue(s_nTlsIndent, (PVOID)0);
    }
    if (s_nTlsThread >= 0) {
        LONG nThread = InterlockedIncrement(&s_nThreadCnt);
        TlsSetValue(s_nTlsThread, (PVOID)(LONG_PTR)nThread);
    }
    return TRUE;
}

BOOL ThreadDetach(HMODULE hDll)
{
    (void)hDll;

    if (s_nTlsIndent >= 0) {
        TlsSetValue(s_nTlsIndent, (PVOID)0);
    }
    if (s_nTlsThread >= 0) {
        TlsSetValue(s_nTlsThread, (PVOID)0);
    }
    return TRUE;
}

BOOL ProcessAttach(HMODULE hDll)
{
    s_bLog = FALSE;
    s_nTlsIndent = TlsAlloc();
    s_nTlsThread = TlsAlloc();

    s_hInst = hDll;
#if 0
    s_hDetouredDll = DetourGetDetouredMarker();
    Real_GetModuleFileNameA(s_hDetouredDll, s_szDetouredDllPath, ARRAYOF(s_szDetouredDllPath));
    Real_GetModuleFileNameA(s_hInst, s_szDllPath, ARRAYOF(s_szDllPath));
#endif

    SyelogOpen("traceapi", SYELOG_FACILITY_APPLICATION);
    ProcessEnumerate();

    LONG error = AttachDetours();
    if (error != NO_ERROR) {
        Syelog(SYELOG_SEVERITY_FATAL, "### Error attaching detours: %d\n", error);
    }

    ThreadAttach(hDll);

    s_bLog = TRUE;
    return TRUE;
}

BOOL ProcessDetach(HMODULE hDll)
{
    ThreadDetach(hDll);
    s_bLog = FALSE;

    LONG error = DetachDetours();
    if (error != NO_ERROR) {
        Syelog(SYELOG_SEVERITY_FATAL, "### Error detaching detours: %d\n", error);
    }

    Syelog(SYELOG_SEVERITY_NOTICE, "### Closing.\n");
    SyelogClose(FALSE);

    if (s_nTlsIndent >= 0) {
        TlsFree(s_nTlsIndent);
    }
    if (s_nTlsThread >= 0) {
        TlsFree(s_nTlsThread);
    }
    return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
    (void)hModule;
    (void)lpReserved;

    switch (dwReason) {
      case DLL_PROCESS_ATTACH:
        printf("tracereg.dll: Starting.\n");
        fflush(stdout);
        Sleep(50);
        Sleep(50);
        DetourRestoreAfterWith();
        return ProcessAttach(hModule);
      case DLL_PROCESS_DETACH:
        return ProcessDetach(hModule);
      case DLL_THREAD_ATTACH:
        return ThreadAttach(hModule);
      case DLL_THREAD_DETACH:
        return ThreadDetach(hModule);
    }
    return TRUE;
}
//
///////////////////////////////////////////////////////////////// End of File.
