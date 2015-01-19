#pragma once
#include "afxwin.h"

#include "AbstractOutputStream.h"
#include "ClientConnection.h"

class ClientThread : public CWinThread
{
private:
	ClientConnection& client;
	CAbstractOutputStream &stream;
	bool Terminated;
public:
	ClientThread(CAbstractOutputStream &_stream, ClientConnection &_client);
	~ClientThread(void);
	
	virtual BOOL InitInstance();
	virtual int Run();
	virtual void Delete();
	void Terminate();
	virtual int ExitInstance();
};
