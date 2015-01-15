#pragma once
//
#include "adoclasses.h"
#include "adodefs.h"
#include "stdafx.h"
class CDBWork
{
public:
	struct TFieldValue
	{
       CString R;
	};
	static void writeDB(TGPSPoint &Add);
    static void readDB(TGPSPoint *Add);
	static void clearDB();
	CDBWork(void);
	~CDBWork(void);
};
