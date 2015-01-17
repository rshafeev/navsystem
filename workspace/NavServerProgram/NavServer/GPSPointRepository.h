#pragma once
//
#include "adoclasses.h"
#include "adodefs.h"
#include "stdafx.h"
class GPSPointRepository
{
private:
	GPSPointRepository(void);
	~GPSPointRepository(void);
public:
	void insert(TGPSPoint &gpsPoint);
	void clearDB();
};
