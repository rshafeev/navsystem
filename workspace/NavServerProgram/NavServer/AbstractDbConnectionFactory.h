#pragma once
#include "adoclasses.h"

class CAbstractDbConnectionFactory
{
protected:
	CAbstractDbConnectionFactory();
public:
	
	virtual ~CAbstractDbConnectionFactory();

	virtual CADOConnection* getConnection() = 0;
	virtual void closeConnection(CADOConnection* connection) = 0;
};

