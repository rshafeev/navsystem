#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "GPSPointRepository.h"
#include "AbstractOutputStream.h"
#include "ClientThread.h"
#include "Server.h"
/*
& - начало протокола от программы 3dEditor
@ - начало протокола от программы PCMobileGPS
*/
class CServerThread : public CWinThread
{
private:
	WSADATA WSAData;           // Структура конфигурации DLL
	CString List;
	HANDLE hMainThread;        // Дескриптор основного потока
	HANDLE hGetMes;            // Дескриптор функции, определяющей тип сообщения
	vector<ClientThread * > hRecv;
	bool terminated;
	CServer &server;
	CAbstractOutputStream &stream;
public:
	virtual BOOL InitInstance();
	CServerThread(CServer &serv, CAbstractOutputStream &stream);
	~CServerThread(void);
	virtual int Run();
	void Terminate();
};