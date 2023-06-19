
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

#include <stdio.h>
#include "../shared/utils.h"

#include "settings.h"

#include "resource.h"

#define SETTINGS_FILE_VER '02FF'


void GetSettingsFileName(char* buffer, int iSize)
{
	GetModuleFileName(NULL, buffer, iSize);

	char* pszExt = GetExtension(buffer);
	strcpy(pszExt, "settings");
}


bool ReadSettings(settings_t* ps)
{
	char szFileName[MAX_PATH];
	FILE* stream;
	bool bSuccess = false;

	GetSettingsFileName(szFileName, MAX_PATH);

	stream = fopen(szFileName, "rb");
	if (stream != NULL)
	{
		int ver;
		fread(&ver, sizeof(int), 1, stream);
		if (ver == SETTINGS_FILE_VER)
		{
			fread(&ps->bMatchCase, sizeof(int), 1, stream);
			fread(&ps->bMatchWholeWords, sizeof(int), 1, stream);
			fread(&ps->bRecurseSubDirs, sizeof(int), 1, stream);
			fread(&ps->bListAllOccurrences, sizeof(int), 1, stream);
			fread(&ps->bDirectoriesFirst, sizeof(int), 1, stream);
			fread(&ps->bUseFilter, sizeof(int), 1, stream);
			//fread(&ps->bUseLocalSettings, sizeof(int), 1, stream);
			fread(&ps->bUseShell, sizeof(int), 1, stream);

			fread(&ps->iSearchWhatLen, sizeof(int), 1, stream);
			if (ps->pszSearchWhat != NULL)
			{
				free(ps->pszSearchWhat);
			}
			ps->pszSearchWhat = (char*)malloc(ps->iSearchWhatLen);
			fread(ps->pszSearchWhat, ps->iSearchWhatLen, 1, stream);

			fread(&ps->iSrcDirLen, sizeof(int), 1, stream);
			if (ps->pszSrcDir != NULL)
			{
				free(ps->pszSrcDir);
			}
			ps->pszSrcDir = (char*)malloc(ps->iSrcDirLen);
			fread(ps->pszSrcDir, ps->iSrcDirLen, 1, stream);

			fread(&ps->iFilterLen, sizeof(int), 1, stream);
			if (ps->pszFilter != NULL)
			{
				free(ps->pszFilter);
			}
			ps->pszFilter = (char*)malloc(ps->iFilterLen);
			fread(ps->pszFilter, ps->iFilterLen, 1, stream);

			fread(&ps->iOpenWithLen, sizeof(int), 1, stream);
			if (ps->pszOpenWith != NULL)
			{
				free(ps->pszOpenWith);
			}
			ps->pszOpenWith = (char*)malloc(ps->iOpenWithLen);
			fread(ps->pszOpenWith, ps->iOpenWithLen, 1, stream);

			bSuccess = true;
		}

		fclose(stream);
	}

	return bSuccess;
}


bool WriteSettings(settings_t* ps)
{
	char szFileName[MAX_PATH];
	FILE* stream;
	bool bSuccess = false;

	GetSettingsFileName(szFileName, MAX_PATH);

	stream = fopen(szFileName, "wb");
	if (stream != NULL)
	{
		int ver = SETTINGS_FILE_VER;
		fwrite(&ver, sizeof(int), 1, stream);

		fwrite(&ps->bMatchCase, sizeof(int), 1, stream);
		fwrite(&ps->bMatchWholeWords, sizeof(int), 1, stream);
		fwrite(&ps->bRecurseSubDirs, sizeof(int), 1, stream);
		fwrite(&ps->bListAllOccurrences, sizeof(int), 1, stream);
		fwrite(&ps->bDirectoriesFirst, sizeof(int), 1, stream);
		fwrite(&ps->bUseFilter, sizeof(int), 1, stream);
		//fwrite(&ps->bUseLocalSettings, sizeof(int), 1, stream);
		fwrite(&ps->bUseShell, sizeof(int), 1, stream);

		fwrite(&ps->iSearchWhatLen, sizeof(int), 1, stream);
		fwrite(ps->pszSearchWhat, ps->iSearchWhatLen, 1, stream);

		fwrite(&ps->iSrcDirLen, sizeof(int), 1, stream);
		fwrite(ps->pszSrcDir, ps->iSrcDirLen, 1, stream);

		fwrite(&ps->iFilterLen, sizeof(int), 1, stream);
		fwrite(ps->pszFilter, ps->iFilterLen, 1, stream);

		fwrite(&ps->iOpenWithLen, sizeof(int), 1, stream);
		fwrite(ps->pszOpenWith, ps->iOpenWithLen, 1, stream);

		bSuccess = true;

		fclose(stream);
	}

	return bSuccess;
}


void UpdateUseShell(HWND hWnd)
{
	BOOL bEnable = (SendDlgItemMessage(hWnd, IDC_USE_SHELL, BM_GETCHECK, 0, 0) == BST_CHECKED);
	EnableWindow(GetDlgItem(hWnd, IDC_OPEN_WITH), !bEnable);
	EnableWindow(GetDlgItem(hWnd, IDC_OPEN_WITH_HELP), !bEnable);
}


void LoadOptions(HWND hWnd)
{
	SendDlgItemMessage(hWnd, IDC_OPEN_WITH, WM_SETTEXT, 0, (LPARAM)g_s.pszOpenWith);
	SendDlgItemMessage(hWnd, IDC_OPEN_WITH, EM_SETMODIFY, FALSE, 0);
	SendDlgItemMessage(hWnd, IDC_USE_SHELL, BM_SETCHECK, (g_s.bUseShell)? BST_CHECKED: BST_UNCHECKED, 0);
	SendDlgItemMessage(hWnd, IDC_DIRECTORIES_FIRST, BM_SETCHECK, (g_s.bDirectoriesFirst)? BST_CHECKED: BST_UNCHECKED, 0);
	//SendDlgItemMessage(hWnd, IDC_GLOBAL, BM_SETCHECK, (g_s.bGlobal)? BST_CHECKED: BST_UNCHECKED, 0);
	UpdateUseShell(hWnd);
}


void SaveOptions(HWND hWnd)
{
	if (SendDlgItemMessage(hWnd, IDC_OPEN_WITH, EM_GETMODIFY, 0, 0))
	{
		free(g_s.pszOpenWith);
		g_s.iOpenWithLen = (int)SendDlgItemMessage(hWnd, IDC_OPEN_WITH, WM_GETTEXTLENGTH, 0, 0)+1;
		g_s.pszOpenWith = (char*)malloc(g_s.iOpenWithLen);
		SendDlgItemMessage(hWnd, IDC_OPEN_WITH, WM_GETTEXT, g_s.iOpenWithLen, (LPARAM)g_s.pszOpenWith);
	}
	g_s.bUseShell = (SendDlgItemMessage(hWnd, IDC_USE_SHELL, BM_GETCHECK, 0, 0) == BST_CHECKED);
	g_s.bDirectoriesFirst = (SendDlgItemMessage(hWnd, IDC_DIRECTORIES_FIRST, BM_GETCHECK, 0, 0) == BST_CHECKED);
	//g_s.bGlobal = (SendDlgItemMessage(hWnd, IDC_GLOBAL, BM_GETCHECK, 0, 0) == BST_CHECKED);
}


void Options_InitDlg(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	LoadOptions(hWnd);
}


void Options_Command(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		switch (LOWORD(wParam))
		{
		case IDC_USE_SHELL:
			UpdateUseShell(hWnd);
			break;
		case IDOK:
			SaveOptions(hWnd);
			EndDialog(hWnd, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			break;
		}
	}
}


BOOL CALLBACK OptionsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		Options_InitDlg(hWnd, wParam, lParam);
		break;
	case WM_COMMAND:
		Options_Command(hWnd, wParam, lParam);
		break;
	case WM_CLOSE:
		EndDialog(hWnd, 0);
		break;
	default:
		return 0;
	}

	return 1;
}

