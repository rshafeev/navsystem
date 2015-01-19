#include "stdafx.h"
#include "AccessDbConnectionFactory.h"
#include "adodefs.h"


CAccessDbConnectionFactory::CAccessDbConnectionFactory(TConnectionProperties _connectionProperties)
:connectionProperties(_connectionProperties)
{
	this->connection = nullptr;
}


CAccessDbConnectionFactory::~CAccessDbConnectionFactory()
{
	if (this->connection != nullptr){
		this->connection->Close();
		delete this->connection;
	}
}

CADOConnection* CAccessDbConnectionFactory::getConnection()
{
	if (this->connection != nullptr)
		return this->connection;
	TRY_ADO()
		this->connection = new CADOConnection();
	this->connection->SetMSAccessConnectionParam(connectionProperties.nJetVer, connectionProperties.nJetVerEx, connectionProperties.dbFileName,
		connectionProperties.userName,
		connectionProperties.userPassword);
	this->connection->Open();
	CATCH_ADO()
		return connection;
}

void CAccessDbConnectionFactory::closeConnection(CADOConnection* connection)
{
	
}