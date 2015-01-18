// NavServer.h : main header file for the NavServer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "adoclasses.h"
#include "adodefs.h"
#include "ServerManager.h"
#include "IDataBaseService.h"
#include "AbstractDbConnectionFactory.h"
#include "AbstractOutputStream.h"

class CNavServerApp : public CWinApp
{
private:
	//CADOConnection m_connection;
	CServerManager *server;
	IDataBaseService* dbService;
	CAbstractDbConnectionFactory* dbConnectionFactory;
	CAbstractOutputStream *stream;

	CServerThread *serverThread;
// Overrides
public:
	CNavServerApp();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	CServerManager& ServerManager()
	{
		return *server;
	}
	IDataBaseService& DdService()
	{
		return *dbService;
	}
	void Stream(CAbstractOutputStream *stream)
	{
		this->stream = stream;
	}

	CAbstractOutputStream& Stream()
	{
		return *stream;
	}

	CServerThread* ServerThread()
	{
		return serverThread;
	}
	
public:
	
	void startServerThread()
	{
		if (serverThread != nullptr)
			return;
		serverThread = new CServerThread(Stream());
		serverThread->CreateThread();
	}

	void stopServerThread()
	{
		if (serverThread == nullptr)
			return;
		serverThread->Terminate();
		WaitForSingleObject(*serverThread, 3000);
	}

	bool isStartedServerThread()
	{
		return serverThread != nullptr ? true : false;
	}
	DECLARE_MESSAGE_MAP()
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PumpMessage();
	virtual int Run();

};

extern CNavServerApp theApp;
