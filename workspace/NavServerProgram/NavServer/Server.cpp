#include "stdafx.h"
#include "Server.h"


CServer::CServer()
:CServer(100)
{


}
CServer::CServer(unsigned int maxWorkingClientCount){
	this->maxWorkingClientCount = maxWorkingClientCount;
	clients.resize(maxWorkingClientCount + 2);
	this->packageSize = 4048;
}

CServer::~CServer()
{
}

