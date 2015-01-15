// NavServer.h : main header file for the NavServer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "adoclasses.h"
#include "adodefs.h"

// CNavServerApp:
// See NavServer.cpp for the implementation of this class
//

class CNavServerApp : public CWinApp
{
public:


// Overrides
public:
	CNavServerApp();
	virtual BOOL InitInstance();
   	CADOConnection m_connection; 
// Implementation

public:
	DECLARE_MESSAGE_MAP()
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PumpMessage();
	virtual int Run();
};

extern CNavServerApp theApp;
extern CADOConnection d;