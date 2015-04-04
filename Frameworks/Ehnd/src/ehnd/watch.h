#pragma once
class watch
{
public:
  watch();
  ~watch();
  void TurnOn();
  void TurnOff();
private:
  static watch *m_pThis;
  HANDLE hWatchThread;

  bool bWatch = true;

  DWORD _NotifyThread(LPVOID lpParam);
  MMRESULT _NotifyProc(UINT m_nTimerID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD d2);

  // Trampoline
  static DWORD WINAPI NotifyThread(LPVOID lpParam)
  {
    return m_pThis->_NotifyThread(lpParam);
  }

  static MMRESULT CALLBACK NotifyProc(UINT m_nTimerID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD d2)
  {
    return m_pThis->_NotifyProc(m_nTimerID, uiMsg, dwUser, dw1, d2);
  }

};


