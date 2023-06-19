

#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>

#include "shell.h"



static int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	// If the BFFM_INITIALIZED message is received
	// set the path to the start path.
	switch (uMsg)
	{
		case BFFM_INITIALIZED:
		{
			if (NULL != lpData)
			{
				SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
			}
		}
	}

	return 0; // The function should always return 0.
}


bool BrowseForFolder(HWND hWnd, char* buffer)
{
	static BROWSEINFO bi;
	//static char szDirPath[MAX_PATH];
	LPITEMIDLIST pidl;

	bi.hwndOwner = hWnd;
	bi.lpszTitle = NULL;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = (LPARAM)buffer;

	pidl = SHBrowseForFolder(&bi);
	if (pidl != NULL)
	{
		SHGetPathFromIDList(pidl, buffer);

		return true;
	}

	return false;
}


bool ExecuteOpen(HWND hWnd, char* pszFileName)
{
	HINSTANCE hInst;

	hInst = ShellExecute(
		hWnd,
		"open",
		pszFileName,
		NULL,
		NULL,
		SW_SHOWNORMAL
		);

	return ((DWORD)hInst > 32);
}

