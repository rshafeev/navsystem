// 3DEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "3DEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy3DEditorApp

BEGIN_MESSAGE_MAP(CMy3DEditorApp, CWinApp)
END_MESSAGE_MAP()


// CMy3DEditorApp construction

CMy3DEditorApp::CMy3DEditorApp()
{
	
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMy3DEditorApp object

CMy3DEditorApp theApp;


// CMy3DEditorApp initialization

BOOL CMy3DEditorApp::InitInstance()
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
	 GPS            =  new CGPS;
	 MainForm       =  new CMainFrame;
	 FrameContainer =  new CFrameContainer;
	 GPSDlgAdd      =  new CGPSDlgAdd;
	 GPSDlgDel      =  new CGPSDlgDel;
	 HardToolFrame  =  new CHardToolFrame;
	 
	 m_pMainWnd = MainForm;

	 FrameContainer->InitContainer();
 
	if (!MainForm)
		return FALSE;
    
	// create and load the frame with its resources
	
	MainForm->LoadFrame(IDR_MAINFRAME,WS_OVERLAPPEDWINDOW|WS_MAXIMIZE | FWS_ADDTOTITLE, NULL,
		NULL);
	/* pFrame->Create(NULL,NULL,
		               WS_CAPTION,
		              CRect(0,0,992,992),NULL,NULL);*/
	// The one and only window has been initialized, so show and update it
	MainForm->ShowWindow(SW_SHOW);
	MainForm->UpdateWindow();
		
	

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}
int CMy3DEditorApp::ExitInstance()
{

	FrameContainer->DestroyContainer();
	return CWinApp::ExitInstance();
}
