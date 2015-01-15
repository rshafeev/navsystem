#pragma once
#include "afxwin.h"
#include "DBWork.h"
#include "stdafx.h"
#include "ClientThread.h"
#define SIZE 4048         // Размер принимаемого сообщения
/*
& - начало протокола от программы 3dEditor
@ - начало протокола от программы PCMobileGPS
*/
class CServerThread :	public CWinThread
{
private:
 int Port;
 CListBox *ListBox;
 WSADATA WSAData;           // Структура конфигурации DLL
 SOCKET WinSocket;          // Наш сокет
 sockaddr_in SockStruct;    // Структура содержит информацию о сокете
 int Bind;                  // Хранит результат связывания локального сетевого адреса с сокетом
 int Listen;                // Хранит результат прослушивания сокета
 unsigned int  Q;           // Определяем количество одновременно работающих клиентов
 CString List;
 HANDLE hMainThread;        // Дескриптор основного потока
 HANDLE hGetMes;            // Дескриптор функции, определяющей тип сообщения
 CADOConnection *m_connection; 
 vector<CServerUsersThread * > hRecv;
 bool Terminated;
public:
	 virtual BOOL InitInstance();
	 CServerThread();
 	 CServerThread(int Port,CListBox *OutPut, CADOConnection *DbConnect);
	~CServerThread(void);
	virtual int Run();
	void Terminate();
};
extern  vector<User> users;