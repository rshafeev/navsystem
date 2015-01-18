#pragma once
#include "IDataBaseService.h"
#include "AbstractDbConnectionFactory.h"

class CAccessDataBaseService : public IDataBaseService
{
	ILocationsRepository* locationsRepository;
	CAbstractDbConnectionFactory& connectionFactory;
public:
	CAccessDataBaseService(CAbstractDbConnectionFactory& _connectionFactory);
	virtual ~CAccessDataBaseService();
	virtual ILocationsRepository& Locations();
};

