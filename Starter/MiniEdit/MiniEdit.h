
// MiniEdit.h : main header file for the MiniEdit application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMiniEditApp:
// See MiniEdit.cpp for the implementation of this class
//

class CMiniEditApp : public CWinApp
{
public:
	CMiniEditApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMiniEditApp theApp;
