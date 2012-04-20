/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/

#pragma once

#pragma comment(lib,"Version.lib")

#include <strsafe.h>

class CVersionInfo
{
public:
	CVersionInfo(void);
	~CVersionInfo(void);
	CString GetValue(CString strKey);
private:
	LPVOID m_lpData;
};
