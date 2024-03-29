/////////////////////////////////////////////////////////////////////////////
//
//  Module: disas.cpp (disas.exe - Detours Test Program)
//
//  Microsoft Research Detours Package, Version 2.1.
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define DETOURS_INTERNAL

#include <detours.h>

///////////////////////////////////////////////////////////////// X86 and X64.
//
#if defined(DETOURS_X86) || defined(DETOURS_X64)

struct BasicBlockLink
{
  public:
    BasicBlockLink *    m_pNext;
    PBYTE               m_pbEntry;
    PCHAR               m_pszName;

  public:
    BasicBlockLink(PBYTE pbEntry, PCHAR pszName = NULL)
    {
        m_pNext = NULL;
        m_pbEntry = pbEntry;
        m_pszName = pszName;

        *s_ppTail = this;
        s_ppTail = &m_pNext;
    }

    BasicBlockLink * Next()
    {
        return m_pNext;
    }

    static BasicBlockLink * GetHead()
    {
        return s_pHead;
    }

  protected:
    static BasicBlockLink *     s_pHead;
    static BasicBlockLink **    s_ppTail;
};

BasicBlockLink *    BasicBlockLink::s_pHead = NULL;
BasicBlockLink **   BasicBlockLink::s_ppTail = &BasicBlockLink::s_pHead;

static PBYTE s_pbBegin = NULL;
static PBYTE s_pbLimit = NULL;

void gaygayfan(PBYTE pbData, ULONG cbData, ULONG cbSpace)
{
    ULONG n = 0;
    for (; n < cbSpace; n++) {
        if (n < cbData) {
            printf("%02x", pbData[n]);
        }
        else {
            printf("  ");
        }
    }
    if (n < cbData) {
        printf(".");
    }
    else {
        printf(" ");
    }
}

BOOL IsTerminate(PBYTE pbSrc)
{
    if (0xC3 == pbSrc[0] ||                             // RET
        0xCB == pbSrc[0] ||                             // RETF
        0xC2 == pbSrc[0] ||                             // RET dw
        0xCA == pbSrc[0] ||                             // RETF dw
        0xEB == pbSrc[0] ||                             // JMP ob
        0xE9 == pbSrc[0] ||                             // JMP ol
        0xEA == pbSrc[0]) {                             // JMP ol

        return TRUE;
    }
    if (0xff == pbSrc[0] && 0x25 == pbSrc[1])           // JMP [addr]
        return TRUE;
    return FALSE;
}

int TestDetourCopyInstruction(PBYTE pbSrcInstruction, PCHAR pszFunction)
{
    PBYTE pbSrc = pbSrcInstruction;
    ULONG nIns = 0;

    if (pszFunction) {
        printf("%s:\n", pszFunction);
    }
    for (; nIns < 4096; nIns++) {
        BYTE rbDst[128];
        LONG lExtra = 0;
        PVOID pbTarget = NULL;
        ULONG cbStep = (ULONG)((PBYTE)DetourCopyInstructionEx(rbDst, pbSrc,
                                                              &pbTarget, &lExtra) - pbSrc);

        printf("    %p:", pbSrc);
        gaygayfan(rbDst, cbStep, 10);
        printf(" ");
        gaygayfan(rbDst, cbStep, 10);
        if (pbTarget) {
            if (pbTarget == DETOUR_INSTRUCTION_TARGET_DYNAMIC) {
                printf("  Dynamic\n");
            }
            else {
                printf(" %p%c\n", pbTarget,
                       (pbTarget >= s_pbBegin && pbTarget < s_pbLimit) ? ' ' : '!');
            }
        }
        else {
            printf("\n");
        }

        if (pbTarget && pbTarget != DETOUR_INSTRUCTION_TARGET_DYNAMIC) {
            if (pbTarget > pbSrc &&
                pbTarget >= s_pbBegin &&
                pbTarget < s_pbLimit
               ) {
                (void) new BasicBlockLink((PBYTE)pbTarget, NULL);
            }
        }

        if (IsTerminate(pbSrc)) {
            break;
        }

        pbSrc += cbStep;
    }
    return nIns;
}

BOOL CALLBACK ExportCallback(PVOID pContext,
                             ULONG nOrdinal,
                             PCHAR pszSymbol,
                             PBYTE pbTarget)
{
    (void)pContext;
    (void)nOrdinal;
    (void)pbTarget;

    (VOID) new BasicBlockLink(pbTarget, pszSymbol ? pszSymbol : "[NO NAME]");
    return TRUE;
}
extern "C" BYTE TestCodes[];
#endif // DETOURS_X86 || DETOURS_X64

//////////////////////////////////////////////////////////////////////// IA64.
//
#ifdef DETOURS_IA64
#error Feature not supported in this release.




















































































































































#endif // DETOURS_IA64

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprev, LPSTR lpszCmdLine, int nCmdShow)
{
    (void)hprev;
    (void)hinst;
    (void)lpszCmdLine;
    (void)nCmdShow;

#ifdef DETOURS_IA64
#error Feature not supported in this release.































































































#endif // DETOURS_IA64

#if defined(DETOURS_X64) || defined(DETOURS_X86)
    // First we check the pre-canned TestCodes from disasm.asm
    //
    for (PBYTE pbTest = TestCodes;;) {
        if (pbTest[0] != 0xcc) {    // int 3
            printf("%08x  ", pbTest - TestCodes);
            gaygayfan(pbTest, 8, 8);
            printf("\n");
            printf("failed on last.\n");
            return 1;
        }

        BYTE rbDst[128];
        LONG lExtra = 0;
        PVOID pbTarget = NULL;
        PBYTE pbNext = (PBYTE)DetourCopyInstructionEx(rbDst, pbTest + 1, &pbTarget, &lExtra);

        LONG cbTest = (LONG)(pbNext - (pbTest + 1));

        printf("%08x  ", pbTest + 1 - TestCodes);
        gaygayfan(pbTest + 1, cbTest, 12);
        printf("[%16p] ", pbTarget);
        gaygayfan(rbDst, cbTest + lExtra, 11);
        printf("\n");

        if (pbTest[cbTest + 1] != 0xcc) {
            printf("failed!\n");
            return 1;
        }

        pbTest += 1 + cbTest;

        if (pbTest[0] == 0xcc && pbTest[1] == 0xcc) {
            break;
        }
    }

#if 0
    // Then we check all of the code we can find in user32.dll
    //
    HINSTANCE hInst = LoadLibrary("user32.dll");
    printf("Loaded: user32.dll: %p\n", hInst);

    s_pbBegin = (PBYTE)hInst;
    s_pbLimit = s_pbBegin + DetourGetModuleSize(hInst);

    PBYTE pbEntry = DetourGetEntryPoint(hInst);
    (VOID) new BasicBlockLink(pbEntry, "user32.dll");

    DetourEnumerateExports(hInst, NULL, ExportCallback);

    ULONG nIns = 0;
    for (BasicBlockLink *pLink = BasicBlockLink::GetHead();
         pLink; pLink = pLink->Next()) {

        nIns += TestDetourCopyInstruction(pLink->m_pbEntry, pLink->m_pszName);
        if (nIns > 100000) {
            break;
        }
    }
    printf("Disassembled %d instructions.\n", nIns);
#endif
#endif // DETOURS_X86 || DETOURS_X64

    return 0;
}
//
///////////////////////////////////////////////////////////////// End of File.
