#include "StdAfx.h"
#include "ClientThread.h"

CClientThread::CClientThread(SOCKET WinSocket)
{
	this->WinSocket=WinSocket;
}

CClientThread::~CClientThread(void)
{
}

BOOL CClientThread::InitInstance()
{
	return TRUE;
}

int CClientThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

double d = 0.0000137;
double Height = 10 + rand()*0.0822 / RAND_MAX;
double lat = 50.0159;
double lon = 36.3313;

int CClientThread::Run()
{
//	@#<ID>#<Lo>#<De>#<Height>#;
	// TODO: Add your specialized code here and/or call the base class
   CString g; 
  // CString IMEI="234324235";
  
	for(;;)
	{
		    if(WinSocket==NULL)
	{
		return WM_QUIT;
	}
		
		g.Format("@#%s#%f#%f#%f;",IMEI,lon,lat,Height);
		send(WinSocket,(LPCSTR)g,g.GetLength(),0);
		Sleep(100);
		lat += d;
		lon += d;
		
	}
	return WM_QUIT;
}
