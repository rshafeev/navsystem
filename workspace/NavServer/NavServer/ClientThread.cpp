#include "stdafx.h"
#include "ClientThread.h"
#include <winsock2.h>
#include "NavServerApp.h"

using namespace std;


ClientThread::ClientThread(CAbstractOutputStream &_stream, ClientConnection &_client)
:stream(_stream), client(_client)
{
	m_bAutoDelete = true;
	Terminated = false;
}

ClientThread::~ClientThread(void)
{
}
//==========================================================================================================
void ClientThread::Terminate()
{
	Terminated = true;
}

//============================================================================================================================
int ClientThread::Run()
{
	const auto SIZE = theApp.ServerManager().PackageSize();
	char* buf = new char[SIZE]; // Буфер входящих данных
	while (true)                                  // Запускаем бесконечный цикл приема сообщений
	{
		for (int i = 0; i < SIZE; i++)                // Делаем так, чтобы не было мусора во входящих данных
			buf[i] = '\0';
		int recvResult = recv(this->client.Accept(), buf, this->client.SocketInfoSize(), 0);  // Получаем данные
		CString msg = CString(buf);
		if (Terminated == true)
		{
			this->client.close();
			break;
		}
		if (recvResult == SOCKET_ERROR || recvResult == 0)
		{
			CString str;
			str.Format("Клиент отключился.  IP: %s Порт: %d", this->client.IP(), this->client.Port());
			this->stream.println(str);
			this->client.close();
			PostQuitMessage(WM_QUIT);
			break;
		}
		else
		if (msg != "")
		{
			if (this->client.ClientHandler() == nullptr)
			{
				ClientType type;
				if (msg[0] == '$')
					type = ClientType::DISPETCHER;
				else
				if (msg[0] == '@')
					type = ClientType::TRACKER;
				else
					type = ClientType::UNKNOWN;
				this->client.setupClientHandler(type);
			}
			this->client.ClientHandler()->receiveMassege(msg);
		}
	}
	delete[] buf;
	return WM_QUIT;
}

void ClientThread::Delete()
{
	this->client.close();
}

//============================================================================================================================
BOOL  ClientThread::InitInstance()
{
	SetThreadPriority(THREAD_PRIORITY_LOWEST);
	m_bAutoDelete = true;
	return true;
}


int ClientThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}
