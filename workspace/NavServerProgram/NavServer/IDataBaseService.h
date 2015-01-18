#pragma once

#include "ILocationsRepository.h"

class IDataBaseService
{
protected:
	IDataBaseService();
public:
	virtual ~IDataBaseService();
	virtual ILocationsRepository& Locations() = 0;
};

