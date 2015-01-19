#pragma once
#include "protocols.h"

class ILocationsRepository
{
protected:
	ILocationsRepository();
public:
	virtual ~ILocationsRepository();

	virtual void insert(TGPSPoint &gpsPoint)=0;

};

