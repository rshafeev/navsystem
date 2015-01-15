#include "StdAfx.h"
#include "Linkage.h"
#include "mathem.h"
CLinkage::CLinkage(void)
{
	   bindingFlag = false;
	   created     = false;
}

CLinkage::~CLinkage(void)
{
	PointsPriviazka.clear();

}
//===========================================================================================================
void CLinkage::CreateObj(CRect MapRect)
{
	this->MapRect=MapRect;
	created=true;
}
//===========================================================================================================
double CLinkage::F1(double &x, double &y, double &De)
{
return (a[1] + a[2] * x + a[3] * y + a[4] * x*x+a[5]*x*y + a[6] *y*y-De);

}
double CLinkage::F2(double &x, double &y, double &Lo)
{
return (b[1] + b[2] * x + b[3] * y + b[4] * x*x+ b[5]*x*y + b[6] *y*y-Lo);
}
//===========================================================================================================
/*	int x1[9]={3059,9649,6231,353,5938,8601,8619,349,3110};
	int y1[9]={2529,3047,7408,9368,11187,8767,4926,8760,6733};
	double x2[9]={50.37880,50.32451,49.590984,49.572320,49.554860,49.577810,50.14669,49.578075,49.597660};//широта
	double y2[9]={36.118426,36.216291,36.165390,36.77810,36.160710,36.200580,36.201182,36.77870,36.118820};//долгота
*/ //(X*XT)^-1)*XT*y
void CLinkage::GenerateKoefPriv()
{
   // создаем мартицу X
	int n=6,m;                  //n-кол-во коэф-тов, m-точек
	int i;                      //счетчик   
	CMatrix *K=NULL,R(n,n);     // временные матрицы

	m=PointsPriviazka.size();  //узнаем количество точек
	if(m>=6)                   //если кол-во точек больше минимального 
	{
	  //высчитываем коэ-фты дл€ перевода из кооринат X/Y в долготу/широту
      try
      {
	     CMatrix X(m,n);           
  	     CMatrix Xt(n,m),Va(6,1),Vx(m,1),Vb(6,1),Vy(m,1);
		 // заполн€ем матрицу 
	     for(i=0;i<m;i++)
	     {
		   X.SetElem(i,0,1);
		   X.SetElem(i,1,PointsPriviazka[i].MapX);
		   X.SetElem(i,2,PointsPriviazka[i].MapY);
		   X.SetElem(i,3,PointsPriviazka[i].MapX*PointsPriviazka[i].MapX);
		   X.SetElem(i,4,PointsPriviazka[i].MapX*PointsPriviazka[i].MapY);
		   X.SetElem(i,5,PointsPriviazka[i].MapY*PointsPriviazka[i].MapY);
	     }
		 //заполн€ем векторы x и y
         for(i=0;i<m;i++)
	     {
		   Vx.SetElem(i,0,PointsPriviazka[i].Dest);
		   Vy.SetElem(i,0,PointsPriviazka[i].Lon);
	     }
	     Xt=Xt.T(X);      //на обратную матрицу к матрице X
	     R=Xt*X;          //XT*X
		 R=R.Inv(R);      //инвертируем матрицу
		 K=new CMatrix(R.GetRows(),Xt.GetCols());
         *K=R*Xt;        
		 //последний шиг поиска коэ-тов
	     Va=(*K)*Vx;     
   	     Vb=(*K)*Vy;
	     delete K;
         for(i=0;i<6;i++)
	     {
		   a[i+1]= Va(i,0);
		   b[i+1]= Vb(i,0);
	      }
          
          bindingFlag=true;
    }
       catch(CMatrixException &MatrixException)
       {
	      CString ErrorString,MsgStr;
	      ErrorString=MatrixException.PrintMessage();
	      MsgStr="ќшибка при вычислении коэффициентов прив€зки карты.\n “ип ошибки: " +
			    ErrorString+"\n. ласс CLinkage; ‘-ци€ GenerateKoefPriv().";
	      AfxMessageBox (MsgStr, MB_ICONWARNING);
	      bindingFlag=false;
	      if(K!=NULL) delete K;
       }
	//эксперименты
	CPoint p;
	p.x=PointsPriviazka[2].MapX;
	p.y=PointsPriviazka[2].MapY;
	double d,l;
	ConvertMapToGeoCord(p,&d,&l);

	//p=ConvertGeoToMapCord(PointsPriviazka[3].Dest,PointsPriviazka[2].Lon);
	p=ConvertGeoToMapCord(d,l);
	return;
	}

	bindingFlag=false;
 }


//======================================================================================================================
void CLinkage::AddPoint(int MapX, int MapY, double GeoDest, double GeoLon)
{
	TPointPriviazka PointPriviazka;
	PointPriviazka.Dest=GeoDest;
	PointPriviazka.Lon=GeoLon;
	PointPriviazka.MapX=MapX;
	PointPriviazka.MapY=MapY;
	PointsPriviazka.push_back(PointPriviazka);
//	GenerateKoefPriv();
}
//======================================================================================================================
CString* CLinkage::GetListPointPriv(int *count)
{
    //count - кол-во строк в массиве massStr
	if(PointsPriviazka.size()>0)
	{
		CString *massStr=new CString[PointsPriviazka.size()+1];
		unsigned int i;

		for(i=0;i<PointsPriviazka.size();i++)
		{
			massStr[i].Format("%d %d %f %f",PointsPriviazka[i].MapX,PointsPriviazka[i].MapY,
				                            PointsPriviazka[i].Dest,PointsPriviazka[i].Lon);
		}

        *count=PointsPriviazka.size();
		return  massStr;
	}
    *count=0;
	return NULL;
}
//======================================================================================================================
void CLinkage::ConvertMapToGeoCord(CPoint Point, double *GeoDest, double *GeoLon)
{
	if(bindingFlag==true)
	{
            *GeoDest = a[1] + a[2] * Point.x + a[3] * Point.y + a[4] * Point.x*Point.x+
				                 a[5] *Point.x*Point.y + a[6] * Point.y*Point.y;
			*GeoLon  = b[1] + b[2] * Point.x + b[3] * Point.y + b[4] * Point.x*Point.x+
				                 b[5] *Point.x*Point.y + b[6] * Point.y*Point.y;
	}
	else
	{
	  *GeoDest = 0.0;
	  *GeoLon  = 0.0;
	}
}

//======================================================================================================================
CPoint CLinkage::SolveNewtonMethod(double &De,double &Lo)
{
  CPoint MapP;                //результат решени€ системы 
  double ApprX,ApprY;         //точка приближенна€ к корню
  double Epsilon;             //допустима€ погрешность
  double xn,yn;               //текущие значени€ корней(i+1)
  double xp=0,yp=0;           //предыдущие значени€ корней(i)
  int i;

  //точка приближени€- центр карты:
  ApprX=MapRect.Width() /2.0;
  ApprY=MapRect.Height()/2.0;

  Epsilon=0.00001;
  xn=ApprX;
  yn=ApprY;
  i=0;
 
   try
  {
       CMatrix Iacob(2,2);      //матрица якоби     
	   CMatrix IacobRev(2,2);   //обратна€ матрице якоби
	   CMatrix Vf(2,1);         //значение системы функций(F,G)
	   CMatrix Vn(2,1),Vp(2,1); 
	   Vf.SetElem(0,0,0.0);
	   Vf.SetElem(1,0,0.0);

        
	   Vp.SetElem(0,0,0);
       Vp.SetElem(1,0,0);
	   Vn.SetElem(0,0,ApprX);
       Vn.SetElem(1,0,ApprY);
     //итерационный цикл вычислени€
	   while((Epsilon<fabs(Vn(0,0)-Vp(0,0))||Epsilon<fabs(Vn(1,0)-Vp(1,0)))&&
             (Epsilon<fabs(Vf(0,0))||Epsilon<fabs(Vf(1,0))))
	   {
	        Vp=Vn;
            // заполн€ем матрицу якоби
			//a[],b[] - коэ-ты, отсчет с 1
			Iacob.SetElem(0,0,a[2]+2*a[4]*Vp(0,0)+a[5]*Vp(1,0));         
			Iacob.SetElem(0,1,a[3]+2*a[6]*Vp(1,0)+a[5]*Vp(0,0));
			Iacob.SetElem(1,0,b[2]+2*b[4]*Vp(0,0)+b[5]*Vp(1,0));
			Iacob.SetElem(1,1,b[3]+2*b[6]*Vp(1,0)+b[5]*Vp(0,0));
			//обращаем матрицу Gess
			IacobRev=Iacob.Inv(Iacob);
	   
 		    Vf.SetElem(0,0,F1(Vp(0,0),Vp(1,0),De));
		    Vf.SetElem(1,0,F2(Vp(0,0),Vp(1,0),Lo));
		    Vn=Vp-IacobRev*Vf;
 		    i++;if(i>10000) break;
	   }
	   MapP.x=(LONG)Vn(0,0);
	   MapP.y=(LONG)Vn(1,0);
  }
  
  catch(CMatrixException &MatrixException)
  {
	  //обработка ошибок
	   MapP.x=-1;
	   MapP.y=-1;
  }
  return MapP;
}
//======================================================================================================================
CPoint CLinkage::SolveModificNewtonMethod(double &De,double &Lo)
{
  CPoint MapP;
  double ApprX=MapRect.Width()/2.0,ApprY=MapRect.Height()/2.0; //точка приближенна€ к корню
  double Epsilon=0.0001;
  double xn=ApprX,yn=ApprY;  //текущие значени€ корней
  double xp=0,yp=0;  //предыдущие значени€ корней
  double F1p=0.0,F2p=1.0;
  int i=0;

  try
  {
       CMatrix Gess(2,2),GessRev(2,2),Vf(2,1),Vn(2,1),Vp(2,1); 
       // заполн€ем матрицу √ессеана
  	   Gess.SetElem(0,0,a[2]+2*a[4]*ApprX+a[5]*ApprY);         
       Gess.SetElem(0,1,a[3]+2*a[6]*ApprY+a[5]*ApprX);
	   Gess.SetElem(1,0,b[2]+2*b[4]*ApprX+b[5]*ApprY);
	   Gess.SetElem(1,1,b[3]+2*b[6]*ApprY+b[5]*ApprX);
	   //обращаем матрицу Gess
	   GessRev=Gess.Inv(Gess);
       //итерационный цикл вычислени€  
	   Vp.SetElem(0,0,0);
       Vp.SetElem(1,0,0);
	   Vn.SetElem(0,0,ApprX);
       Vn.SetElem(1,0,ApprY);
	   while((Epsilon<fabs(Vn(0,0)-Vp(0,0))||Epsilon<fabs(Vn(1,0)-Vp(1,0)))&&
             (Epsilon<fabs(F1p)||Epsilon<fabs(F2p)))
	   {

	        Vp=Vn;
 		    F1p=F1(Vp(0,0),Vp(1,0),De);
            F2p=F2(Vp(0,0),Vp(1,0),Lo);
		    Vf.SetElem(0,0,F1p);
		    Vf.SetElem(1,0,F2p);
		    Vn=Vp-GessRev*Vf;
 		    i++;
			if(i>100000) break;
	   }
	   MapP.x=(LONG)Vn(0,0);
	   MapP.y=(LONG)Vn(1,0);
  }
  catch(CMatrixException &MatrixException)
  {
 	   MapP.x=-1;
	   MapP.y=-1;
  }
	   return MapP;
}
//======================================================================================================================
double CLinkage::F1IsolateX1(double &y,double &De)
{
  double x;
  if(a[4]*(4*De-4*a[1]-4*a[6]*y*y-4*a[3]*y)+(a[5]*y+a[2])*(a[5]*y+a[2])<0)
	  return -1.0;   //корн€ нет, следовательно возвращает невозможное значение x на карте, т.е. -1
  x = -a[5]*y-a[2];  //-B квадратного ур-ни€
  x+= sqrt(a[4]*(4*De-4*a[1]-4*a[6]*y*y-4*a[3]*y)+(a[5]*y+a[2])*(a[5]*y+a[2])   );//прибввили дискр
  x/= 2*a[4];
  return x;
}
double CLinkage::F1IsolateX2(double &y,double &De)
{
  double x;
  if(a[4]*(4*De-4*a[1]-4*a[6]*y*y-4*a[3]*y)+(a[5]*y+a[2])*(a[5]*y+a[2])<0)
	  return -1.0;   //корн€ нет, следовательно возвращает невозможное значение x на карте, т.е. -1
  x = -a[5]*y-a[2];  //-B квадратного ур-ни€
  x-= sqrt(a[4]*(4*De-4*a[1]-4*a[6]*y*y-4*a[3]*y)+(a[5]*y+a[2])*(a[5]*y+a[2])   );//прибввили дискр
  x/= 2*a[4];
  return x;
}

//======================================================================================================================
double CLinkage::FxToF2(double &y, double &De, double &Lo,int X_Ind)
{
  double F2;
  double x;
  if(X_Ind==1)
   x=F1IsolateX1(y,De);
  else
   x=F1IsolateX2(y,De);
  if(x!=-1.0)
  {
	  
     F2=b[1]+b[2]*x+b[3]*y+b[4]*x*x+b[5]*x*y+b[6]*y*y-Lo;
	    return F2;
  }
  return -1.0;
}
//======================================================================================================================
CPoint CLinkage::SolveDivHalfMethod(double &De,double &Lo)
{
   CPoint MapP;
	double x,y;
	bool flag=false;
	double y1=0.0,y2=MapRect.Height(); //промежуток, на котором ищем корни
	double Epsilon=0.00001;
	int ind;
    unsigned int i=0;
	double FValue1,FValue2;
	double pogr=0.0;
//находим y, выразив x с первого уравнени€ и подставив во второе(ф-ци€ F1xToF2())
while(pogr<2.1)
{
	FValue1=FxToF2(y1,De,Lo,1);
	FValue2=FxToF2(y2,De,Lo,1);

	if ((FValue1*FValue2<0)&&FValue1!=-1&&FValue2!=-1)
	{
		ind=1;
		
		break;
	}
	else
	{
		
       	FValue1=FxToF2(y1,De,Lo,2);
	    FValue2=FxToF2(y2,De,Lo,2);

	    if ((FValue1*FValue2<0)&&FValue1!=-1&&FValue2!=-1)
		{
		   ind=2;
		   break;
		}
		/*else
			return CPoint(-1,-1);*/
	}
	 De=De+0.0001;
	 //Lo=Lo-0.001;
	 pogr+=0.001;
}
if(pogr>2.1)
  return CPoint(-1,-1); 
	if(FxToF2(y2,De,Lo,ind)-FxToF2(y1,De,Lo,ind)>0) flag=true;//провер€ем на поведение ф-ции(возрастание-убывание)
	//y=(y1+y2)/2;
	while(fabs(y1-y2)>=Epsilon)
	{
		y=(y1+y2)/2.0;
		if((FxToF2(y,De,Lo,ind)<0)&&(flag==true)||(FxToF2(y,De,Lo,ind)>0)&&(flag==false))
			y1=y;
		else
		if (FxToF2(y,De,Lo,ind)==0) 
			break; 
		else
			y2=y;
		i++;
		if(i>4000)
			return CPoint(-1,-1);
	}

//дл€ найденного y ищем xif
	if(ind==1)
	  x=F1IsolateX1(y,De);
	else
	  x=F1IsolateX2(y,De);
	MapP.x=(int)x;
	MapP.y=(int)y;
   return MapP;
}
//======================================================================================================================
CPoint CLinkage::ConvertGeoToMapCord(double GeoDest, double GeoLon)
{
  CPoint MapP;

  if(bindingFlag==true&&created==true)
	{
		//CPoint p1,p2;
		//p1=SolveDivHalfMethod(GeoDest,GeoLon);
		MapP=SolveModificNewtonMethod(GeoDest,GeoLon);
        //MapP=p2;
	}
	else
	{
	 MapP.x = -1;
	 MapP.y = -1;
	}

  return MapP;
}
//======================================================================================================================
bool CLinkage::IsBinding()
{
  return bindingFlag;
}
//======================================================================================================================
void CLinkage::AddPointFromStr(CString S)
{
	int ind;
	while((ind=S.Find("  "))!=-1) //удал€ем двойные пробелы
		S.Delete(ind,1);
   if(S=="") return;
   CString buf,Xs,Ys,Des,Los;
   
   Xs=S.SpanExcluding(" ");
   if(Xs=="") return;
   S.Delete(0,Xs.GetLength()+1);

   Ys=S.SpanExcluding(" ");
   if(Ys=="") return;
   S.Delete(0,Ys.GetLength()+1);

   Des=S.SpanExcluding(" ");
   if(Des=="") return;
   S.Delete(0,Des.GetLength()+1);

   Los=S;
   if(Los=="") return;
   AddPoint(atoi(Xs),atoi(Ys),atof(Des),atof(Los));
}
//======================================================================================================================
void     CLinkage::RewriteElem(unsigned int ind,int MapX,int MapY,double GeoDest,double GeoLon)
	{
		if(ind<PointsPriviazka.size())
		{
			PointsPriviazka[ind].MapX=MapX;
			PointsPriviazka[ind].MapY=MapY;
			PointsPriviazka[ind].Dest=GeoDest;
			PointsPriviazka[ind].Lon=GeoLon;

		}
}
//======================================================================================================================
bool     CLinkage::GetPointPriv(TPointPriviazka *PointPriv,unsigned int ind)
{
		if(PointsPriviazka.size()>0&&PointsPriviazka.size()>ind)
		{
		  *PointPriv= PointsPriviazka[ind];
		  return true;
		}
		return false;
}
//======================================================================================================================
void     CLinkage::DeletePoint(unsigned int ind)
{
		if(ind<0||PointsPriviazka.size()==0||PointsPriviazka.size()<(ind+1))
			return;
			PointsPriviazka.erase(PointsPriviazka.begin()+ind);
}
//======================================================================================================================