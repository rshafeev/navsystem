#pragma once
#include "afxwin.h"
/*#include "afx.h"
#include "stdafx.h"*/
#define SIZE 2048         // Размер принимаемого сообщения



class CGPSThread :	public CWinThread
{
	/*
HANDLE hMainThread;        // Дескриптор основного потока
HANDLE hGetMes;             // Дескриптор функции, определяющей тип сообщения
*/
WSADATA WSAData;           // Структура конфигурации DLL

SOCKET *WinSocket;          // Наш сокет
sockaddr_in SockStruct;    // Структура содержит информацию о сокете

int Bind;                  // Хранит результат связывания локального сетевого адреса с сокетом
int Listen;                // Хранит результат прослушивания сокета
int Connect;               // Переменная хранит результат подключения клиента к серверу
int Send;                  // Переменная хранит результат отправки сообщения
int Recv;                  // Переменная хранит результат приема сообщения
bool Terminated;
CString List ;
CString ServIP;
int Port;
char Buffer[SIZE]; 
virtual BOOL InitInstance();
void GetMessage(CString Msg);//обрабатывает пришедшее с сервера сообщение
public:
    virtual	int Run();
	CGPSThread(SOCKET *WinSocket);
	
	void Terminate();
	~CGPSThread(void);
	virtual int ExitInstance();
};
