#pragma once
#include "afxwin.h"

class CClientThread :
	public CWinThread
{
	CString IMEI;
public:
	SOCKET WinSocket;
	bool Terminate;
	void SetIMEI(CString imei){this->IMEI=imei;}
	CClientThread(SOCKET WinSocket);
	~CClientThread(void);
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
};
