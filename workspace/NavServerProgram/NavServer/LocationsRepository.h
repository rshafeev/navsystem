#pragma once
//
#include "adoclasses.h"
#include "adodefs.h"
#include "stdafx.h"

#include "protocols.h"
#include "ILocationsRepository.h"
#include "AbstractDbConnectionFactory.h"


class CLocationsRepository : public ILocationsRepository
{
	CAbstractDbConnectionFactory& connectionFactory;
public:
	CLocationsRepository(CAbstractDbConnectionFactory& _connectionFactory);
	~CLocationsRepository(void);
	virtual void insert(TGPSPoint &gpsPoint);
	void clear();
};
