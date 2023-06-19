
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


#include "registry.h"

#include <stdio.h>
#include "../shared/utils.h"


// CRegKeyEntry

class CRegKeyEntry
{
public:
	CRegKeyEntry::CRegKeyEntry(char* pszName, int eType, void** pValue, int iSize, int* piSize);
	~CRegKeyEntry();
	void Query(HKEY hkey);
	void Store(HKEY hkey);
	CRegKeyEntry* next;
private:
	char* m_pszName;
	int m_eType;
	void** m_pValue;
	int m_iSize;
	int* m_piSize;
};


CRegKeyEntry::CRegKeyEntry(char* pszName, int eType, void** pValue, int iSize, int* piSize)
{
	next = NULL;

	m_pszName = AllocString(pszName);
	m_eType = eType;
	m_pValue = pValue;
	m_iSize = iSize;
	m_piSize = piSize;
}


CRegKeyEntry::~CRegKeyEntry()
{
	if (m_pszName != NULL)
	{
		FreeString(m_pszName);
	}
}


void CRegKeyEntry::Query(HKEY hkey)
{
	DWORD eType = m_eType;
	int eStatus;
	void* p;
	int iSize;

	if (m_piSize != NULL)
	{
		eStatus = RegQueryValueExA(hkey, m_pszName, NULL, &eType, NULL, (DWORD*)&iSize);
		if (eStatus == ERROR_SUCCESS)
		{
			*m_piSize = iSize;
			if (*m_pValue != NULL)
			{
				free(*m_pValue);
			}
			*m_pValue = malloc(*m_piSize);
		}

		p = *m_pValue;
		iSize = *m_piSize;
	}
	else
	{
		p = m_pValue;
		iSize = m_iSize;
	}

	RegQueryValueExA(hkey, m_pszName, NULL, &eType, (BYTE*)p, (DWORD*)&iSize);
}


void CRegKeyEntry::Store(HKEY hkey)
{
	void* p;
	int iSize;

	if (m_piSize != NULL)
	{
		p = *m_pValue;
		iSize = *m_piSize;
	}
	else
	{
		p = m_pValue;
		iSize = m_iSize;
	}

	RegSetValueExA(hkey, m_pszName, NULL, m_eType, (BYTE*)p, iSize);
}


// CRegKey


void CRegKey::Open(void)
{
	RegOpenKeyExA(
		m_hkeyRoot,
		m_pszSubKey,
		0,
		KEY_READ | KEY_WRITE,
		&m_hkey
		);
}


void CRegKey::Close(void)
{
	if (m_hkey != NULL)
	{
		RegCloseKey(m_hkey);
		m_hkey = NULL;
	}
}


CRegKey::CRegKey(HKEY hkeyRoot, char* pszSubKey)
{
	int eDisposition;
	int eStatus;

	m_hkeyRoot = hkeyRoot;
	m_pszSubKey = AllocString(pszSubKey);
	m_hkey = NULL;
	m_pEntryFirst = NULL;

	eStatus = RegCreateKeyExA(
		m_hkeyRoot,
		m_pszSubKey,
		0,
		"REG_DWORD",
		0,
		KEY_READ | KEY_WRITE,
		NULL,
		&m_hkey,
		(DWORD*)&eDisposition
		);

	Close();
}


CRegKey::~CRegKey()
{
	Close();

	if (m_pszSubKey != NULL)
	{
		FreeString(m_pszSubKey);
	}

	CRegKeyEntry* p;

	p = m_pEntryFirst;
	while (p != NULL)
	{
		CRegKeyEntry* pEntry;
		pEntry = p;
		p = p->next;
		delete pEntry;
	}
}


void CRegKey::AddEntry(char* pszName, DWORD eType, void** pValue, int iSize, int* piSize)
{
	CRegKeyEntry** p;

	p = &m_pEntryFirst;
	while ((*p) != NULL)
	{
		p = &(*p)->next;
	}

	*p = new CRegKeyEntry(pszName, eType, pValue, iSize, piSize);
}


void CRegKey::Query(void)
{
	CRegKeyEntry* p;

	Open();

	p = m_pEntryFirst;
	while (p != NULL)
	{
		p->Query(m_hkey);
		p = p->next;
	}

	Close();
}


void CRegKey::Store(void)
{
	CRegKeyEntry* p;

	Open();

	p = m_pEntryFirst;
	while (p != NULL)
	{
		p->Store(m_hkey);
		p = p->next;
	}

	Close();
}

