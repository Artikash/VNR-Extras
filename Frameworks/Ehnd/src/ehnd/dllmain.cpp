// dllmain.cpp : DLL ���� ���α׷��� �������� �����մϴ�.
#include "stdafx.h"
HINSTANCE g_hInst;
filter *pFilter;
watch *pWatch;
config *pConfig;
HMODULE hEzt, hMsv;
int g_initTick;
char g_DicPath[MAX_PATH];
BOOL initOnce = false;
BOOL g_bAnemone = false;

BOOL APIENTRY DllMain(HINSTANCE hInstance,
  DWORD  ul_reason_for_call,
  LPVOID lpReserved
  )
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
    g_hInst = hInstance;

    // init ehnd
    pFilter = new filter();
    pWatch = new watch();
    pConfig = new config();

    char szInitTick[12];
    g_initTick = GetTickCount() + rand();
    _itoa_s(g_initTick, szInitTick, 10);

    GetTempPathA(MAX_PATH, g_DicPath);
    strcat_s(g_DicPath, "UserDict_");
    strcat_s(g_DicPath, szInitTick);
    strcat_s(g_DicPath, ".ehnd");

    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  case DLL_PROCESS_DETACH:
    FreeLibrary(hEzt);
    FreeLibrary(hMsv);
    break;
  }
  return TRUE;
}
