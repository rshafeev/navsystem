#pragma once

class CAbstractClient
{
protected:
	bool active;             // Признак того, что клиент подключен (true - подключен, false - отключен)
	int accept;              // Сокет подключенного клиента
	sockaddr_in acpStruct;   // Сруктура содержит информацию о подключенном клиенте
	int size;                // Размер этой структуры
	CString IP;              // IP-адрес клиента
public:
	CAbstractClient();
	virtual ~CAbstractClient();

};

