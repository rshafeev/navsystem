#include "stdafx.h"
#include "ClientConnection.h"
#include "AbstractClientHandler.h"
#include "TrakerClientHandler.h"
#include "DispatcherClientHandler.h"
#include "NavServerApp.h"

ClientConnection::ClientConnection()
{
	this->socketInfoSize = sizeof(socketInfo);
	this->clientIP = "";
	this->clientHandler = nullptr;
	this->active = false;
}


ClientConnection::~ClientConnection()
{
	if (this->clientHandler != nullptr)
		delete this->clientHandler;
}

void ClientConnection::setupClientHandler(ClientType type)
{
	if (this->clientHandler != nullptr)
		delete this->clientHandler;

	if (type == ClientType::TRACKER)
		this->clientHandler = new CTrakerClientHandler(*this);
	else
	if (type == ClientType::DISPETCHER)
		this->clientHandler = new CDispatcherClientHandler(*this);
}

void ClientConnection::close()
{
	this->active = false;
	if (this->accept != 0)
	{
		closesocket(accept);
		this->accept = 0;
	}
	if (this->clientHandler != nullptr){
		theApp.ServerManager().TrackersMap().removeClient(this->clientHandler);
		this->clientHandler = nullptr;
	}
}