#include "stdafx.h"
#include "AbstractClientHandler.h"


CAbstractClientHandler::CAbstractClientHandler(ClientConnection& _client, ClientType _type)
:client(_client), type(_type)
{
}


CAbstractClientHandler::~CAbstractClientHandler()
{
}
