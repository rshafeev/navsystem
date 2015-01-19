#include "StdAfx.h"
#include "ServerThread.h"
#include "NavServerApp.h"

CServerThread::CServerThread(CAbstractOutputStream &_stream)
:stream(_stream)
{
	hRecv.resize(theApp.ServerManager().MaxWorkingClientCount() + 2);
	terminated = false;
}

CServerThread::~CServerThread(void)
{
	std::vector<ClientConnection> clients = theApp.ServerManager().Clients();
	unsigned int i;
	for (i = 0; i < clients.size(); i++)
	{
		if (clients[i].isActive() == true)
		{
			closesocket(clients[i].Accept());
			clients[i].Accept(NULL);
			if (hRecv[i] != NULL)
				hRecv[i]->Terminate();
			WaitForSingleObject(*hRecv[i], 500);
		}
	}
}
BOOL  CServerThread::InitInstance()
{
	return true;
}
int CServerThread::Run()
{

	CString msg;

	stream.println();
	stream.println(">> Начало работы программы...");
	if (WSAStartup(MAKEWORD(2, 1), &WSAData) != 0)
	{
		stream.println(">> Ошибка! Невозможно инициализировать библиотеку сокетов! Функция MainThread()");
		return WM_QUIT;
	}
	// Создаем сокет
	
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	theApp.ServerManager().ServSocket(serverSocket);
	if (serverSocket == SOCKET_ERROR)
	{
		stream.println(">> Ошибка!  Невозможно создать сокет! Функция MainThread()");
		return WM_QUIT;
	}
	

	// Связываем локальный сетевой адрес с сокетом
	sockaddr_in& socketInfo = theApp.ServerManager().SocketInfo();
	int bindStatus = bind(serverSocket, (sockaddr*)&socketInfo, sizeof(socketInfo));
	theApp.ServerManager().BindStatus(bindStatus);
	if (bindStatus == SOCKET_ERROR)
	{
		stream.println(">> Ошибка! Невозможно связать локальный сетевой адрес с сокетом! Функция MainThread()");
		return WM_QUIT;
	}

	// Ставим подключаемых клиетов в очередь
	int listenStatus = listen(serverSocket, 5);
	theApp.ServerManager().ListenStatus(listenStatus);
	if (listenStatus == SOCKET_ERROR)
	{
		stream.println(">> Ошибка! Невозможно связать локальный сетевой адрес с сокетом! Функция MainThread()");
		return WM_QUIT;
	}
	
	msg.Format(">> Серверное приложение запущено. Выполняется прослушивание порта %d ...", theApp.ServerManager().Port());
	stream.println(msg);

	// Ставим подключаемых клиетов в очередь
	int num = 0;
	while (true)
	{
		if (terminated == true) break;

		ClientConnection* client = theApp.ServerManager().getFreeClientConnection();
		if (client == nullptr)
		{
			Sleep(1000);
			continue;
		}
		sockaddr_in& socketInfo = client->SocketInfo();
		SOCKET clientAccept = accept(serverSocket, (sockaddr*)&socketInfo, (int*)&client->SocketInfoSize());
		client->Accept(clientAccept);
		int clientID = client->ID();
		if (clientAccept != SOCKET_ERROR)
		{
			CString str;
			str.Format("Подключился клиент.  IP: %s Порт: %d", client->IP(), client->Port());
			stream.println(str);
			// Устанавливаем флаг того, что клиент подключен
			client->Active(true);
			//создаем поток обработки/отправки сообщений от данного клиента 
			hRecv[clientID] = new ClientThread(this->stream, *client);
			hRecv[clientID]->CreateThread();

		}

	}

	return WM_QUIT;
}

void CServerThread::Terminate()
{
	terminated = true;
	closesocket(theApp.ServerManager().ServSocket());
}