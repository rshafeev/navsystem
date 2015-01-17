#pragma once
#include <winsock.h>
#include <hash_map>
#include "ClientTraker.h"

class CServer
{
private:
	unsigned int  maxWorkingClientCount;           //Q - Определяем количество одновременно работающих клиентов
	unsigned int packageSize;  // Размер принимаемого сообщения
	int port;
	SOCKET servSocket;          // Наш сокет
	sockaddr_in socketInfo;    // Структура содержит информацию о сокете
	int bindStatus;            // Хранит результат связывания локального сетевого адреса с сокетом
	int listenStatus;          // Хранит результат прослушивания сокета
	std::hash_map<CString, ClientTraker> trackersMap;
	std::vector<CAbstractClient> clients;

public:
	CServer();
	CServer(unsigned int maxWorkingClientCount);
	~CServer();
	void Port(int val) { port = val; }
	SOCKET ServSocket() const { return servSocket; }
	void ServSocket(SOCKET val) { servSocket = val; }
	unsigned int MaxWorkingClientCount() const { return maxWorkingClientCount; }
	std::vector<CAbstractClient> Clients() const { return clients;}
};

