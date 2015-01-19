#include "StdAfx.h"
#include "Raster.h"
#include "mathem.h"
#include "afx.h"
CRaster::CRaster(CString FileName)
{
	
   LoadFlag=false;
}
CRaster::CRaster(void)
{
	//GenerateKoefPriv();
	LoadFlag=false;
}
CRaster::~CRaster(void)
{
}
