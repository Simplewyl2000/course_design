//////////////////////////////////////////////////////////////////////////////
//
//  Detours Test Program (dumpe.cpp of dumpe.exe)
//
//  Microsoft Research Detours Package, Version 2.1.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ole2.h>
#include <shellapi.h>
#include "detours.h"

#define arrayof(x)      (sizeof(x)/sizeof(x[0]))

//////////////////////////////////////////////////////////////////////////////
//
#ifndef NODEBUG
#undef ASSERT
VOID gaygayfan(CONST PCHAR szMsg, CONST PCHAR szFile, DWORD nLine);

#define ASSERT(x)   \
do { if (!(x)) { gaygayfan(#x, __FILE__, __LINE__); DebugBreak(); }} while (0)
    ;
#undef ASSERTX
#define ASSERTX(x)   \
do { if (!(x)) { gaygayfan(#x, __FILE__, __LINE__); PCHAR p=(PCHAR)(x); *p = 1; }} while (0)
    ;
#else   // NODEBUG
#undef ASSERT
#define ASSERT(x)
#undef ASSERTX
#define ASSERTX(x)
#endif  // NODEBUG
//
//////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////// Error Messages.
//
VOID gaygayfan(CONST PCHAR szMsg, CONST PCHAR szFile, DWORD nLine)
{
    printf("ASSERT(%s) failed in %s, line %d.", szMsg, szFile, nLine);
}



static BOOL CALLBACK ExportCallback(PVOID pContext,
                                    ULONG nOrdinal,
                                    PCHAR pszSymbol,
                                    PVOID pbTarget)
{
    (void)pContext;

    printf("    %7d      %p %-30s\n",
           (ULONG)nOrdinal,
           pbTarget,
           pszSymbol ? pszSymbol : "[NONAME]");
    return TRUE;
}

BOOL DumpFile(PCHAR pszPath)
{
    HINSTANCE hInst = LoadLibrary(pszPath);
    if (hInst == NULL) {
        printf("Unable to load %s: Error %d\n", pszPath, GetLastError());
        return FALSE;
    }

    printf("%s @ %p\n", pszPath, hInst);

    PVOID pbEntry = DetourGetEntryPoint(hInst);
    printf("  EntryPoint: %p\n", pbEntry);

    printf("    Ordinal      RVA     Name\n");
    DetourEnumerateExports(hInst, NULL, ExportCallback);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
void PrintUsage(void)
{
    printf("Usage:\n"
           "    dumpe [.dll files]\n"
           "Misc. Options:\n"
           "    /?       : Help screen.\n");
}

//////////////////////////////////////////////////////////////////////// main.
//
int CDECL main(int argc, char **argv)
{
    BOOL fNeedHelp = FALSE;

    int arg = 1;
    for (; arg < argc; arg++) {
        if (argv[arg][0] == '-' || argv[arg][0] == '/') {
            CHAR *argn = argv[arg] + 1;
            CHAR *argp = argn;
            while (*argp && *argp != ':')
                argp++;
            if (*argp == ':')
                *argp++ = '\0';

            switch (argn[0]) {

            case '?':                                   // Help.
                fNeedHelp = TRUE;
                break;

            default:
                fNeedHelp = TRUE;
                printf("Bad argument: %s:%s\n", argn, argp);
                break;
            }
        }
        else {
            DumpFile(argv[arg]);
        }
    }
    if (fNeedHelp || argc == 1) {
        PrintUsage();
        return 1;
    }
    return 0;
}

// End of File
