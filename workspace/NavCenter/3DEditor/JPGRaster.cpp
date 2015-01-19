#include "StdAfx.h"
#include "JPGRaster.h"

CJPGRaster::CJPGRaster(void)
{
	
}
CJPGRaster::CJPGRaster(CString FileName)
{
	this->FileName=FileName;
}
CJPGRaster::~CJPGRaster(void)
{
}
//*******************************************************************************************
bool CJPGRaster::LoadFile(CString FileName)
{
	FileName="Maps\\1.jpg";
	if(FileName!="")
        this->FileName=FileName;
	Image.Load(this->FileName);
	
 // map=new CBitmap;
 // map->m_hObject=LoadImageA(NULL,(LPCSTR)(this->FileName),IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_DEFAULTCOLOR);
  LoadFlag=true;
  return true;
}
//*******************************************************************************************
void CJPGRaster::Paint(CPaintDC *dc)
{

  return;
}
//*******************************************************************************************