// NavServer.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "NavServerApp.h"
#include "MainFrm.h"
#include "AccessDbConnectionFactory.h"
#include "AccessDataBaseService.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNavServerApp

BEGIN_MESSAGE_MAP(CNavServerApp, CWinApp)
END_MESSAGE_MAP()


// CNavServerApp construction

CNavServerApp::CNavServerApp()
{
	this->server = new CServerManager();
	TConnectionProperties connectionProperties; 
	connectionProperties.dbFileName = "C://History.mdb";
	this->dbConnectionFactory = new CAccessDbConnectionFactory(connectionProperties);
	this->dbService = new CAccessDataBaseService(*this->dbConnectionFactory);

	this->dbConnectionFactory->getConnection();
	/*TGPSPoint location; 
	location.IMEI = "Sfdsf";
	location.DateTime = "01.01.2010";
	location.Description = "dsfsdf";
	location.Height = "33";
	location.Longitude = "33.11";
	
	this->dbService->Locations().insert(location);*/
}


// The one and only CNavServerApp object

CNavServerApp theApp;


// CNavServerApp initialization

BOOL CNavServerApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));



	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,WS_MINIMIZEBOX|WS_SYSMENU|WS_EX_OVERLAPPEDWINDOW |
		FWS_ADDTOTITLE, NULL,
		NULL);
	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
    


	startServerThread();
	
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
    


	return TRUE;
}

int CNavServerApp::ExitInstance()
{
	if (this->dbService != nullptr)
		delete dbService;
	if (this->dbConnectionFactory != nullptr)
		delete dbConnectionFactory;
	return CWinApp::ExitInstance();
}


int CNavServerApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
}

BOOL CNavServerApp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::PreTranslateMessage(pMsg);
}

BOOL CNavServerApp::PumpMessage()
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::PumpMessage();
}

int CNavServerApp::Run()
{
	if(WM_LBUTTONDOWN)
	{
		m_pMainWnd->ShowWindow(SW_SHOW);
	}

	return CWinApp::Run();
}
