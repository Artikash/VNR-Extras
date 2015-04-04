#include "stdafx.h"
#include "watch.h"

watch *watch::m_pThis = NULL;
std::vector<std::wstring> fileList;

watch::watch()
{
  m_pThis = this;

  SECURITY_ATTRIBUTES ThreadAttributes;
  ThreadAttributes.bInheritHandle = false;
  ThreadAttributes.lpSecurityDescriptor = NULL;
  ThreadAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);

  hWatchThread = CreateThread(&ThreadAttributes, 0, NotifyThread, NULL, 0, NULL);
  if (hWatchThread == NULL)
  {
    WriteLog(ERROR_LOG, L"WatchThread : WatchThread Create Error");
  }
}

watch::~watch()
{
  TerminateThread(hWatchThread, 0);
}

void watch::TurnOn()
{
  bWatch = true;
}

void watch::TurnOff()
{
  bWatch = false;
}

DWORD watch::_NotifyThread(LPVOID lpParam)
{
  HWND hwnd = (HWND)lpParam;
  UINT m_nTimerID;
  WCHAR lpEztPath[MAX_PATH];
  std::wstring Path;
  GetLoadPath(lpEztPath, MAX_PATH);
  Path = lpEztPath;
  Path += L"\\Ehnd";

  WriteLog(NORMAL_LOG, L"watch to %s directory\n", Path.c_str());

  HANDLE hDir = CreateFile(Path.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
    0, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
  CONST DWORD cbBuffer = 1024;
  BYTE pBuffer[1024];
  BOOL bWatchSubtree = FALSE;
  DWORD dwNotifyFilter = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME |
    FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE |
    FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION;
  DWORD bytesReturned;

  m_nTimerID = timeSetEvent(500, 0, (LPTIMECALLBACK)NotifyProc, 0, TIME_PERIODIC);

  wchar_t temp[MAX_PATH] = { 0 };
  for (;;)
  {
    FILE_NOTIFY_INFORMATION* pfni;
    BOOL fOk = ReadDirectoryChangesW(hDir, pBuffer, cbBuffer,
      bWatchSubtree, dwNotifyFilter, &bytesReturned, 0, 0);
    if (!fOk)
    {
      DWORD dwLastError = GetLastError();
      break;
    }
    pfni = (FILE_NOTIFY_INFORMATION*)pBuffer;

    if (m_pThis->bWatch)
    {
      std::vector<std::wstring> fileList_Temp;
      do {
        memcpy(temp, pfni->FileName, pfni->FileNameLength);
        temp[pfni->FileNameLength / 2] = 0;
        std::wstring filename(temp);
        transform(filename.begin(), filename.end(), filename.begin(), tolower);

        if (fileList_Temp.begin() == fileList_Temp.end())
        {
          fileList_Temp.push_back(filename.c_str());
        }

        std::vector<std::wstring>::iterator it = fileList_Temp.begin();
        for (; it != fileList_Temp.end(); it++)
        {
          if (it + 1 == fileList_Temp.end())
          {
            fileList_Temp.push_back(filename.c_str());
            break;
          }
          else if ((*it).compare(filename.c_str()) == 0) break;
        }

        pfni = (FILE_NOTIFY_INFORMATION*)((PBYTE)pfni + pfni->NextEntryOffset);
      } while (pfni->NextEntryOffset > 0);
      fileList = fileList_Temp;
    }
  }
  return 0;
}

MMRESULT watch::_NotifyProc(UINT m_nTimerID, UINT uiMsg, DWORD dwUser, DWORD dw1, DWORD d2)
{
  bool c_prefilter;
  bool c_postfilter;
  bool c_userdic;
  bool c_config;
  bool c_skiplayer;

  c_prefilter = false;
  c_postfilter = false;
  c_userdic = false;
  c_skiplayer = false;

  if (fileList.begin() == fileList.end()) return 0;

  std::vector<std::wstring> fileList_Temp = fileList;
  fileList.clear();

  std::vector<std::wstring>::iterator it = fileList_Temp.begin();
  for (; it != fileList_Temp.end(); it++)
  {
    if ((*it).rfind(L".txt") != -1 && pConfig->GetEhndWatchSwitch())
    {
      if ((*it).find(L"prefilter") != -1)
        c_prefilter = true;
      else if ((*it).find(L"postfilter") != -1)
        c_postfilter = true;
      else if ((*it).find(L"userdic") != -1)
        c_userdic = true;
      else if ((*it).find(L"skiplayer") != -1)
        c_skiplayer = true;
    }
    else if (!(*it).compare(L"ehnd_conf.ini"))
      c_config = true;
  }

  if (c_prefilter == true)
  {
    WriteLog(NORMAL_LOG, L"PreFilter : ��ó�� ���� ���� ������� ����.\n");
    pFilter->pre_load();
  }

  if (c_postfilter == true)
  {
    WriteLog(NORMAL_LOG, L"PostFilter : ��ó�� ���� ���� ������� ����.\n");
    pFilter->post_load();
  }


  if (c_skiplayer == true)
  {
    WriteLog(NORMAL_LOG, L"SkipLayer : ��ŵ ���̾� ���� ������� ����.\n");
    pFilter->skiplayer_load();
  }

  if (c_userdic == true)
  {
    WriteLog(NORMAL_LOG, L"UserDic : ����� ���� ���� ������� ����.\n");
    J2K_ReloadUserDict();
  }

  if (c_config == true)
  {
    WriteLog(NORMAL_LOG, L"Config : �������� ������� ����.\n");
    pConfig->LoadConfig();
  }

  return 0;
}
