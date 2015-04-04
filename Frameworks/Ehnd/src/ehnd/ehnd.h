// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� �����
// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� _EXPORTS ��ȣ��
// �����ϵǸ�, �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������
// EHND_API �Լ��� DLL���� �������� ������ ����, �� DLL��
// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.

#pragma once
#define EHND_EXPORT __declspec(dllexport)

#include <string>

extern "C"
{
  EHND_EXPORT void J2K_Initialize(void);
  EHND_EXPORT void __stdcall J2K_InitializeEx(int data0, LPSTR key);
  EHND_EXPORT void J2K_FreeMem(void);
  EHND_EXPORT void J2K_GetPriorDict(void);
  EHND_EXPORT void J2K_GetProperty(void);
  EHND_EXPORT void __stdcall J2K_ReloadUserDict(void);
  EHND_EXPORT void J2K_SetDelJPN(void);
  EHND_EXPORT void J2K_SetField(void);
  EHND_EXPORT void J2K_SetHnj2han(void);
  EHND_EXPORT void J2K_SetJWin(void);
  EHND_EXPORT void J2K_SetPriorDict(void);
  EHND_EXPORT void J2K_SetProperty(void);
  EHND_EXPORT void J2K_StopTranslation(void);
  EHND_EXPORT void J2K_Terminate(void);
  EHND_EXPORT void J2K_TranslateChat(void);
  EHND_EXPORT void J2K_TranslateFM(void);
  EHND_EXPORT void J2K_TranslateMM(void);
  EHND_EXPORT void J2K_TranslateMMEx(void);
  EHND_EXPORT void *__stdcall J2K_TranslateMMNT(int data0, LPCSTR *szText);
  EHND_EXPORT void *__stdcall J2K_TranslateMMNTW(int data0, LPCWSTR *szText);
  EHND_EXPORT void J2K_GetJ2KMainDir(void);
  EHND_EXPORT void *msvcrt_free(void *_Memory);
  EHND_EXPORT void *msvcrt_malloc(size_t _Size);
  EHND_EXPORT void *msvcrt_fopen(char *path, char *mode);
};

extern FARPROC apfnEzt[100];
extern FARPROC apfnMsv[100];

bool EhndInit();
wstring replace_all(const wstring &str, const wstring &pattern, const wstring &replace);

inline HMODULE GetEztrModule() // Return j2kengine dll handle or 0 if failed
{
  HMODULE h = LoadLibrary(L"j2kengine.dlx");
  if (!h)
    h = LoadLibrary(L"j2kengine.dll");
  return h;
}

// jichi: get enclosing directory path for the dll without trailing '\\'
bool GetModuleDirectory(HMODULE h,LPWSTR buf, int size);
bool GetModuleBaseName(LPWSTR buffer, int size); // jichi 4/4/2015: Get dll file name without suffix and without trailing .'

extern HINSTANCE g_hInst;
inline bool GetLoadPath(LPWSTR buf, int size) // jichi 4/4/2015: Get dll's enclosing directory
{ return GetModuleDirectory(g_hInst, buf, size); }

inline bool GetExecutePath(LPWSTR buf, int size)  // jichi 4/4/2015: Get executable's enclosing directory
{ return GetModuleDirectory(GetModuleHandle(nullptr), buf, size); }

inline bool GetEztrPath(LPWSTR buf, int size) // jichi 4/4/2015: Get ezTrans directory
{
  HMODULE h = GetEztrModule();
  return h && GetModuleDirectory(h, buf, size);
}

std::wstring GetEhndDicPath(); // jichi 4/4/2015: Get ehnd dic directory
