#pragma once
#include "AbstractClientHandler.h"
#include "protocols.h"

class CDispatcherClientHandler : public CAbstractClientHandler
{
public:
	CDispatcherClientHandler(ClientConnection& _client);
	~CDispatcherClientHandler();
	virtual void receiveMassege(CString msg);
	bool sendMessage(TGPSPoint &location);
};

