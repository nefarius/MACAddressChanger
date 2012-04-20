/*
	Copyright(C) Nishant Sivakumar, 2005. All rights reserved.
	http://blog.voidnish.com
*/

// MACAddressChanger.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMACAddressChangerApp:
// See MACAddressChanger.cpp for the implementation of this class
//

class CMACAddressChangerApp : public CWinApp
{
public:
	CMACAddressChangerApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMACAddressChangerApp theApp;