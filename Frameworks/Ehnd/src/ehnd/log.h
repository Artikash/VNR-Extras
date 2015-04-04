#pragma once
#include <Richedit.h>

void WriteLog(int LogType, const wchar_t *format, ...);
void WriteTextLog(const wchar_t* format, ...);

void LogStartMsg();
void CheckLogSize();
void CheckConsoleLine();

bool CreateLogWin(HINSTANCE);
void SetLogText(LPCWSTR);
void SetLogText(LPCWSTR, COLORREF, COLORREF);
void ClearLog(void);
void ShowLogWin(bool bShow);
bool IsShownLogWin(void);
DWORD WINAPI LogThreadMain(LPVOID lpParam);
LRESULT CALLBACK LogProc(HWND, UINT, WPARAM, LPARAM);
extern HWND hLogRes, hLogWin;
extern int logLine;
