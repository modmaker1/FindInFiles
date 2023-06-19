
//
// Copyright (c) 2011-2020, Ilya Lyutin
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


/*
TODO:
- UTF-8?
- allow SearchWhat to be unicode too
- make codepages work (for the case insensitive search)
- explorer directory context menus...
*/


#include <windows.h>
#include <commctrl.h>
#pragma comment (lib, "comctl32.lib")
#include <stdio.h>
#include "../shared/utils.h"

#include "registry.h"
#include "settings.h"
#include "shell.h"

#include "resource.h"

extern "C" {
// asm implemented
extern void* __tp1250_toupper;
extern void* __tp1251_toupper;
extern void* __tp1252_toupper;
extern void* __tp1253_toupper;
extern void* __tp1254_toupper;
extern void* __tp1257_toupper;
extern void* __tp1258_toupper;
DWORD I386_ScanMemory(char* buffer, DWORD size, char* psz, int iLen, int* piLine);
DWORD I386_ScanMemory_I(char* buffer, DWORD size, char* psz, int iLen, int* piLine, void* cp);
}
 
HINSTANCE g_hInst;
HWND g_hWnd;
HMENU g_hPopup;

// search settings
char* g_pszSearchWhat;
char* g_pszSrcDir;
char** g_apszFilter;
bool g_bMatchCase;
bool g_bMatchWholeWords;
bool g_bRecurseSubDirs;
bool g_bListAllOccurrences;
//bool g_bLocalSettings;
bool g_bDetectUnicode;
bool g_bForceUnicode;

// search vars
bool g_bWorking;
bool g_bStop;
int g_nFilesSearched;
int g_nFilesFound;
int g_nOccurs;
char* g_pszCurFile;

// global settings
settings_t g_s;

const char g_szAppName[] = "Find in Files 2.0";
const char g_szCopyright[] = "";


void WM_InitDialog(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	g_hWnd = hWnd;

	LV_COLUMN lvc;
	ZeroMemory(&lvc, sizeof(LV_COLUMN));
	lvc.mask = LVCF_WIDTH | LVCF_TEXT;
	lvc.cx = 200;
	lvc.pszText = "File";
	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);

	lvc.cx = 50;
	lvc.pszText = "Line";
	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);

	lvc.cx = 500;
	lvc.pszText = "Text";
	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_INSERTCOLUMN, 2, (LPARAM)&lvc);

	int iStyleEx = LVS_EX_FULLROWSELECT;
	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)iStyleEx);

	g_hPopup = CreatePopupMenu();
	AppendMenu(g_hPopup, MF_STRING, 10001, "Open");

	SetWindowText(g_hWnd, g_szAppName);
	SetDlgItemText(g_hWnd, IDC_STATUS_STRING, g_szCopyright);
}


void ExecuteOpenCommand(char* pszFileName, char* pszLine)
{
	char szCmdLine[512];
	char* psz = g_s.pszOpenWith;
	int j = 0;
	int i;

	for (i = 0; ; i++)
	{
		if (psz[i] == '%')
		{
			if ((psz[i+1] == 'f') || (psz[i+1] == 'F'))
			{
				i++;
				for (int n = 0; pszFileName[n] != '\0'; n++)
				{
					szCmdLine[j++] = pszFileName[n];
				}
				continue;
			}
			else if ((psz[i+1] == 'l') || (psz[i+1] == 'L'))
			{
				i++;
				for (int n = 0; pszLine[n] != '\0'; n++)
				{
					szCmdLine[j++] = pszLine[n];
				}
				continue;
			}
		}
		szCmdLine[j++] = psz[i];

		if (psz[i] == '\0')
		{
			break;
		}
	}

	WinExec(szCmdLine, SW_SHOWNORMAL);
}


void OpenListItem(int i)
{
	char szFileName[MAX_PATH];
	char szLine[16];
	int iPos;
	iPos = sprintf(szFileName, "%s\\", g_pszSrcDir);

	LV_ITEM lvi;
	ZeroMemory(&lvi, sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = i;
	lvi.pszText = &szFileName[iPos];
	lvi.cchTextMax = MAX_PATH-iPos;
	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_GETITEMTEXT, i, (LPARAM)&lvi);

	lvi.iSubItem = 1;
	lvi.pszText = szLine;
	lvi.cchTextMax = 16;
	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_GETITEMTEXT, i, (LPARAM)&lvi);

	if (g_s.bUseShell)
	{
		ExecuteOpen(g_hWnd, szFileName);
	}
	else
	{
		ExecuteOpenCommand(szFileName, szLine);
	}
}


void OpenSelectedListItem(void)
{
	int i = (int)SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_GETNEXTITEM, -1, MAKELPARAM(LVNI_SELECTED, 0));
	if (i != -1)
	{
		OpenListItem(i);
	}
}


void AddToList(char* pszFileName, int iPos, char* pszLine)
{
	LV_ITEM lvi;
	int i;

	i = (int) SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_GETITEMCOUNT, 0, 0);

	ZeroMemory(&lvi, sizeof(LV_ITEM));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = i;

	lvi.pszText = pszFileName;
	//lvi.cchTextMax = 255;
	//lvi.lParam = (LPARAM) pid;

	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_INSERTITEM, 0, (LPARAM)&lvi);

	lvi.iSubItem = 1;
	char sz[16];
	sprintf(sz, "%d", iPos);
	lvi.pszText = sz;

	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_SETITEM, 0, (LPARAM)&lvi);

	lvi.iSubItem = 2;
	lvi.pszText = pszLine;

	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_SETITEM, 0, (LPARAM)&lvi);
}


void ClearList(void)
{
	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_DELETEALLITEMS, 0, 0);
}


bool IsAlphanumeric(unsigned char c)
{
	return (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || (c == '_'));
}


bool IsPrintable(unsigned char c)
{
	return (c >= 32);
}


// the word is conisdered to be whole if it's surrounded by non-alphanumerical chars
bool IsWhole(char* data, DWORD iSize, DWORD iPos, DWORD iLen)
{
	// check the symbol before the first one
	if (iPos > 0)
	{
		if (IsAlphanumeric(data[iPos-1]))
		{
			return false;
		}
	}

	// check the symbol after the last one
	if ((iPos + iLen) < iSize)
	{
		if (IsAlphanumeric(data[iPos+iLen]))
		{
			return false;
		}
	}

	return true;
}


#define MAX_LEADING 32
#define MAX_TAILING 128

// tries scan for line feeds in both directions from iPos...
void GetLine(char* pszLine, int iMax, char* data, DWORD iSize, DWORD iPos)
{
	int iStart;
	int iEnd;
	int iStop;
	int i;

	// find the beginning of the line
	iStop = iPos - min(MAX_LEADING, iPos);
	for (i = iPos; i >= iStop; i--)
	{
		if ((data[i] == '\n') || (data[i] == '\r') || (data[i] == '\0'))
		{
			break;
		}
	}
	/*
	// cut spaces
	for ( ; i < iPos; i++)
	{
		if ((data[i] != ' ') && (data[i] != '\t'))
		{
			break;
		}
	}
	*/
	iStart = i+1;

	// find the end of the line
	iStop = iPos + min(MAX_TAILING, iSize-iPos);
	for (i = iPos; i < iStop; i++)
	{
		if ((data[i] == '\n') || (data[i] == '\r') || (data[i] == '\0'))
		{
			//i--;
			break;
		}
	}

	iEnd = i;

	// copy the line
	i = iEnd - iStart;
	memcpy(pszLine, &data[iStart], i);
	pszLine[i] = '\0';

	/*
	// cut tabs and spaces
	i = 0;
	j = 0;
	for ( ; pszLine[i] != '\0'; i++)
	{
		if ((pszLine[i] == ' ') || (pszLine[i] == '\t'))
		{
			pszLine[i] = ' ';
			if (i > iStart)
			{
				if (pszLine[i-1] == ' ')
				{
					continue;
				}
			}
		}

		pszLine[j++] = pszLine[i];
	}

	pszLine[j] = '\0';
	*/

	for (i = 0; pszLine[i] != '\0'; i++)
	{
		if (!IsPrintable(pszLine[i]))
		{
			pszLine[i] = ' ';
		}
	}
}


void GetLineUnicode(char* pszLine, int iMax, char* data, DWORD iSize, DWORD iPos)
{
	int iStart;
	int iEnd;
	int iStop;
	int iLen;
	int i, j;

	// get the beginning
	iStop = iPos - min(MAX_LEADING, iPos);
	for (i = iPos; i >= iStop; i -= 2)
	{
		if ((*(wchar_t*)&data[i] == '\n') || (*(wchar_t*)&data[i] == '\r') || (*(wchar_t*)&data[i] == '\0'))
		{
			break;
		}
	}

	iStart = i + 2;

	// get the end
	iStop = iPos + min(MAX_TAILING, iSize-iPos);
	for (i = iPos; i < iStop; i += 2)
	{
		if ((*(wchar_t*)&data[i] == '\n') || (*(wchar_t*)&data[i] == '\r') || (*(wchar_t*)&data[i] == '\0'))
		{
			//i--;
			break;
		}
	}

	iEnd = i;

	// copy the line
	//iLen = iEnd - iStart;
	j = sprintf(pszLine, "[UNICODE] ");
	for (i = iStart; i < iEnd; i += 2)
	{
		pszLine[j++] = (unsigned char)*(unsigned short*)&data[i];
	}
	pszLine[j] = '\0';

	// replace unprintable
	for (i = 0; pszLine[i] != '\0'; i++)
	{
		if (!IsPrintable(pszLine[i]))
		{
			pszLine[i] = ' ';
		}
	}
}


bool IsUnicode(char* data, DWORD iSize)
{
	if (iSize >= 2)
	{
		if ((data[0] == -1) && (data[1] == -2))
		{
			return true;
		}
	}

	return false;
}


void ScanMemory(char* data, DWORD iSize)
{
	DWORD iGlobalPos = 0;
	int iGlobalLine = 1;
	int nOccurs = 0;
	char* pszSearchStr;
	int iLen;
	bool bUnicode;
	char szLine[MAX_PATH];
	int i;

	iLen = strlen(g_pszSearchWhat);

	if (g_bForceUnicode || (g_bDetectUnicode && IsUnicode(data, iSize)))
	{
		pszSearchStr = (char*)AllocMemory((iLen + 1) * 2);
		
		// fast unicode conversion
		for (i = 0; i < (iLen + 1); i++)
		{
			((unsigned short*)pszSearchStr)[i] = (unsigned char)g_pszSearchWhat[i];
		}

		iLen *= 2;
		bUnicode = true;
	}
	else
	{
		pszSearchStr = AllocString(g_pszSearchWhat);
		bUnicode = false;
	}

	for (;; iGlobalPos++)
	{
		int iLine;
		DWORD iPos;
		
		if (g_bMatchCase)
		{
			iPos = I386_ScanMemory(
				&data[iGlobalPos], iSize - iGlobalPos, pszSearchStr, iLen, &iLine);
		}
		else
		{
			// TODO: codepages
			iPos = I386_ScanMemory_I(
				&data[iGlobalPos], iSize - iGlobalPos, pszSearchStr, iLen, &iLine, &__tp1250_toupper);
		}

		if (iPos == -1)
		{
			break;	
		}

		iGlobalPos += iPos;
		iGlobalLine += iLine;

		if (g_bMatchWholeWords)
		{
			if (!IsWhole(data, iSize, iGlobalPos, iLen))
			{
				goto skip;
			}
		}

		nOccurs++;
		g_nOccurs++;

		// add to the list
		
		if (bUnicode)
		{
			GetLineUnicode(szLine, MAX_PATH, data, iSize, iGlobalPos);
		}
		else
		{
			GetLine(szLine, MAX_PATH, data, iSize, iGlobalPos);
		}

		AddToList(g_pszCurFile, iGlobalLine, szLine);

		if (!g_bListAllOccurrences)
		{
			break;
		}
skip:
		if (g_bStop)
		{
			break;
		}
	}

	FreeMemory(pszSearchStr);

	if (nOccurs != 0)
	{
		g_nFilesFound++;
	}
}


void ProcessFile(char* pszPath)
{
	char szFileName[MAX_PATH];
	HANDLE hFile;
	HANDLE hFileMapping;
	char* data;
	DWORD iSize;
	DWORD iSizeHigh;

	g_pszCurFile = pszPath;

	SetDlgItemText(g_hWnd, IDC_STATUS_STRING, pszPath);

	sprintf(szFileName, "%s\\%s", g_pszSrcDir, pszPath);

	hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		iSize = GetFileSize(hFile, &iSizeHigh);
		if ((iSize < 0x80000000) && (iSizeHigh == 0))
		{
			hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
			if (hFileMapping != NULL)
			{
				data = (char*)MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
				if (data != NULL)
				{
					ScanMemory(data, iSize);

					UnmapViewOfFile(data);
				}

				CloseHandle(hFileMapping);
			}
		}
		CloseHandle(hFile);
	}
}


char* GetExt(char* source)
{
    char* ext = NULL;
    int i;

    for (i = 0; source[i] != 0; i++)
	{
        if (source[i] == '.')
		{
            ext = &source[i+1];
        }
    }

    return ext;
}


bool IsExt(char* pszFileName)
{
	char* pszExt;

	pszExt = GetExt(pszFileName);
	if (pszExt != NULL)
	{
		for (int i = 0; g_apszFilter[i] != NULL; i++)
		{
			if (stricmp(pszExt, g_apszFilter[i]) == 0)
			{
				return true;
			}
		}
	}

	return false;
}


void ProcessDirectory(char* pszPath)
{
	WIN32_FIND_DATA fd;
	HANDLE hFile;
	char szSearchPath[MAX_PATH];
	char szFileName[MAX_PATH];
	int iDirPass;
	int iFilePass;

	if (pszPath != NULL)
	{
		sprintf(szSearchPath, "%s\\%s\\*", g_pszSrcDir, pszPath);
	}
	else
	{
		sprintf(szSearchPath, "%s\\*", g_pszSrcDir);
	}


	iDirPass = (g_s.bDirectoriesFirst)? 0: 1;
	iFilePass = (g_s.bDirectoriesFirst)? 1: 0;

	for (int iPass = 0; iPass < 2; iPass++)
	{
		hFile = FindFirstFile(szSearchPath, &fd);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (g_bStop)
				{
					break;
				}

				for (int i = 0; fd.cFileName[i] == '.'; i++)
				{
					// do nothing
				}
				if (fd.cFileName[i] == '\0')
				{
					continue;
				}

				if (pszPath != NULL)
				{
					sprintf(szFileName, "%s\\%s", pszPath, fd.cFileName);
				}
				else
				{
					strcpy(szFileName, fd.cFileName);
				}

				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (iPass == iDirPass)
					{
						if (g_bRecurseSubDirs)
						{
							ProcessDirectory(szFileName);
						}
					}
				}
				else
				{
					if (iPass == iFilePass)
					{
						if (g_apszFilter != NULL)
						{
							if (!IsExt(szFileName))
							{
								continue;
							}
						}

						ProcessFile(szFileName);
						g_nFilesSearched++;
					}
				}
			}
			while (FindNextFile(hFile, &fd));
			FindClose(hFile);
		}
	}
}


char* AllocTextFromItem(int ctrl)
{
	char* psz = NULL;
	int iSize = (int) SendDlgItemMessage(g_hWnd, ctrl, WM_GETTEXTLENGTH, 0, 0);
	if (iSize != 0)
	{
		iSize++;
		psz = (char*)malloc(iSize);
		SendDlgItemMessage(g_hWnd, ctrl, WM_GETTEXT, (WPARAM)iSize, (LPARAM)psz);
	}

	return psz;
}


void FreeText(char* psz)
{
	if (psz != NULL)
	{
		free(psz);
	}
}


char** AllocFilter(void)
{
	char** apszFilter = NULL;
	char* psz = AllocTextFromItem(IDC_FILTER);
	if (psz != NULL)
	{
		// count num items in the filter string
		int n = 0;
		bool bStop = false;
		for (int i = 0; !bStop; i++)
		{
			bStop = (psz[i] == '\0');

			if ((psz[i] == ' ') || (psz[i] == '\0'))
			{
				if (i > 0)
				{
					if (psz[i-1] != '\0')
					{
						n++;
					}
				}
				psz[i] = '\0';
			}
		}

		// alloc if any
		if (n > 0)
		{
			apszFilter = (char**)malloc(sizeof(char*) * (n + 1));
			int iFilter = 0;
			int j = 0;
			for (int i = 0; n > iFilter; i++)
			{
				if (psz[i] == '\0')
				{
					if (i > 0)
					{
						if (psz[i-1] != '\0')
						{
							apszFilter[iFilter] = (char*)malloc((i-j)+1);
							strcpy(apszFilter[iFilter], &psz[j]);
							iFilter++;
						}
					}
					j = i + 1;
				}
			}
			apszFilter[iFilter] = NULL;
		}

		FreeText(psz);
	}

	return apszFilter;
}


void FreeFilter(char** apsz)
{
	if (apsz != NULL)
	{
		for (int i = 0; apsz[i] != NULL; i++)
		{
			free(apsz[i]);
		}
		free(apsz);
	}
}


bool IsChecked(int ctrl)
{
	return ((int)SendDlgItemMessage(g_hWnd, ctrl, BM_GETCHECK, 0, 0) == BST_CHECKED);
}


void SetChecked(int ctrl, bool b)
{
	SendDlgItemMessage(g_hWnd, ctrl, BM_SETCHECK, (b)? BST_CHECKED: BST_UNCHECKED, 0);
}


void UpdateComboBoxSelection(int ctrl)
{
	int i = (int)SendDlgItemMessage(g_hWnd, ctrl, CB_GETCURSEL, 0, NULL);
	if ((i != 0) && (i != CB_ERR))
	{
		int iSize = (int) SendDlgItemMessage(g_hWnd, ctrl, CB_GETLBTEXTLEN, i, NULL);
		char* psz = (char*)malloc(iSize+1);
		SendDlgItemMessage(g_hWnd, ctrl, CB_GETLBTEXT, i, (LPARAM)psz);
		SendDlgItemMessage(g_hWnd, ctrl, CB_DELETESTRING, i, NULL);
		SendDlgItemMessage(g_hWnd, ctrl, CB_INSERTSTRING, 0, (LPARAM)psz);
		free(psz);
		SendDlgItemMessage(g_hWnd, ctrl, CB_SETCURSEL, 0, NULL);
	}
}


void CheckAddStringToComboBox(int ctrl, int iMaxStrings)
{
	char* psz = AllocTextFromItem(ctrl);
	if (psz != NULL)
	{
		int i = (int)SendDlgItemMessage(g_hWnd, ctrl, CB_FINDSTRINGEXACT, -1, (LPARAM)psz);
		if (i != 0)
		{
			if (i != CB_ERR) // move to top
			{
				SendDlgItemMessage(g_hWnd, ctrl, CB_DELETESTRING, i, NULL);
			}

			SendDlgItemMessage(g_hWnd, ctrl, CB_INSERTSTRING, 0, (LPARAM)psz);
			SendDlgItemMessage(g_hWnd, ctrl, CB_SETCURSEL, 0, NULL);

			int n = (int)SendDlgItemMessage(g_hWnd, ctrl, CB_GETCOUNT, 0, NULL);
			if (n > iMaxStrings)
			{
				for (i = iMaxStrings; i < n; i++)
				{
					SendDlgItemMessage(g_hWnd, ctrl, CB_DELETESTRING, i, NULL);
				}
			}
		}
	}
}


int MultiStringLength(const char* psz)
{
	int i;

	for (i = 0; ; i++)
	{
		if (psz[i] == '\0')
		{
			i++;
			if (psz[i] == '\0')
			{
				break;
			}
		}
	}

	return i;
}



void GetMultiStringFromComboBox(int ctrl, char** ppsz, int* piSize, char c)
{
	char* psz = NULL;
	int j = 0;
	int i;

	int n = (int)SendDlgItemMessage(g_hWnd, ctrl, CB_GETCOUNT, 0, NULL);
	if (n > 0)
	{
		int iLen = 0;
		
		for (i = 0; i < n; i++)
		{
			iLen += (int)SendDlgItemMessage(g_hWnd, ctrl, CB_GETLBTEXTLEN, i, NULL) + 1;
		}

		psz = (char*)malloc(iLen + 1);

		for (i = 0; i < n; i++)
		{
			j += (int)SendDlgItemMessage(g_hWnd, ctrl, CB_GETLBTEXT, i, (LPARAM)&psz[j]);
			psz[j++] = c;
		}

		psz[j++] = '\0';
	}

	*ppsz = psz;
	*piSize = j;
}


void InitComboBoxFromMultiString(int ctrl, char* psz, int iSize, char c)
{
	int i;
	int j = 0;

	SendDlgItemMessage(g_hWnd, ctrl, WM_SETREDRAW, FALSE, NULL);
	for (i = 0; i < iSize; i++)
	{
		if (psz[i] == c)
		{
			psz[i] = '\0';
			SendDlgItemMessage(g_hWnd, ctrl, CB_ADDSTRING, 0, (LPARAM)&psz[j]);

			j = i + 1;
			if (psz[j] == '\0')
			{
				break;
			}
		}
	}
	SendDlgItemMessage(g_hWnd, ctrl, WM_SETREDRAW, TRUE, NULL);
	SendDlgItemMessage(g_hWnd, ctrl, CB_SETCURSEL, 0, NULL);
}


#define MAX_SEARCH_STRINGS 128
#define MAX_PATH_STRINGS 32
#define MAX_FILTER_STRINGS 128

DWORD WINAPI SearchFunc(void* lpParameter)
{
	FreeText(g_pszSearchWhat);
	FreeText(g_pszSrcDir);
	FreeFilter(g_apszFilter);
	g_pszSearchWhat = NULL;
	g_pszSrcDir = NULL;
	g_apszFilter = NULL;
	g_bMatchCase = IsChecked(IDC_MATCH_CASE);
	g_bMatchWholeWords = IsChecked(IDC_MATCH_WHOLE_WORDS);
	g_bRecurseSubDirs = IsChecked(IDC_RECURSE_SUBDIRS);
	g_bListAllOccurrences = IsChecked(IDC_LIST_ALL_OCCURS);
	g_bDetectUnicode = IsChecked(IDC_DETECT_UNICODE);
	g_bForceUnicode = IsChecked(IDC_FORCE_UNICODE);

	g_nFilesSearched = 0;
	g_nFilesFound = 0;
	g_nOccurs = 0;

	ClearList();

	SetWindowText(g_hWnd, g_szAppName);
	SetDlgItemText(g_hWnd, IDC_STATUS_STRING, "Working...");

	g_pszSearchWhat = AllocTextFromItem(IDC_SEARCH_WHAT);
	if (g_pszSearchWhat != NULL)
	{
		CheckAddStringToComboBox(IDC_SEARCH_WHAT, MAX_SEARCH_STRINGS);

		g_pszSrcDir = AllocTextFromItem(IDC_SRC_DIR);
		if (g_pszSrcDir != NULL)
		{
			CheckAddStringToComboBox(IDC_SRC_DIR, MAX_PATH_STRINGS);
		}

		if (g_pszSrcDir == NULL)
		{
			// if none, just grab the current directory
			g_pszSrcDir = (char*)malloc(MAX_PATH);
			GetCurrentDirectory(MAX_PATH, g_pszSrcDir);
		}

		if (IsChecked(IDC_USE_FILTER))
		{
			g_apszFilter = AllocFilter();
			if (g_apszFilter != NULL)
			{
				CheckAddStringToComboBox(IDC_FILTER, MAX_FILTER_STRINGS);
			}
		}

		ProcessDirectory(NULL);

		char szStatus[MAX_PATH];
		sprintf(szStatus, "%d files found for \'%s\' (%d files searched). %d occurrences total.", g_nFilesFound, g_pszSearchWhat, g_nFilesSearched, g_nOccurs);
		SetDlgItemText(g_hWnd, IDC_STATUS_STRING, szStatus);

		sprintf(szStatus, "\'%s\' - %s - %d files found", g_pszSearchWhat, g_szAppName, g_nFilesFound);
		SetWindowText(g_hWnd, szStatus);
	}
	else
	{
		SetDlgItemText(g_hWnd, IDC_STATUS_STRING, "");
	}

	SetDlgItemText(g_hWnd, IDC_SEARCH, "Search");
	g_bWorking = false;

	return 0;
}


void BeginSearch(void)
{
	HANDLE hThread;
	static DWORD threadID;

	if (!g_bWorking)
	{
		g_bWorking = true;
		g_bStop = false;
		SetDlgItemText(g_hWnd, IDC_SEARCH, "Stop");

		hThread = (HANDLE)CreateThread(NULL, 0, SearchFunc, NULL, CREATE_SUSPENDED, &threadID);
		if (hThread != NULL)
		{
			g_bStop = false;

			SetThreadPriority(hThread, THREAD_PRIORITY_IDLE);

			ResumeThread(hThread);

			CloseHandle(hThread);
		}
	}
	else
	{
		g_bStop = true;
	}
}


void ShowOptionsDlg(void)
{
	DialogBox(NULL, "Options", g_hWnd, OptionsDlgProc);
}


void BrowseSrcDir(void)
{
	static char szDir[MAX_PATH];

	SendDlgItemMessage(g_hWnd, IDC_SRC_DIR, WM_GETTEXT, (WPARAM)MAX_PATH, (LPARAM)szDir);

	if (BrowseForFolder(g_hWnd, szDir))
	{
		SendDlgItemMessage(g_hWnd, IDC_SRC_DIR, WM_SETTEXT, 0, (LPARAM)szDir);
	}
}


void UpdateUseFilter(void)
{
	BOOL bEnable = (IsChecked(IDC_USE_FILTER))? TRUE: FALSE;
	EnableWindow(GetDlgItem(g_hWnd, IDC_FILTER), bEnable);
	if (!bEnable)
	{
		SendDlgItemMessage(g_hWnd, IDC_FILTER, WM_SETTEXT, 0, (LPARAM)"All files");
	}
	else
	{
		SendDlgItemMessage(g_hWnd, IDC_FILTER, CB_SETCURSEL, 0, NULL);
	}
}


void WM_Command(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (lParam == 0)
	{
		switch (LOWORD(wParam))
		{
		case 10001:
			OpenSelectedListItem();
			break;
		}
	}
	else if (HIWORD(wParam) == BN_CLICKED)
	{
		switch (LOWORD(wParam))
		{
			case IDC_SEARCH:
				BeginSearch();
				break;
			case IDC_OPTIONS:
				ShowOptionsDlg();
				break;
			case IDC_BROWSE_PATH:
				BrowseSrcDir();
				break;
			case IDC_USE_FILTER:
				UpdateUseFilter();
				break;
		}
	}
	else if (HIWORD(wParam) == CBN_SELCHANGE)
	{
		switch (LOWORD(wParam))
		{
		case IDC_SEARCH_WHAT:
			UpdateComboBoxSelection(IDC_SEARCH_WHAT);
			break;
		case IDC_SRC_DIR:
			UpdateComboBoxSelection(IDC_SRC_DIR);
			break;
		case IDC_FILTER:
			UpdateComboBoxSelection(IDC_FILTER);
			break;
		}
	}
}


int GetListItemFromCursor(void)
{
	LV_HITTESTINFO lvht;
	ZeroMemory(&lvht, sizeof(LV_HITTESTINFO));
	GetCursorPos(&lvht.pt);
	ScreenToClient(GetDlgItem(g_hWnd, IDC_SEARCH_RESULTS), &lvht.pt);
	
	SendDlgItemMessage(g_hWnd, IDC_SEARCH_RESULTS, LVM_HITTEST, 0, (LPARAM)&lvht);

	return lvht.iItem;
}


void WM_Notify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (LOWORD(wParam) == IDC_SEARCH_RESULTS)
	{
		if (((NMHDR*)lParam)->code == NM_DBLCLK)
		{
			int i = GetListItemFromCursor();
			if (i != -1)
			{
				OpenListItem(i);
			}
		}
		else if (((NMHDR*)lParam)->code == NM_RCLICK)
		{
			int i = GetListItemFromCursor();
			if (i != -1)
			{
				POINT pt;
				GetCursorPos(&pt);
				TrackPopupMenu(g_hPopup, 0, pt.x, pt.y, 0, g_hWnd, NULL);
			}
		}
	}
}


void LoadDefaultSettings(void)
{
	g_s.bMatchCase = FALSE;
	g_s.bMatchWholeWords = FALSE;
	g_s.bRecurseSubDirs = TRUE;
	g_s.bListAllOccurrences = TRUE;
	g_s.bDirectoriesFirst = TRUE;
	g_s.bDetectUnicode = TRUE;
	g_s.bForceUnicode = FALSE;
	g_s.bUseFilter = FALSE;
	//g_s.bUseLocalSettings = FALSE;
	//g_s.bGlobal = TRUE;
	g_s.bUseShell = FALSE;
	const char szDefFilter[] = "c cpp h\0txt\0\0";
	g_s.iFilterLen = MultiStringLength(szDefFilter)+1;
	g_s.pszFilter = (char*)malloc(g_s.iFilterLen);
	memcpy(g_s.pszFilter, szDefFilter, g_s.iFilterLen);
	const char szDefOpenWith[] = "notepad \"%f\"";
	g_s.iOpenWithLen = strlen(szDefOpenWith)+1;
	g_s.pszOpenWith = (char*)malloc(g_s.iOpenWithLen);
	memcpy(g_s.pszOpenWith, szDefOpenWith, g_s.iOpenWithLen);

}


//CRegKey* g_pRegKeyGlobal;
CRegKey* g_pRegKey;

void LoadSettings(void)
{
	LoadDefaultSettings();

	//g_pRegKeyGlobal = new CRegKey(HKEY_LOCAL_MACHINE, "Software\\FindInFiles2");
	//g_pRegKeyGlobal->AddEntry("bGlobal", REG_DWORD, (void**)&g_s.bGlobal, sizeof(DWORD), NULL);

	g_pRegKey = new CRegKey(HKEY_LOCAL_MACHINE, "Software\\FindInFiles2");
	//g_pRegKey->AddEntry("bMatchCase", REG_DWORD, (void**)&g_s.bMatchCase, sizeof(DWORD), NULL);
	//g_pRegKey->AddEntry("bMatchWholeWords", REG_DWORD, (void**)&g_s.bMatchWholeWords, sizeof(DWORD), NULL);
	//g_pRegKey->AddEntry("bRecurseSubDirs", REG_DWORD, (void**)&g_s.bRecurseSubDirs, sizeof(DWORD), NULL);
	//g_pRegKey->AddEntry("bListAllOccurrences", REG_DWORD, (void**)&g_s.bListAllOccurrences, sizeof(DWORD), NULL);
	g_pRegKey->AddEntry("bDirectoriesFirst", REG_DWORD, (void**)&g_s.bDirectoriesFirst, sizeof(DWORD), NULL);
	//g_pRegKey->AddEntry("bUsePath", REG_DWORD, (void**)&g_s.bUsePath, sizeof(DWORD), NULL);
	//g_pRegKey->AddEntry("bUseFilter", REG_DWORD, (void**)&g_s.bUseFilter, sizeof(DWORD), NULL);
	g_pRegKey->AddEntry("bUseShell", REG_DWORD, (void**)&g_s.bUseShell, sizeof(DWORD), NULL);
	//g_pRegKey->AddEntry("bUseLocalSettings", REG_DWORD, (void**)&g_s.bUseLocalSettings, sizeof(DWORD), NULL);
	//g_pRegKey->AddEntry("SearchWhat", REG_MULTI_SZ, (void**)&g_s.pszSearchWhat, 0, &g_s.iSearchWhatLen);
	//g_pRegKey->AddEntry("Path", REG_MULTI_SZ, (void**)&g_s.pszPath, 0, &g_s.iSrcDirLen);
	//g_pRegKey->AddEntry("Filter", REG_MULTI_SZ, (void**)&g_s.pszFilter, 0, &g_s.iFilterLen);
	g_pRegKey->AddEntry("OpenWith", REG_SZ, (void**)&g_s.pszOpenWith, 0, &g_s.iOpenWithLen);

	// read local settigns
	ReadSettings(&g_s);

	// read global settings if allowed
	//g_pRegKeyGlobal->Query();
	//if (g_s.bGlobal)
	//{
		g_pRegKey->Query();
	//}

	SetChecked(IDC_MATCH_CASE, (g_s.bMatchCase != FALSE));
	SetChecked(IDC_MATCH_WHOLE_WORDS, (g_s.bMatchWholeWords != FALSE));
	SetChecked(IDC_RECURSE_SUBDIRS, (g_s.bRecurseSubDirs != FALSE));
	SetChecked(IDC_LIST_ALL_OCCURS, (g_s.bListAllOccurrences != FALSE));
	SetChecked(IDC_DETECT_UNICODE, (g_s.bDetectUnicode != FALSE));
	SetChecked(IDC_FORCE_UNICODE, (g_s.bForceUnicode != FALSE));
	SetChecked(IDC_USE_FILTER, (g_s.bUseFilter != FALSE));
	//SetChecked(IDC_LOCAL_SETTINGS, (g_s.bUseLocalSettings != FALSE));
	InitComboBoxFromMultiString(IDC_SEARCH_WHAT, g_s.pszSearchWhat, g_s.iSearchWhatLen, '\0');
	InitComboBoxFromMultiString(IDC_SRC_DIR, g_s.pszSrcDir, g_s.iSrcDirLen, '\0');
	InitComboBoxFromMultiString(IDC_FILTER, g_s.pszFilter, g_s.iFilterLen, '\0');

	UpdateUseFilter();
}


void SaveSettings(void)
{
	g_s.bMatchCase = (IsChecked(IDC_MATCH_CASE))? TRUE: FALSE;
	g_s.bMatchWholeWords = (IsChecked(IDC_MATCH_WHOLE_WORDS))? TRUE: FALSE;
	g_s.bRecurseSubDirs= (IsChecked(IDC_RECURSE_SUBDIRS))? TRUE: FALSE;
	g_s.bListAllOccurrences = (IsChecked(IDC_LIST_ALL_OCCURS))? TRUE: FALSE;
	g_s.bDetectUnicode = (IsChecked(IDC_DETECT_UNICODE))? TRUE: FALSE;
	g_s.bForceUnicode = (IsChecked(IDC_FORCE_UNICODE))? TRUE: FALSE;
	g_s.bUseFilter = (IsChecked(IDC_USE_FILTER))? TRUE: FALSE;
	//g_s.bUseLocalSettings = (IsChecked(IDC_LOCAL_SETTINGS))? TRUE: FALSE;
	GetMultiStringFromComboBox(IDC_SEARCH_WHAT, &g_s.pszSearchWhat, &g_s.iSearchWhatLen, '\0');
	GetMultiStringFromComboBox(IDC_SRC_DIR, &g_s.pszSrcDir, &g_s.iSrcDirLen, '\0');
	GetMultiStringFromComboBox(IDC_FILTER, &g_s.pszFilter, &g_s.iFilterLen, '\0');

	WriteSettings(&g_s);

	//g_pRegKeyGlobal->Store();
	//if (g_s.bGlobal)
	//{
		g_pRegKey->Store();
	//}

	// XXX:
	delete g_pRegKey;
}


BOOL CALLBACK MainDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg)
	{
	case WM_INITDIALOG:
		WM_InitDialog(hWnd, wParam, lParam);
		LoadSettings();
		break;

	case WM_COMMAND:
		WM_Command(hWnd, wParam, lParam);
		break;

	case WM_NOTIFY:
		WM_Notify(hWnd, wParam, lParam);
		break;

	case WM_CLOSE:
		SaveSettings();
		EndDialog(hWnd, 0);
		break;

	default:
		return 0;
	}

	return 1;
}


int APIENTRY WinMain(HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    CmdLine,
					int       nCmdShow)
{
	g_hInst = hInstance;

	InitCommonControls();

	DialogBoxParam(g_hInst, "Main", NULL, MainDlgProc, 0);

	return 0;
}

