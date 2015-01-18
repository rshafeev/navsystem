#include "stdafx.h"
#include "AccessDataBaseService.h"
#include "LocationsRepository.h"

CAccessDataBaseService::CAccessDataBaseService(CAbstractDbConnectionFactory& _connectionFactory)
:connectionFactory(_connectionFactory)
{
	this->locationsRepository = new CLocationsRepository(_connectionFactory);
}




CAccessDataBaseService::~CAccessDataBaseService()
{
}

ILocationsRepository& CAccessDataBaseService::Locations()
{
	return *locationsRepository;
}