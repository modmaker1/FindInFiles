
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


typedef struct settings_s
{
	int bMatchCase;
	int bMatchWholeWords;
	int bRecurseSubDirs;
	int bListAllOccurrences;
	int bDetectUnicode;
	int bForceUnicode;
	int bUseFilter;
	//int bUseLocalSettings;
	//int bGlobal;
	int bUseShell;
	int bDirectoriesFirst;
	char* pszSearchWhat;
	int iSearchWhatLen;
	char* pszSrcDir;
	int iSrcDirLen;
	char* pszFilter;
	int iFilterLen;
	char* pszOpenWith;
	int iOpenWithLen;
} settings_t;
extern settings_t g_s;

bool ReadSettings(settings_t* ps);
bool WriteSettings(settings_t* ps);

BOOL CALLBACK OptionsDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

