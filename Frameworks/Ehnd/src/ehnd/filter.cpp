#include "stdafx.h"
#include "filter.h"

filter::filter()
{
  hLoadEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
}


filter::~filter()
{
  CloseHandle(hLoadEvent);
}

bool filter::load()
{
  WaitForSingleObject(hLoadEvent, INFINITE);
  if (!pre_load() || !post_load() ||
    !userdic_load() || skiplayer_load())
  {
    SetEvent(hLoadEvent);
    return false;
  }
  SetEvent(hLoadEvent);
  return true;
}

bool filter::load_dic()
{
  WaitForSingleObject(hLoadEvent, INFINITE);
  if (!userdic_load())
  {
    SetEvent(hLoadEvent);
    return false;
  }
  SetEvent(hLoadEvent);
  return true;
}

bool filter::pre_load()
{
  WIN32_FIND_DATA FindFileData;
  FILTERSTRUCT fs;

  DWORD dwStart, dwEnd;
  dwStart = GetTickCount();

  // jichi 4/4/2015: Do not hardcode ehnd directory name
  std::wstring dicpath = GetEhndDicPath() + L"\\",
               Path = dicpath + L"PreFilter*.txt";

  int pre_line = 1;

  vector<FILTERSTRUCT> Filter;

  HANDLE hFind = FindFirstFile(Path.c_str(), &FindFileData);

  do
  {
    if (hFind == INVALID_HANDLE_VALUE) break;
    else if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      continue;

    // jichi 4/4/2015: Do not hardcode ehnd directory name
    Path = dicpath;

    filter_load(Filter, Path.c_str(), FindFileData.cFileName, PREFILTER, pre_line);

  } while (FindNextFile(hFind, &FindFileData));

  // ����
  sort(Filter.begin(), Filter.end());
  //WriteLog(NORMAL_LOG, L"PreFilterRead : ���� ������ �Ϸ��߽��ϴ�.\n");

  // ���� ��ü
  PreFilter = Filter;
  WriteLog(NORMAL_LOG, L"PreFilterRead : �� %d���� ��ó�� ���͸� �о����ϴ�.\n", PreFilter.size());

  // �ҿ�ð� ���
  dwEnd = GetTickCount();
  WriteLog(TIME_LOG, L"PreFilterRead : --- Elasped Time : %dms ---\n", dwEnd - dwStart);
  return true;
}

bool filter::post_load()
{
  WIN32_FIND_DATA FindFileData;
  FILTERSTRUCT fs;

  DWORD dwStart, dwEnd;
  dwStart = GetTickCount();

  // jichi 4/4/2015: Do not hardcode ehnd directory name
  std::wstring dicpath = GetEhndDicPath() + L"\\",
               Path = dicpath + L"PostFilter*.txt";

  int post_line = 1;

  vector<FILTERSTRUCT> Filter;

  HANDLE hFind = FindFirstFile(Path.c_str(), &FindFileData);

  do
  {
    if (hFind == INVALID_HANDLE_VALUE) break;
    else if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      continue;

    // jichi 4/4/2015: Do not hardcode ehnd directory name
    Path = dicpath;

    filter_load(Filter, Path.c_str(), FindFileData.cFileName, POSTFILTER, post_line);

  } while (FindNextFile(hFind, &FindFileData));

  // ����
  sort(Filter.begin(), Filter.end());
  //WriteLog(NORMAL_LOG, L"PostFilterRead : ���� ������ �Ϸ��߽��ϴ�.\n");

  // ���� ��ü
  PostFilter = Filter;
  WriteLog(NORMAL_LOG, L"PostFilterRead : �� %d���� ��ó�� ���͸� �о����ϴ�.\n", PostFilter.size());

  // �ҿ�ð� ���
  dwEnd = GetTickCount();
  WriteLog(TIME_LOG, L"PostFilterRead : --- Elasped Time : %dms ---\n", dwEnd - dwStart);
  return true;
}

bool filter::skiplayer_load()
{
  WIN32_FIND_DATA FindFileData;
  FILTERSTRUCT fs;

  DWORD dwStart, dwEnd;
  dwStart = GetTickCount();

  // jichi 4/4/2015: Do not hardcode ehnd directory name
  std::wstring dicpath = GetEhndDicPath() + L"\\",
               Path = dicpath + L"SkipLayer*.txt";

  int skiplayer_line = 1;

  vector<SKIPLAYERSTRUCT> _SkipLayer;

  HANDLE hFind = FindFirstFile(Path.c_str(), &FindFileData);

  do
  {
    if (hFind == INVALID_HANDLE_VALUE) break;
    else if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      continue;

    // jichi 4/4/2015: Do not hardcode ehnd directory name
    Path = dicpath;

    skiplayer_load2(_SkipLayer, Path.c_str(), FindFileData.cFileName, skiplayer_line);

  } while (FindNextFile(hFind, &FindFileData));

  // ����
  sort(_SkipLayer.begin(), _SkipLayer.end());
  //WriteLog(NORMAL_LOG, L"SkipLayerRead : ��ŵ���̾� ������ �Ϸ��߽��ϴ�.\n");

  // ��ŵ���̾� ��ü
  SkipLayer = _SkipLayer;
  WriteLog(NORMAL_LOG, L"SkipLayerRead : �� %d���� ��ŵ���̾ �о����ϴ�.\n", SkipLayer.size());

  // �ҿ�ð� ���
  dwEnd = GetTickCount();
  WriteLog(TIME_LOG, L"SkipLayerRead : --- Elasped Time : %dms ---\n", dwEnd - dwStart);
  return true;
}
bool filter::userdic_load()
{
  WIN32_FIND_DATA FindFileData;

  DWORD dwStart, dwEnd;
  dwStart = GetTickCount();

  // jichi 4/4/2015: Do not hardcode ehnd directory name
  std::wstring dicpath = GetEhndDicPath() + L"\\",
               Path = dicpath + L"UserDict*.txt";

  int userdic_line = 1;
  UserDic.clear();

  // load userdict.jk
  if (pConfig->GetJKDICSwitch()) jkdic_load(userdic_line);

  // load anedic.txt
  anedic_load(userdic_line);

  HANDLE hFind = FindFirstFile(Path.c_str(), &FindFileData);

  do
  {
    if (hFind == INVALID_HANDLE_VALUE) break;
    else if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      continue;

    // jichi 4/4/2015: Do not hardcode ehnd directory name
    Path = dicpath;

    userdic_load2(Path.c_str(), FindFileData.cFileName, userdic_line);

  } while (FindNextFile(hFind, &FindFileData));

  // ����
  sort(UserDic.begin(), UserDic.end());
  //WriteLog(NORMAL_LOG, L"UserDicRead : ����� ���� ������ �Ϸ��߽��ϴ�.\n");

  WriteLog(NORMAL_LOG, L"UserDicRead : �� %d���� ����� ���� ������ �о����ϴ�.\n", UserDic.size());

  // �ҿ�ð� ���
  dwEnd = GetTickCount();
  WriteLog(TIME_LOG, L"UserDicRead : --- Elasped Time : %dms ---\n", dwEnd - dwStart);

  // ���� �ӽ����� ����
  ehnddic_create();
  return true;
}

bool filter::jkdic_load(int &g_line)
{
  WCHAR lpEztPath[MAX_PATH];
  CHAR Jpn[32], Kor[32], Part[6], Attr[38], Hidden;
  CHAR Buffer[1024];
  FILE *fp;
  int line = 1;
  wchar_t lpBuffer[128];

  DWORD dwStart, dwEnd;
  dwStart = GetTickCount();

  // jichi 4/4/2015: Use eztr directory
  GetEztrPath(lpEztPath, MAX_PATH);
  std::wstring Path = lpEztPath;
  Path += L"\\Dat\\UserDict.jk";

  if (_wfopen_s(&fp, Path.c_str(), L"rb") != 0)
  {
    WriteLog(NORMAL_LOG, L"JkDicLoad : DAT ����� ���� ���� \"UserDict.jk\"�� �����ϴ�.\n");

    // userdict.jk ������ ������ �� ���� ����
    if (_wfopen_s(&fp, Path.c_str(), L"wb") == 0) fclose(fp);
    return false;
  }

  //
  // ezTrans XP UserDict.jk Struct
  // [0] hidden (1 byte)
  // [1-31] jpn (31 bytes)
  // [32-62] kor (31 bytes)
  // [63-67] part of speech (5 bytes)
  // [68-109] attributes (42 bytes)
  //
  USERDICSTRUCT us;
  int vaild_line = 0;
  while (1)
  {
    Jpn[31] = 0, Kor[31] = 0, Part[5] = 0, Attr[37] = 0;

    if (!fread(Buffer, sizeof(char), 1, fp)) break;
    else (Buffer[0] == 0x00) ? Hidden = false : Hidden = true;

    if (!fread(Buffer, sizeof(char), 31, fp)) break;
    else memcpy(Jpn, Buffer, 31);

    if (!fread(Buffer, sizeof(char), 31, fp)) break;
    else memcpy(Kor, Buffer, 31);

    if (!fread(Buffer, sizeof(char), 5, fp)) break;
    else memcpy(Part, Buffer, 5);

    if (!fread(Buffer, sizeof(char), 37, fp)) break;
    else memcpy(Attr, Buffer, 37);
    memcpy(Attr+37, L"", 1);

    if (!fread(Buffer, sizeof(char), 5, fp)) break;

    int len;
    len = _MultiByteToWideChar(932, MB_PRECOMPOSED, Jpn, -1, NULL, NULL);
    _MultiByteToWideChar(932, 0, Jpn, -1, lpBuffer, len);
    wcsncpy_s(us._jpn, lpBuffer, len);

    len = _MultiByteToWideChar(949, MB_PRECOMPOSED, Kor, -1, NULL, NULL);
    _MultiByteToWideChar(949, 0, Kor, -1, lpBuffer, len);
    wcsncpy_s(us._kor, lpBuffer, len);

    len = _MultiByteToWideChar(949, MB_PRECOMPOSED, Attr, -1, NULL, NULL);
    _MultiByteToWideChar(949, 0, Attr, -1, lpBuffer, len);
    wcsncpy_s(us._attr, lpBuffer, len);

    wcscpy_s(us._db, L"UserDict.jk");

    if (!strcmp(Part, "A9D0"))
      us._type = USERDIC_COMM;
    else
      us._type = USERDIC_NOUN;

    us.g_line = g_line;
    us.line = line;

    if (!Hidden) UserDic.push_back(us);
    line++, g_line++;
  }
  fclose(fp);

  WriteLog(NORMAL_LOG, L"JkDicRead : %d���� DAT ����� ���� \"UserDict.jk\"�� �о����ϴ�.\n", line-1);

  // �ҿ�ð� ���
  dwEnd = GetTickCount();
  WriteLog(TIME_LOG, L"JkDicRead : --- Elasped Time : %dms ---\n", dwEnd - dwStart);
  return true;
}

bool filter::ehnddic_cleanup()
{
  WCHAR lpTmpPath[MAX_PATH];
  WIN32_FIND_DATA FindFileData;

  DWORD dwStart, dwEnd;
  dwStart = GetTickCount();

  GetTempPath(MAX_PATH, lpTmpPath);
  std::wstring Path = lpTmpPath;
  if (Path[-1] != '\\')
    Path.push_back('\\');
  Path += L"UserDict*.ehnd";

  HANDLE hFind = FindFirstFile(Path.c_str(), &FindFileData);

  do
  {
    WriteLog(NORMAL_LOG, L"EhndDicCleanUp : %s\n", FindFileData.cFileName);
    if (hFind == INVALID_HANDLE_VALUE) break;
    else if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      continue;

    int i = wcslen(FindFileData.cFileName) - 1;
    while (i--)
    {
      if (FindFileData.cFileName[i] == L'.' &&
        !wcscmp(FindFileData.cFileName + i, L".ehnd"))
      {
        wstring DelFile = lpTmpPath;
        DelFile += FindFileData.cFileName;
        DeleteFile(DelFile.c_str());
        break;
      }
    }

  } while (FindNextFile(hFind, &FindFileData));

  // �ҿ�ð� ���
  dwEnd = GetTickCount();
  WriteLog(TIME_LOG, L"EhndDicCleanUp : --- Elasped Time : %dms ---\n", dwEnd - dwStart);

  return true;
}

bool filter::ehnddic_create()
{
  WCHAR lpTmpPath[MAX_PATH], lpText[12];
  CHAR Jpn[32], Kor[32], Part[6], Attr[38];
  FILE *fp;
  DWORD dwStart, dwEnd;
  _itow_s(g_initTick, lpText, 10);
  dwStart = GetTickCount();

  GetTempPath(MAX_PATH, lpTmpPath);
  std::wstring Path = lpTmpPath;
  if (Path[-1] != '\\')
    Path.push_back('\\');
  Path += L"UserDict_";
  Path += lpText;
  Path += L".ehnd";

  if (_wfopen_s(&fp, Path.c_str(), L"wb") != 0)
    WriteLog(NORMAL_LOG, L"EhndDicCreate : ����ڻ��� ���̳ʸ� \"UserDict_%s.ehnd\" ������ �����ϴµ� �����߽��ϴ�.\n", lpText);

  for (UINT i = 0; i < UserDic.size(); i++)
  {
    // ���� ����
    if (UserDic[i]._jpn == 0) continue;

    memset(Jpn, 0, sizeof(Jpn));
    memset(Kor, 0, sizeof(Kor));
    memset(Attr, 0, sizeof(Attr));

    int len;

    // �����ڵ� -> 932/949
    len = _WideCharToMultiByte(932, 0, UserDic[i]._jpn, -1, NULL, NULL, NULL, NULL);
    _WideCharToMultiByte(932, 0, UserDic[i]._jpn, -1, Jpn, len, NULL, NULL);
    len = _WideCharToMultiByte(949, 0, UserDic[i]._kor, -1, NULL, NULL, NULL, NULL);
    _WideCharToMultiByte(949, 0, UserDic[i]._kor, -1, Kor, len, NULL, NULL);
    len = _WideCharToMultiByte(949, 0, UserDic[i]._attr, -1, NULL, NULL, NULL, NULL);
    _WideCharToMultiByte(949, 0, UserDic[i]._attr, -1, Attr, len, NULL, NULL);

    // �ܾ� Ÿ��
    if (UserDic[i]._type == USERDIC_COMM)
      strcpy_s(Part, "A9D0");
    else strcpy_s(Part, "I110");

    fwrite(L"", sizeof(char), 1, fp);
    fwrite(Jpn, sizeof(char), 31, fp);
    fwrite(Kor, sizeof(char), 31, fp);
    fwrite(Part, sizeof(char), 5, fp);
    fwrite(Attr, sizeof(char), 37, fp);

    // UserDic_Log ������ ���� NULL+attr �� ���ڸ��� Ȱ��
    // ���� attr �迭�� 42�ڱ��� ���Ǿ����� ���ڸ��� ����ϴ� ����� 36�ڱ����� ��� (1�ڴ� ����)
    fwrite(L"", sizeof(char), 1, fp);
    fwrite(&i, sizeof(char), 4, fp);
  }
  WriteLog(NORMAL_LOG, L"EhndDicCreate : ����ڻ��� ���̳ʸ� \"UserDict_%s.ehnd\" ����.\n", lpText);
  fclose(fp);

  // �ҿ�ð� ���
  dwEnd = GetTickCount();
  WriteLog(TIME_LOG, L"EhndDicCreate : --- Elasped Time : %dms ---\n", dwEnd - dwStart);
  return true;
}

bool filter::skiplayer_load2(vector<SKIPLAYERSTRUCT> &SkipLayer, LPCWSTR lpPath, LPCWSTR lpFileName, int &g_line)
{
  WCHAR Buffer[1024], Context[1024];
  FILE *fp;

  wstring Path;
  Path = lpPath;
  Path += lpFileName;
  int vaild_line = 0;

  if (_wfopen_s(&fp, Path.c_str(), L"rt,ccs=UTF-8") != 0)
  {
    WriteLog(NORMAL_LOG, L"SkipLayerRead : ��ŵ���̾� %s �ε� ����.\n", lpFileName);
    return false;
  }
  //WriteLog(NORMAL_LOG, L"SkipLayerRead : ��ŵ���̾� %s �ε�.\n", lpFileName);

  for (int line = 0; fgetws(Buffer, 1000, fp) != NULL; line++, g_line++)
  {
    int tab = 0;
    if (Buffer[0] == L'/' && Buffer[1] == L'/') continue;

    SKIPLAYERSTRUCT ss;
    ss.line = line;

    for (UINT i = 0, prev = 0; i <= wcslen(Buffer) + 1; i++)
    {
      if (Buffer[i] == L'\t' || Buffer[i] == L'\n' || (Buffer[i] == L'/' && Buffer[i - 1] == L'/') || i == wcslen(Buffer))
      {
        switch (tab)
        {
        case 0:
          wcsncpy_s(Context, Buffer + prev, i - prev);
          tab++;
          prev = i + 1;
          ss.wtype = Context;
          if (!wcsncmp(Context, L"PRE", 3)) ss.type = 1;
          else if (!wcsncmp(Context, L"POST", 4)) ss.type = 2;
          else i = wcslen(Buffer) + 1;
          break;
        case 1:
          wcsncpy_s(Context, Buffer + prev, i - prev);
          tab++;
          prev = i + 1;
          ss.layer = _wtoi(Context);
          ss.wlayer = Context;
          break;
        case 2:
          wcsncpy_s(Context, Buffer + prev, i - prev);
          tab++;
          prev = i + 1;
          ss.cond = Context;
          break;
        }
      }
    }
    if (tab < 2) continue;

    try
    {
      wregex ex(ss.cond);
    }
    catch (regex_error ex)
    {
      WCHAR lpWhat[255];
      int len = _MultiByteToWideChar(949, MB_PRECOMPOSED, ex.what(), -1, NULL, NULL);
      _MultiByteToWideChar(949, MB_PRECOMPOSED, ex.what(), -1, lpWhat, len);

      WriteLog(ERROR_LOG, L"SkipLayerRead : ���Խ� ����! : [%s:%d] %s | %s | %s\n", L"SkipLayer.txt", line, ss.wtype, ss.wlayer, ss.cond);

      continue;
    }
    vaild_line++;
    SkipLayer.push_back(ss);
  }
  fclose(fp);
  WriteLog(NORMAL_LOG, L"SkipLayerRead : %d���� ��ŵ���̾� %s�� �о����ϴ�.\n", vaild_line, lpFileName);
  return true;
}

bool filter::filter_load(vector<FILTERSTRUCT> &Filter, LPCWSTR lpPath, LPCWSTR lpFileName, int FilterType, int &g_line)
{
  FILE *fp;
  WCHAR Buffer[1024], Context[1024];

  wstring Path;
  Path = lpPath;
  Path += lpFileName;
  int vaild_line = 0;

  if (_wfopen_s(&fp, Path.c_str(), L"rt,ccs=UTF-8") != 0)
  {
    if (FilterType == PREFILTER) WriteLog(NORMAL_LOG, L"PreFilterRead : ��ó�� ���� '%s' �ε� ����!\n", lpFileName);
    if (FilterType == POSTFILTER) WriteLog(NORMAL_LOG, L"PostFilterRead : ��ó�� ���� '%s' �ε� ����!\n", lpFileName);
    return false;
  }

  /*
  if (FilterType == PREFILTER && IsUnicode) WriteLog(NORMAL_LOG, L"PreFilterRead : ��ó�� �����ڵ� ���� ���� \"%s\" �ε�.\n", lpFileName);
  if (FilterType == PREFILTER && !IsUnicode) WriteLog(NORMAL_LOG, L"PreFilterRead : ��ó�� ���� \"%s\" �ε�.\n", lpFileName);
  else if (FilterType == POSTFILTER && IsUnicode) WriteLog(NORMAL_LOG, L"PostFilterRead : ��ó�� �����ڵ� ���� ���� \"%s\" �ε�.\n", lpFileName);
  else if (FilterType == POSTFILTER && !IsUnicode) WriteLog(NORMAL_LOG, L"PostFilterRead : ��ó�� ���� \"%s\" �ε�.\n", lpFileName);
  */
  for (int line = 0; fgetws(Buffer, 1000, fp) != NULL; line++, g_line++)
  {
    if (Buffer[0] == L'/' && Buffer[1] == L'/') continue;  // �ּ�

    FILTERSTRUCT fs;
    fs.g_line = g_line;
    fs.line = line;
    fs.db = lpFileName;
    fs.src = L"";
    fs.dest = L"";

    int tab = 0;
    for (UINT i = 0, prev = 0; i <= wcslen(Buffer); i++)
    {
      if (Buffer[i] == L'\t' || Buffer[i] == L'\n' || (Buffer[i] == L'/' && Buffer[i - 1] == L'/') || i == wcslen(Buffer))
      {
        switch (tab)
        {
        case 0:
          wcsncpy_s(Context, Buffer + prev, i - prev);
          prev = i + 1;
          tab++;
          fs.src = Context;
          break;
        case 1:
          wcsncpy_s(Context, Buffer + prev, i - prev);
          prev = i + 1;
          tab++;
          fs.dest = Context;
          break;
        case 2:
          wcsncpy_s(Context, Buffer + prev, i - prev);
          prev = i + 1;
          tab++;
          fs.layer = _wtoi(Context);
          break;
        case 3:
          wcsncpy_s(Context, Buffer + prev, i - prev);
          prev = i + 1;
          tab++;
          fs.regex = _wtoi(Context);
          break;
        }
        if (Buffer[i] == L'/' && Buffer[i - 1] == L'/') break;
      }
    }

    if (tab < 3) continue;
    if (fs.regex == 1)
    {
      try
      {
        wregex ex(fs.src);
      }
      catch (regex_error ex)
      {
        WCHAR lpWhat[255];
        int len = _MultiByteToWideChar(949, MB_PRECOMPOSED, ex.what(), -1, NULL, NULL);
        _MultiByteToWideChar(949, MB_PRECOMPOSED, ex.what(), -1, lpWhat, len);

        if (FilterType == 1) WriteLog(ERROR_LOG, L"PreFilterRead : ���Խ� ����! : [%s:%d] %s | %s | %d | %d\n", lpFileName, line, fs.src.c_str(), fs.dest.c_str(), fs.layer, fs.regex);
        else if (FilterType == 2) WriteLog(ERROR_LOG, L"PostFilterRead : ���Խ� ����! : [%s:%d] %s | %s | %d | %d\n", lpFileName, line, fs.src.c_str(), fs.dest.c_str(), fs.layer, fs.regex);
        continue;
      }
    }
    vaild_line++;
    Filter.push_back(fs);
  }
  fclose(fp);
  if (FilterType == PREFILTER) WriteLog(NORMAL_LOG, L"PreFilterRead : %d���� ��ó�� ���� \"%s\"�� �о����ϴ�.\n", vaild_line, lpFileName);
  else if (FilterType == POSTFILTER) WriteLog(NORMAL_LOG, L"PostFilterRead : %d���� ��ó�� ���� \"%s\"�� �о����ϴ�.\n", vaild_line, lpFileName);
  return true;
}

bool filter::userdic_load2(LPCWSTR lpPath, LPCWSTR lpFileName, int &g_line)
{
  FILE *fp;
  WCHAR Buffer[1024], Context[1024];
  wstring Path, Jpn, Kor, Attr;
  int count, len;
  Path = lpPath;
  Path += lpFileName;

  if (_wfopen_s(&fp, Path.c_str(), L"rt,ccs=UTF-8") != 0)
  {
    WriteLog(NORMAL_LOG, L"UserDicRead : ����� ���� '%s' �ε� ����!\n", lpFileName);
    return false;
  }

  //WriteLog(NORMAL_LOG, L"UserDicRead : ����� ���� \"%s\" �ε�.\n", lpFileName);

  count = 0;
  for (int line = 1; fgetws(Buffer, 1000, fp) != NULL; line++)
  {
    if (!wcsncmp(Buffer, L"//", 2)) continue;
    if (Buffer[wcslen(Buffer) - 1] == 0x0A) Buffer[wcslen(Buffer) - 1] = 0;
    if (Buffer[wcslen(Buffer) - 1] == 0x0D) Buffer[wcslen(Buffer) - 1] = 0;

    USERDICSTRUCT us;
    memset(us._jpn, 0, sizeof(us._jpn));
    memset(us._kor, 0, sizeof(us._kor));
    memset(us._attr, 0, sizeof(us._attr));
    memset(us._db, 0, sizeof(us._db));

    us._type = USERDIC_NOUN;

    int t = 0, n = 0;
    wchar_t *pStr = Buffer;
    while (*pStr != 0)
    {
      if (*pStr == L'\t' || (!wcsncmp(pStr, L"//", 2)) || *(pStr + 1) == 0)
      {
        if (*(pStr + 1) == 0)
          wcsncpy_s(Context, pStr - n, n+1);
        else
          wcsncpy_s(Context, pStr - n, n);

        if (t < 4)
        {
          if (t == 0)
            Jpn = Context;
          else if (t == 1)
            Kor = Context;
          else if (t == 2)
            if ((Context[0] == L'0' || Context[0] == L'2') && Context[1] == 0) us._type = USERDIC_COMM;
            else us._type = USERDIC_NOUN;
          else if (t == 3)
            Attr = Context;

          if (!wcsncmp(pStr, L"//", 2)) break;
        }

        pStr++, n = 0, t++;
      }
      else
        pStr++, n++;
    }

    if (t > 1)
    {
      if ((len = _WideCharToMultiByte(932, 0, Jpn.c_str(), -1, NULL, NULL, NULL, NULL)) > 31)
      {
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : ���� �ܾ��� ���̴� 15��(30Byte)�� �ʰ��� �� �����ϴ�.\n");
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : ���� �ܾ ���õ˴ϴ�. (����: %dByte)\n", len);
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : [%s:%d] : <<%s>> | %s | %x | %s\n", lpFileName, line, Jpn.c_str(), Kor.c_str(), us._type, Attr.c_str());
        t = 0; break;
      }
      wcscpy_s(us._jpn, Jpn.c_str());
      if ((len = _WideCharToMultiByte(949, 0, Kor.c_str(), -1, NULL, NULL, NULL, NULL)) > 31)
      {
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : ���� �ܾ��� ���̴� 15��(30Byte)�� �ʰ��� �� �����ϴ�.\n");
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : ���� �ܾ ���õ˴ϴ�. (����: %dByte)\n", len);
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : [%s:%d] : %s | <<%s>> | %x | %s\n", lpFileName, line, Jpn.c_str(), Kor.c_str(), us._type, Attr.c_str());
        t = 0; break;
      }
      wcscpy_s(us._kor, Kor.c_str());
      if ((len = _WideCharToMultiByte(949, 0, Attr.c_str(), -1, NULL, NULL, NULL, NULL)) > 37)
      {
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : �ܾ� �Ӽ��� 36Byte�� �ʰ��� �� �����ϴ�.\n");
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : ���� �ܾ ���õ˴ϴ�. (����: %dByte)\n", len);
        WriteLog(NORMAL_LOG, L"UserDicRead : ���� : [%s:%d] : %s | %s | %x | <<%s>>\n", lpFileName, line, Jpn.c_str(), Kor.c_str(), us._type, Attr.c_str());
        t = 0; break;
      }
      wcscpy_s(us._attr, Attr.c_str());

      wcscpy_s(us._db, lpFileName);
      us.line = line;
      us.g_line = g_line;
      g_line++, count++;
      UserDic.push_back(us);
    }
  }
  WriteLog(NORMAL_LOG, L"UserDicRead : %d���� ����� ���� \"%s\"�� �о����ϴ�.\n", count, lpFileName);
  fclose(fp);
  return true;
}

// anedic.txt ȣȯ�� ���� �Լ�
// ���Ͱ� ������ �Ǹ� anedic.txt ������ ã�� �д´�
bool filter::anedic_load(int &g_line)
{
  wstring Jpn, Kor, Attr;
  WCHAR lpPathName[MAX_PATH], lpFileName[MAX_PATH];

  if (!g_bAnemone)
  {
    DWORD pid;
    HWND hwnd = FindWindow(L"AneParentClass", NULL);
    if (hwnd)
    {
      //WriteLog(NORMAL_LOG, L"[AneDicLoad] AneParentClass Found.\n");

      GetWindowThreadProcessId(hwnd, &pid);
      if (GetCurrentProcessId() != pid)
      {
        //WriteLog(NORMAL_LOG, L"[AneDicLoad] GetCurrentProcessId() != pid.\n");
        return false;
      }
    }
    else
    {
      hwnd = FindWindow(L"AnemoneParentWndClass", NULL);
      if (hwnd)
      {
        //WriteLog(NORMAL_LOG, L"[AneDicLoad] AnemoneParentWndClass Found.\n");
        GetWindowThreadProcessId(hwnd, &pid);
        if (GetCurrentProcessId() != pid)
        {
          //WriteLog(NORMAL_LOG, L"[AneDicLoad] GetCurrentProcessId() != pid.\n");
          return false;
        }
      }
      else
      {
        //WriteLog(NORMAL_LOG, L"[AneDicLoad] Anemone Not Found\n");
        return false;
      }
    }
    g_bAnemone = true;
  }

  GetExecutePath(lpPathName, MAX_PATH);
  wcscat_s(lpPathName, L"\\");
  wcscpy_s(lpFileName, L"anedic.txt");

  return userdic_load2(lpPathName, lpFileName, g_line);
}

bool filter::pre(wstring &wsText)
{
  if (!pConfig->GetPreSwitch())
  {
    WriteLog(NORMAL_LOG, L"PreFilter : ��ó���� ���� �ֽ��ϴ�.\n");
    return false;
  }
  return filter_proc(PreFilter, PREFILTER, wsText);
}

bool filter::post(wstring &wsText)
{
  if (!pConfig->GetPostSwitch())
  {
    WriteLog(NORMAL_LOG, L"PostFilter : ��ó���� ���� �ֽ��ϴ�.\n");
    return false;
  }
  return filter_proc(PostFilter, POSTFILTER, wsText);
}

bool filter::filter_proc(vector<FILTERSTRUCT> &Filter, const int FilterType, wstring &wsText)
{
  DWORD dwStart, dwEnd;
  wstring Str = wsText;
  int layer_prev = -1;
  bool layer_pass = false;
  bool pass_once = false;

  wstring pass_log;
  dwStart = GetTickCount();

  for (UINT i = 0; i < Filter.size(); i++)
  {
    // ����� ������ ���� ������ ������ �ٸ� �� ���ǽ��� ���� PASS ���� Ȯ��
    if (Filter[i].layer != layer_prev)
    {
      layer_prev = Filter[i].layer;
      layer_pass = false;

      for (UINT j = 0; j < SkipLayer.size(); j++)
      {
        if (SkipLayer[j].type == FilterType &&
          SkipLayer[j].layer == Filter[i].layer)
        {
          try
          {
            wregex ex(SkipLayer[j].cond);

            if (!regex_search(wsText, ex))
            {
              layer_pass = true;
              pass_once = true;
              pass_log += (SkipLayer[j].wlayer + L", ");
              break;
            }
          }
          catch (regex_error ex)
          {
            WCHAR lpWhat[255];
            int len = _MultiByteToWideChar(949, MB_PRECOMPOSED, ex.what(), -1, NULL, NULL);
            _MultiByteToWideChar(949, MB_PRECOMPOSED, ex.what(), -1, lpWhat, len);

            WriteLog(ERROR_LOG, L"SkipLayerRead : ���Խ� ����! : [%s:%d] %s | %d | %s\n", L"SkipLayer.txt", SkipLayer[i].line, SkipLayer[i].wtype.c_str(), SkipLayer[i].layer, SkipLayer[i].cond.c_str());
            continue;
          }
        }

      }
    }
    if (Filter[i].layer == layer_prev && layer_pass) continue;

    if (!Filter[i].regex)
    {
      Str = wsText;
      wsText = replace_all(wsText, Filter[i].src, Filter[i].dest);
      if (Str.compare(wsText))
      {
        if (FilterType == PREFILTER) WriteLog(DETAIL_LOG, L"PreFilter : [%s:%d] | %s | %s | %d | %d\n", Filter[i].db.c_str(), Filter[i].line, Filter[i].src.c_str(), Filter[i].dest.c_str(), Filter[i].layer, Filter[i].regex);
        else if (FilterType == POSTFILTER) WriteLog(DETAIL_LOG, L"PostFIlter : [%s:%d] | %s | %s | %d | %d\n", Filter[i].db.c_str(), Filter[i].line, Filter[i].src.c_str(), Filter[i].dest.c_str(), Filter[i].layer, Filter[i].regex);
      }

    }
    else
    {
      Str = wsText;

      try
      {
        wregex ex(Filter[i].src);

        if (regex_search(wsText, ex))
        {
          wsText = regex_replace(wsText, ex, Filter[i].dest, regex_constants::match_default);
        }
      }
      catch (regex_error ex)
      {
        WCHAR lpWhat[255];
        int len = _MultiByteToWideChar(949, MB_PRECOMPOSED, ex.what(), -1, NULL, NULL);
        _MultiByteToWideChar(949, MB_PRECOMPOSED, ex.what(), -1, lpWhat, len);

        if (FilterType == PREFILTER) WriteLog(ERROR_LOG, L"PreFilter : ���Խ� ����! : [%s:%d] %s | %s | %d | %d\n", Filter[i].db.c_str(), Filter[i].line, Filter[i].src.c_str(), Filter[i].dest.c_str(), Filter[i].layer, Filter[i].regex);
        else if (FilterType == POSTFILTER) WriteLog(ERROR_LOG, L"PostFilter : ���Խ� ����! : [%s:%d] %s | %s | %d | %d\n", Filter[i].db.c_str(), Filter[i].line, Filter[i].src.c_str(), Filter[i].dest.c_str(), Filter[i].layer, Filter[i].regex);
        continue;
      }

      if (Str != wsText)
      {
        if (FilterType == PREFILTER) WriteLog(DETAIL_LOG, L"PreFilter : [%s:%d] %s | %s | %d | %d\n", Filter[i].db.c_str(), Filter[i].line, Filter[i].src.c_str(), Filter[i].dest.c_str(), Filter[i].layer, Filter[i].regex);
        else if (FilterType == POSTFILTER) WriteLog(DETAIL_LOG, L"PostFilter : [%s:%d] %s | %s | %d | %d\n", Filter[i].db.c_str(), Filter[i].line, Filter[i].src.c_str(), Filter[i].dest.c_str(), Filter[i].layer, Filter[i].regex);
        Str = wsText;
      }
    }
  }

  pass_log = pass_log.substr(0, pass_log.rfind(L", "));

  if (pass_once)
  {
    if (FilterType == PREFILTER) WriteLog(SKIPLAYER_LOG, L"PreSkipLayer : %s\n", pass_log.c_str());
    else if (FilterType == POSTFILTER) WriteLog(SKIPLAYER_LOG, L"PostSkipLayer : %s\n", pass_log.c_str());
  }

  dwEnd = GetTickCount();
  if (FilterType == PREFILTER) WriteLog(TIME_LOG, L"PreFilter : --- Elasped Time : %dms ---\n", dwEnd - dwStart);
  else if (FilterType == POSTFILTER) WriteLog(TIME_LOG, L"PostFIlter : --- Elasped Time : %dms ---\n", dwEnd - dwStart);
  return true;
}

bool filter::cmd(wstring &wsText)
{
  BOOL bCommand = false;
  BOOL bSaveINI = false;
  if (wsText[0] != L'/') return false;
  if (!wsText.compare(L"/ver") || !wsText.compare(L"/version"))
  {
    wsText += L" : Ehnd ";
    wsText += WIDEN(EHND_VER);
    wsText += L", ";
    wsText += WIDEN(__DATE__);
    wsText += L", ";
    wsText += WIDEN(__TIME__);
    wsText += L"\r\n";

    SetLogText(wsText.c_str(), RGB(168, 25, 25), RGB(255, 255, 255));
    return true;
  }
  else if (!wsText.compare(L"/log"))
  {
    // jichi 4/5/2015: Disable logging
    //if (IsShownLogWin())
    //{
    //  pConfig->SetConsoleSwitch(false);
    //  wsText = L"/log : Log Window Off.";
    //}
    //else
    //{
    //  pConfig->SetConsoleSwitch(true);
    //  wsText = L"/log : Log Window On.";
    //}
    bCommand = true;
  }
  else if (!wsText.compare(L"/command"))
  {
    if (pConfig->GetCommandSwitch())
    {
      pConfig->SetCommandSwitch(false);
      wsText = L"/command : Command Off.";
    }
    else
    {
      pConfig->SetCommandSwitch(true);
      wsText = L"/command : Command On.";
    }
    bCommand = true;
    bSaveINI = true;
  }
  else if (!wsText.compare(L"/reload"))
  {
    pFilter->load();
    bCommand = true;
  }
  else if (pConfig->GetCommandSwitch())
  {
    if (!wsText.compare(L"/log_detail"))
    {
      if (pConfig->GetLogDetail())
      {
        pConfig->SetLogDetail(false);
        wsText = L"/log_detail : Detail Log Off.";
      }
      else
      {
        pConfig->SetLogDetail(true);
        wsText = L"/log_detail : Detail Log On.";
      }
      bCommand = true;
      bSaveINI = true;
    }
    else if (!wsText.compare(L"/log_time"))
    {
      if (pConfig->GetLogTime())
      {
        pConfig->SetLogTime(false);
        wsText = L"/log_time : Time Log Off.";
      }
      else
      {
        pConfig->SetLogTime(true);
        wsText = L"/log_time : Time Log On.";
      }
      bCommand = true;
      bSaveINI = true;
    }
    else if (!wsText.compare(L"/log_skiplayer"))
    {
      if (pConfig->GetLogSkipLayer())
      {
        pConfig->SetLogSkipLayer(false);
        wsText = L"/log_skiplayer : SkipLayer Log Off.";
      }
      else
      {
        pConfig->SetLogSkipLayer(true);
        wsText = L"/log_skiplayer : SkipLayer Log On.";
      }
      bCommand = true;
      bSaveINI = true;
    }
    else if (!wsText.compare(L"/log_userdic"))
    {
      if (pConfig->GetLogUserDic())
      {
        pConfig->SetLogUserDic(false);
        wsText = L"/log_userdic : UserDic Log Off.";
      }
      else
      {
        pConfig->SetLogUserDic(true);
        wsText = L"/log_userdic : UserDic Log On.";
      }
      bCommand = true;
      bSaveINI = true;
    }
    else if (!wsText.compare(L"/filelog"))
    {
      if (pConfig->GetFileLogSwitch())
      {
        pConfig->SetFileLogSwitch(false);
        wsText = L"/filelog : Write FileLog Off.";
      }
      else
      {
        pConfig->SetFileLogSwitch(true);
        wsText = L"/filelog : Write FileLog On.";
      }
      bCommand = true;
      bSaveINI = true;
    }
    else if (!wsText.compare(L"/preon") || !wsText.compare(L"/pre") && !pConfig->GetPreSwitch())
    {
      pConfig->SetPreSwitch(true);
      wsText += L" : PreFilter On.";
      bCommand = true;
    }

    else if (!wsText.compare(L"/preoff") || !wsText.compare(L"/pre") && pConfig->GetPreSwitch())
    {
      pConfig->SetPreSwitch(false);
      wsText += L" : PreFilter Off.";
      bCommand = true;
    }

    else if (!wsText.compare(L"/poston") || !wsText.compare(L"/post") && !pConfig->GetPostSwitch())
    {
      pConfig->SetPostSwitch(true);
      wsText += L" : PostFilter On.";
      bCommand = true;
    }

    else if (!wsText.compare(L"/postoff") || !wsText.compare(L"/post") && pConfig->GetPostSwitch())
    {
      pConfig->SetPostSwitch(false);
      wsText += L" : PostFilter Off.";
      bCommand = true;
    }

    else if (!wsText.compare(L"/dicon") || !wsText.compare(L"/dic") && !pConfig->GetUserDicSwitch())
    {
      pConfig->SetUserDicSwitch(true);
      wsText += L" : UserDic On.";
      bCommand = true;
    }

    else if (!wsText.compare(L"/dicoff") || !wsText.compare(L"/dic") && pConfig->GetUserDicSwitch())
    {
      pConfig->SetUserDicSwitch(false);
      wsText += L" : UserDic Off.";
      bCommand = true;
    }
  }

  if (bSaveINI) pConfig->SaveConfig();
  if (bCommand) return true;
  return false;
}
