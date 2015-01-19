#pragma once
#include "ClientConnection.h"


enum ClientType
{
	DISPETCHER, TRACKER, UNKNOWN
};
class CAbstractClientHandler
{
protected:
	ClientConnection& client;
	ClientType type;
public:
	CAbstractClientHandler(ClientConnection& _client, ClientType _type);
	virtual ~CAbstractClientHandler();

	virtual void receiveMassege(CString msg) = 0;

	ClientConnection& Client(){ return this->client; }

	bool sendMessage(CString msg)
	{

		if (send(this->client.Accept(), (LPCSTR)msg, msg.GetLength(), 0) == S_OK)
			return true;
		return false;
	}


	ClientType Type()
	{
		return this->type;
	}
};

