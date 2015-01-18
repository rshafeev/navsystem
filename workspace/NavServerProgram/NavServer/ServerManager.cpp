#include "stdafx.h"
#include "ServerManager.h"


CServerManager::CServerManager()
:CServerManager(599933)
{
}

CServerManager::CServerManager(int port)
:CServerManager(100, port)
{


}
CServerManager::CServerManager(unsigned int maxWorkingClientCount, int port){
	this->maxWorkingClientCount = maxWorkingClientCount;
	clients.resize(maxWorkingClientCount + 2);
	this->packageSize = 4048;
	this->port = port;

	// Заполняем поля структуры для сокета
	sockaddr_in socketInfo;
	socketInfo.sin_family = AF_INET;
	socketInfo.sin_port = port;
	socketInfo.sin_addr.s_addr = INADDR_ANY;
	this->socketInfo = socketInfo;
	for (unsigned int i = 0; i < clients.size(); i++)
	{
		clients[i].ID(i);
	}
}

CServerManager::~CServerManager()
{
}

ClientConnection* CServerManager::getFreeClientConnection()
{

	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i].isActive() == false)
			return &this->clients[i];
	}

	return nullptr;
}
