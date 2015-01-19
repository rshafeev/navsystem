#pragma once
#include "AbstractDbConnectionFactory.h"

struct TConnectionProperties
{
	CString dbFileName;
	CString userName = "";
	CString userPassword = "";
	int nJetVer = 4;
	int nJetVerEx = 0;
};

class CAccessDbConnectionFactory : public CAbstractDbConnectionFactory
{
	TConnectionProperties connectionProperties;
	CADOConnection* connection;
public:
	CAccessDbConnectionFactory(TConnectionProperties _connectionProperties);
	virtual ~CAccessDbConnectionFactory();
	virtual CADOConnection* getConnection();
	virtual void closeConnection(CADOConnection* connection);

};

