/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/


#include "StdAfx.h"
#include "VersionInfo.h"

CVersionInfo::CVersionInfo(void)
{
	m_lpData = NULL;

	//Get EXE path
	TCHAR buff[MAX_PATH];
	GetModuleFileName(NULL, buff, MAX_PATH);	

	DWORD dwHandle = 0;
	DWORD dwVerInfoSize = GetFileVersionInfoSize(buff, &dwHandle);

	m_lpData = malloc(dwVerInfoSize);

	if(dwVerInfoSize != 0) //Success
	{
		if(GetFileVersionInfo(buff, dwHandle, dwVerInfoSize, m_lpData) == FALSE)
		{
			free(m_lpData);
			m_lpData = NULL;
		}
	}
}

CVersionInfo::~CVersionInfo(void)
{
	if(m_lpData != NULL)
		free(m_lpData);
}

CString CVersionInfo::GetValue(CString strKey)
{	
	CString strRet = _T("");

	if(m_lpData != NULL)
	{
		UINT cbTranslate = 0;

		struct LANGANDCODEPAGE 
		{
			WORD wLanguage;
			WORD wCodePage;
		} *lpTranslate;	

		VerQueryValue(m_lpData,_T("\\VarFileInfo\\Translation"),
			(LPVOID*)&lpTranslate,&cbTranslate);

		int count = (int)(cbTranslate/sizeof(struct LANGANDCODEPAGE));

		for(int i=0; i < count; i++ )
		{
			TCHAR SubBlock[128];
			HRESULT hr = StringCchPrintf(SubBlock, 127,_T("\\StringFileInfo\\%04x%04x\\%s"),
				lpTranslate[i].wLanguage,lpTranslate[i].wCodePage,strKey);	

			if(SUCCEEDED(hr))
			{
				UINT dwBytes = 0;
				TCHAR* lpBuffer;

				if(VerQueryValue(m_lpData, SubBlock, (LPVOID*)&lpBuffer, &dwBytes))
				{
					strRet = lpBuffer;
					break;
				}
			}		
		}
	}
	return strRet;
}
