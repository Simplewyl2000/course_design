//////////////////////////////////////////////////////////////////////////////
//
//  Detours Test Program (impmunge.cpp of impmunge.exe)
//
//  Microsoft Research Detours Package, Version 2.1.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <detours.h>
#include <imagehlp.h>

////////////////////////////////////////////////////////////// Error Messages.
//
VOID gaygayfan(PCSTR szMsg, PCSTR szFile, DWORD nLine)
{
    printf("ASSERT(%s) failed in %s, line %d.", szMsg, szFile, nLine);
}

#define ASSERT(x)   \
do { if (!(x)) { gaygayfan(#x, __FILE__, __LINE__); DebugBreak(); }} while (0)
    ;


//////////////////////////////////////////////////////////////////////////////
//
static BOOLEAN s_fRestore = FALSE;
static BOOLEAN s_fList = TRUE;
static BOOLEAN s_fMunge = FALSE;
static BOOLEAN s_fToSymbols = FALSE;

//////////////////////////////////////////////////////////////////////////////
//
static BOOL CALLBACK ListByway(PVOID pContext,
                               PCHAR pszFile,
                               PCHAR *ppszOutFile)
{
    (void)pContext;
    (void)ppszOutFile;

    printf("  byway   -------------------------------- %s\n", pszFile ? pszFile : "");
    return TRUE;
}

static BOOL CALLBACK ListFile(PVOID pContext,
                              PCHAR pszOrigFile,
                              PCHAR pszFile,
                              PCHAR *ppszOutFile)
{
    (void)pContext;
    (void)ppszOutFile;

    printf("  file    %-32.32s %-32.32s\n",
           pszOrigFile ? pszOrigFile : "",
           pszFile ? pszFile : "");
    return TRUE;
}

static BOOL CALLBACK ListSymbol(PVOID pContext,
                                ULONG nOrigOrdinal,
                                ULONG nOrdinal,
                                ULONG *pnOutOrdinal,
                                PCHAR pszOrigSymbol,
                                PCHAR pszSymbol,
                                PCHAR *ppszOutSymbol)
{
    (void)pContext;
    (void)pnOutOrdinal;
    (void)ppszOutSymbol;

    char szOrig[80];
    char szLast[80];

    if (pszOrigSymbol == NULL) {
#ifdef _CRT_INSECURE_DEPRECATE
        sprintf_s(szOrig, sizeof(szOrig), "#%d", nOrigOrdinal);
#else
        sprintf(szOrig, "#%d", nOrigOrdinal);
#endif
        pszOrigSymbol = szOrig;
    }
    if (pszSymbol == NULL) {
#ifdef _CRT_INSECURE_DEPRECATE
        sprintf_s(szLast, sizeof(szLast), "#%d", nOrdinal);
#else
        sprintf(szLast, "#%d", nOrdinal);
#endif
        pszSymbol = szLast;
    }

    printf("  symbol  %-32.32s %-32.32s\n", pszOrigSymbol, pszSymbol);
    return TRUE;
}

static BOOL CALLBACK ListCommit(PVOID pContext)
{
    (void)pContext;

    printf("  commit\n");
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
struct MUNGE_STATE
{
    BOOL fLastWasByway;
    LONG nBywayCount;
    CHAR szBuffer[512];
};

static BOOL CALLBACK MungeByway(PVOID pContext,
                                PCHAR pszFile,
                                PCHAR *ppszOutFile)
{
    MUNGE_STATE *pState = (MUNGE_STATE *)pContext;

    printf("|");

    if (pState->fLastWasByway) {
        return TRUE;
    }

    pState->fLastWasByway = TRUE;

    if (pszFile == NULL) {
#ifdef _CRT_INSECURE_DEPRECATE
        sprintf_s(pState->szBuffer, sizeof(pState->szBuffer), "mb_munge_%d.dll", pState->nBywayCount++);
#else
        sprintf(pState->szBuffer, "mb_munge_%d.dll", pState->nBywayCount++);
#endif
        *ppszOutFile = pState->szBuffer;
    }
    return TRUE;
}

static BOOL CALLBACK MungeFile(PVOID pContext,
                               PCHAR pszOrigFile,
                               PCHAR pszFile,
                               PCHAR *ppszOutFile)
{
    (void)pszOrigFile;
    MUNGE_STATE *pState = (MUNGE_STATE *)pContext;

    pState->fLastWasByway = FALSE;

    printf("*");
#ifdef _CRT_INSECURE_DEPRECATE
    sprintf_s(pState->szBuffer, sizeof(pState->szBuffer), "mf_%s", pszFile);
#else
    sprintf(pState->szBuffer, "mf_%s", pszFile);
#endif
    *ppszOutFile = pState->szBuffer;
    return TRUE;
}

static BOOL CALLBACK MungeSymbol(PVOID pContext,
                                 ULONG nOrigOrdinal,
                                 ULONG nOrdinal,
                                 ULONG *pnOutOrdinal,
                                 PCHAR pszOrigSymbol,
                                 PCHAR pszSymbol,
                                 PCHAR *ppszOutSymbol)
{
    (void)nOrigOrdinal;
    (void)pszOrigSymbol;
    MUNGE_STATE *pState = (MUNGE_STATE *)pContext;

    pState->fLastWasByway = FALSE;

    printf(".");
    if (nOrdinal != 0) {
        if (s_fToSymbols) {
#ifdef _CRT_INSECURE_DEPRECATE
            sprintf_s(pState->szBuffer, sizeof(pState->szBuffer), "mo_%d", (int)nOrdinal);
#else
            sprintf(pState->szBuffer, "mo_%d", (int)nOrdinal);
#endif
            *pnOutOrdinal = 0;
            *ppszOutSymbol = pState->szBuffer;
        }
        else {
            *pnOutOrdinal = 10000 + nOrdinal;
            *ppszOutSymbol = NULL;
        }
    }
    else {
#ifdef _CRT_INSECURE_DEPRECATE
        sprintf_s(pState->szBuffer, sizeof(pState->szBuffer), "ms_%s", pszSymbol);
#else
        sprintf(pState->szBuffer, "ms_%s", pszSymbol);
#endif
        *pnOutOrdinal = 0;
        *ppszOutSymbol = pState->szBuffer;
    }
    return TRUE;
}

static BOOL CALLBACK MungeCommit(PVOID pContext)
{
    MUNGE_STATE *pState = (MUNGE_STATE *)pContext;

    pState->fLastWasByway = FALSE;

    printf("\n");
    (void)pContext;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
static BOOL CALLBACK RestoreByway(PVOID pContext,
                                  PCHAR pszFile,
                                  PCHAR *ppszOutFile)
{
    (void)pContext;
    (void)pszFile;

    *ppszOutFile = NULL;
    return TRUE;
}

static BOOL CALLBACK RestoreFile(PVOID pContext,
                                 PCHAR pszOrigFile,
                                 PCHAR pszFile,
                                 PCHAR *ppszOutFile)
{
    (void)pContext;
    (void)pszFile;

    *ppszOutFile = pszOrigFile;
    return TRUE;
}

static BOOL CALLBACK RestoreSymbol(PVOID pContext,
                                   ULONG nOrigOrdinal,
                                   ULONG nOrdinal,
                                   ULONG *pnOutOrdinal,
                                   PCHAR pszOrigSymbol,
                                   PCHAR pszSymbol,
                                   PCHAR *ppszOutSymbol)
{
    (void)pContext;
    (void)nOrdinal;
    (void)pszSymbol;

    *pnOutOrdinal = nOrigOrdinal;
    *ppszOutSymbol = pszOrigSymbol;
    return TRUE;
}

static BOOL CALLBACK RestoreCommit(PVOID pContext)
{
    (void)pContext;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//

BOOL EditFile(PCHAR pszInput, PCHAR pszOutput)
{
    BOOL fGood = TRUE;

    HANDLE hOld = INVALID_HANDLE_VALUE;
    HANDLE hNew = INVALID_HANDLE_VALUE;
    PDETOUR_BINARY pBinary = NULL;

    if (pszOutput != NULL) {
        printf("%s -> %s:\n", pszInput, pszOutput);
    }
    else {
        printf("%s:\n", pszInput, pszOutput);
    }

    hOld = CreateFile(pszInput,
                      GENERIC_READ,
                      FILE_SHARE_READ,
                      NULL,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);

    if (hOld == INVALID_HANDLE_VALUE) {
        printf("Couldn't open input file: %s, error: %d\n",
               pszInput, GetLastError());
        fGood = FALSE;
        goto end;
    }

    if ((pBinary = DetourBinaryOpen(hOld)) == NULL) {
        printf("DetourBinaryOpen failed: %d\n", GetLastError());
        goto end;
    }

    if (hOld != INVALID_HANDLE_VALUE) {
        CloseHandle(hOld);
        hOld = INVALID_HANDLE_VALUE;
    }

    if (s_fRestore) {
        if (!DetourBinaryEditImports(pBinary,
                                     NULL,
                                     RestoreByway,
                                     RestoreFile,
                                     RestoreSymbol,
                                     RestoreCommit)) {

            printf("DetourBinaryEditImports for munge failed: %d\n", GetLastError());
        }
    }

    if (s_fMunge) {
        MUNGE_STATE state;
        state.fLastWasByway = FALSE;
        state.nBywayCount = 1;

        if (!DetourBinaryEditImports(pBinary,
                                     &state,
                                     MungeByway,
                                     MungeFile,
                                     MungeSymbol,
                                     MungeCommit)) {

            printf("DetourBinaryEditImports for munge failed: %d\n", GetLastError());
        }
    }

    if (s_fList) {
        if (!DetourBinaryEditImports(pBinary,
                                     NULL,
                                     ListByway,
                                     ListFile,
                                     ListSymbol,
                                     ListCommit)) {

            printf("DetourBinaryEditImports for list failed: %d\n", GetLastError());
        }
    }

    if (pszOutput != NULL) {
        hNew = CreateFile(pszOutput,
                          GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS,
                          FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
        if (hNew == INVALID_HANDLE_VALUE) {
            printf("Couldn't open output file: %s, error: %d\n",
                   pszOutput, GetLastError());
            fGood = FALSE;
            goto end;
        }

        if (!DetourBinaryWrite(pBinary, hNew)) {
            printf("DetourBinaryWrite failed: %d\n", GetLastError());
            fGood = FALSE;
        }

        CloseHandle(hNew);
        hNew = INVALID_HANDLE_VALUE;
    }

    DetourBinaryClose(pBinary);
    pBinary = NULL;


    if (fGood && pszOutput != NULL) {
        if (!BindImageEx(BIND_NO_BOUND_IMPORTS, pszOutput, ".", ".", NULL)) {
            printf("Warning: Couldn't bind binary %s: %d\n", pszOutput, GetLastError());
        }
    }

  end:
    if (pBinary) {
        DetourBinaryClose(pBinary);
        pBinary = NULL;
    }
    if (hNew != INVALID_HANDLE_VALUE) {
        CloseHandle(hNew);
        hNew = INVALID_HANDLE_VALUE;
    }
    if (hOld != INVALID_HANDLE_VALUE) {
        CloseHandle(hOld);
        hOld = INVALID_HANDLE_VALUE;
    }
    return fGood;
}

//////////////////////////////////////////////////////////////////////////////
//
void PrintUsage(void)
{
    printf("Usage:\n"
           "    impmunge [options] binary_files\n"
           "Options:\n"
           "    /l           : List imports.\n"
           "    /l-          : Don't list imports.\n"
           "    /m           : Munge imports.\n"
           "    /r           : Remove import munges.\n"
           "    /o:file      : Set name of output file; must be include with /m or /r.\n"
           "    /?           : This help screen.\n");
}

//////////////////////////////////////////////////////////////////////// main.
//
int CDECL main(int argc, char **argv)
{
    BOOL fNeedHelp = FALSE;
    PCHAR pszOutput = NULL;

    int arg = 1;
    for (; arg < argc && !fNeedHelp; arg++) {
        if (argv[arg][0] == '-' || argv[arg][0] == '/') {
            CHAR *argn = argv[arg] + 1;
            CHAR *argp = argn;
            while (*argp && *argp != ':')
                argp++;
            if (*argp == ':')
                *argp++ = '\0';

            switch (argn[0]) {

              case 'l':                                 // List contents of import table.
              case 'L':
                s_fList = (argn[1] != '-');
                break;

              case 'm':                                 // Munge import table.
              case 'M':
                s_fMunge = (argn[1] != '-');
                break;

              case 'o':                                 // Set output file name.
              case 'O':
                pszOutput = argp;
                break;
              case 'r':                                 // Restore file to unmunged state.
              case 'R':
                s_fRestore = (argn[1] != '-');
                break;

              case 's':                                 // Munge ordinals to symbols
              case 'S':
                s_fToSymbols = true;
                break;

              case '?':                                 // Help
                fNeedHelp = TRUE;
                break;

              default:
                fNeedHelp = TRUE;
                printf("Bad argument: %s:%s\n", argn, argp);
                break;
            }
        }
        else {
            if (!s_fList && !s_fMunge && !s_fRestore) {
                fNeedHelp = TRUE;
                break;
            }
            if (pszOutput == NULL && (s_fMunge || s_fRestore)) {
                fNeedHelp = TRUE;
                break;
            }

            EditFile(argv[arg], pszOutput);
            pszOutput = NULL;
        }
    }
    if (argc == 1) {
        fNeedHelp = TRUE;
    }
    if (fNeedHelp) {
        PrintUsage();
        return 1;
    }
    return 0;
}

// End of File
