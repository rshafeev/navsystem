#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "AbstractOutputStream.h"
#include "ClientThread.h"
/*
& - начало протокола от программы 3dEditor
@ - начало протокола от программы PCMobileGPS
*/
class CServerThread : public CWinThread
{
private:
	WSADATA WSAData;           // Структура конфигурации DLL
	HANDLE hMainThread;        // Дескриптор основного потока
	HANDLE hGetMes;            // Дескриптор функции, определяющей тип сообщения
	vector<ClientThread * > hRecv;
	bool terminated;
	CAbstractOutputStream &stream;

public:
	virtual BOOL InitInstance();
	CServerThread(CAbstractOutputStream &stream);
	~CServerThread(void);
	virtual int Run();
	void Terminate();
};