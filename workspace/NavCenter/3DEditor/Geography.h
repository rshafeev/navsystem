#pragma once
#include "afx.h"
#include "math.h"
	static bool ConvertDoubleToGMS(double d,int *grad,int* min, double*sec);
    static bool ConvertStringToGMS(CString str,int *grad,int* min, double*sec);
	static double ConvertGMSToDouble(int grad,int min,double sec);

	static CString ConvertGMSToStr(int grad,int min,double sec);
	static CString ConvertGeoDoubleToStr(double d);

	//	static int ReadCoordFile(CString FileName,vector<TPointPriviazka>* List);/*читает файл, хранящий данные привязки и 
	//																		 записывает в List;возвращает кол-во точек*/
    //static int WriteCoordFile(CString FileName,vector<TPointPriviazka>* List);/*записывает в файл данные привязки


//=========================================================================================================================
bool ConvertDoubleToGMS(double d, int *grad, int *min, double *sec)
{
 double dg= (int)d;
 double dm = (int)((d-dg)*60.0);
 double ds=((d-dg-dm/60.0)*3600);

 *grad = (int)dg;
 *min  = (int)dm;
 *sec  = ds;
 return  true;
}
//=========================================================================================================================
bool ConvertStringToGMS(CString str, int *grad, int *min, double *sec)
{
	double d=atof((LPCSTR)str);
	ConvertDoubleToGMS(d,grad,min,sec);
	return true;
}
//=========================================================================================================================
double ConvertGMSToDouble(int grad,int min,double sec)
{
	double result;
	result=(double)grad+(double)min/60.0+(double)sec/3600.0;
   return result;
}
//=========================================================================================================================
CString ConvertGMSToStr(int grad,int min,double sec)
{
      CString Result;
      CString minS,secS;
	  secS.Format("%11.4f",(int(sec*10000.0))/10000.0);
	  secS.TrimLeft();
	  Result.Format("%d:%d:%s",secS);
	  return Result;
}			
//========================================================================================================================= 
CString ConvertGeoDoubleToStr(double d)
{
	 int g,m;
	 double sec;
	  ConvertDoubleToGMS(d,&g,&m,&sec);
      CString Result;
      CString secS,minS,gradS;
	  	  
	  secS.Format("%11.4f",fabs((int(sec*10000.0))/10000.0));
	  secS.TrimLeft();
	  minS.Format("%d",abs(m));
	  gradS.Format("%d",g);
	  
	  if(abs(m)<10.0)minS.Insert(0,"0");
	  if(abs(g)<10.0)gradS.Insert(0,"0");
	  if(fabs(sec)<10.0)secS.Insert(0,"0");
	  Result.Format("%s:%s:%s",gradS,minS,secS);
	  return Result;
}			
//========================================================================================================================= 