#pragma once
#include "AbstractClientHandler.h"
class CTrakerClientHandler : public CAbstractClientHandler
{
private:
	CString IMEI;
public:
	CTrakerClientHandler(ClientConnection& _client);
	~CTrakerClientHandler();

	virtual void receiveMassege(CString msg);
};

