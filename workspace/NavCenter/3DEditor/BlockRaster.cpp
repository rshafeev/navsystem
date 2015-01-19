#include "StdAfx.h"
#include "BlockRaster.h"
#include <math.h>
#include "afxmt.h "
CBlockRaster::CBlockRaster(CString PathName,CString ProgramPath)
{
  Create(PathName,ProgramPath);
}
CBlockRaster::~CBlockRaster(void)
{
	     FreeMemory();

}
void CBlockRaster::AllImagePtrFree()
{
	int i;
	ImageFree.clear();
       for(i=0;i<(ColOfBlocks.x)*(ColOfBlocks.y);i++)
	    ImageFree.push_back(i);
}
//==========================================================================================================================
bool CBlockRaster::PushBlock(int i, int j,int BeginX,int BeginY,vector<TLoadedFiles> &NotLoad)
{
	   CString file;
	   CString BlockName;
	   bool flag=true;
		       BlockName=GetBlockName(i,j,LEVEL);
			   if(ImageFree.size()==0)
				   return false;
	           ImagePtr[i-BeginX][j-BeginY]=ImageFree[0];
	   	       if(BlockName=="")
			   {
				   if(ImageFree[0]<0) return false;
				   ImageList[ImageFree[0]].Destroy();
				   ImageList[ImageFree[0]].Load(ProgramPath+"dissappear.jpg");
				   flag=false;
			   }
		       else
	           {
	              file=PathName+BlockName+".jpg"; 
 		          ImageList[ImageFree[0]].Destroy();   
			      if(ImageList[ImageFree[0]].Load(file)!=NULL)
				  {
					  file=RES_PATH+"cache\\"+BlockName+".jpg"; 
					  ImageList[ImageFree[0]].Destroy(); 
					  if(ImageList[ImageFree[0]].Load(file)!=NULL)
					  {
					    ImageList[ImageFree[0]].Destroy();
					    ImageList[ImageFree[0]].Load(ProgramPath+"dissappear.jpg");
						flag=false;
					  }
				  }
			   }
			   ImageFree.erase(ImageFree.begin()); 

  if(flag==false)
  {
						TLoadedFiles Add;
						Add.FName=BlockName;
						Add.IndexBlock.x=i;
						Add.IndexBlock.y=j;
						
						NotLoad.push_back(Add);
  }
   return flag;
}
//==========================================================================================================================
void CBlockRaster::AddImage(TLoadedFiles Add)
{
	Push1Section.Lock(2000);
	Push2Section.Lock(2000);
	DrawSection.Lock(2000);
	int x,y;
	CPoint Delta;
	Delta=Add.LeftTopBlock-LeftTopBlock;
	x=Add.IndexBlock.x-LeftTopBlock.x+Delta.x;
	y=Add.IndexBlock.y-LeftTopBlock.y-Delta.y;
	if(MassSize.x<x||MassSize.y<y||x<0||y<0)
	{
	Push1Section.Unlock();
	Push2Section.Unlock();
	DrawSection.Unlock();
		return;
	}
	ImageList[ImagePtr[x][y]].Destroy();
	if(ImageList[ImagePtr[x][y]].Load(RES_PATH+"cache//"+Add.FName+".jpg")!=0)
	{
       ImageList[ImageFree[0]].Destroy(); 
       ImageList[ImagePtr[x][y]].Load(ProgramPath+"dissappear.jpg");
	}
	Push1Section.Unlock();
	Push2Section.Unlock();
	DrawSection.Unlock();
}
//==========================================================================================================================

bool CBlockRaster::ConvertCoordToGeogr(CPoint block, CPoint BlockPoint, int level, double *Lat, double *Lon)
{
	//////////////////////////////////////////////////
	//block - номер блока                           //
	//BlockPoint - координаты мыши на этом блоке    //
	//level - уровень                               //
	//Lat,Lon - возвращаемые зн-я: широта и долгота //
	//////////////////////////////////////////////////
  block.x*=PICTSIZE;
  block.y*=PICTSIZE;
  block+=BlockPoint;

   double z=(block.y - BitmapOrigo(level)) / (-PixelsPerLonRadian(level));
   *Lon = (block.x - BitmapOrigo(level)) / PixelsPerLonDegree(level);
   *Lat = (2 * atan(exp(z)) - M_PI/2.0) * 180.0/M_PI;

return true;
}
//***************************************************************************************************
bool CBlockRaster::Create(CString PathName,CString ProgramPath)
{

  this->PathName=PathName;
  this->ProgramPath=ProgramPath;
  	block.x=0;
	block.y=0;
	Sdvig.x=0;
	Sdvig.y=0;
	
	DrawRect.left=0;
	DrawRect.right=0;
	DrawRect.top=0;
	DrawRect.bottom=0;

  DissapearPict.Load(ProgramPath+"dissappear.jpg");
  	
  LeftTopBlock.x=0;
  LeftTopBlock.y=0;
  LEVEL=1;
  ImageList=NULL;
  ImagePtr=NULL;

  return true;
}
//********************************************************************************************

void CBlockRaster::DrawMatrix(CDC *pDC,CPoint ColRow,CPoint Position)
{
	//row - stroka

DrawSection.Lock(2000);
Push1Section.Lock(2000);
Push2Section.Lock(2000);

 int i,j;
 pDC->Rectangle(-1,-1,WindowRect.Width()+2,WindowRect.Height()+2);
 for(i=0;i<=ColRow.x;i++)
	 for(j=0;j<=ColRow.y;j++)
	 {
	/*	 if(ImageList[ImagePtr[i][j]].GetDC()==NULL)
		 {
			 int BED=55;
		 }*/
		 if(!ImageList[ImagePtr[i][j]].IsNull())
        ImageList[ImagePtr[i][j]].BitBlt(pDC->m_hDC,
			 Position.x+i*PICTSIZE-PICTSIZE/2+(WindowRect.Width()-ColRow.x*PICTSIZE)/2,
			 Position.y+j*PICTSIZE-PICTSIZE/2+(WindowRect.Height()-ColRow.y*PICTSIZE)/2);
	 }
 
	 DrawRect.left=(WindowRect.Width()-ColRow.x*PICTSIZE)/2-PICTSIZE/2+Position.x;
	 DrawRect.top=(WindowRect.Height()-(ColRow.y)*PICTSIZE)/2-PICTSIZE/2+Position.y;

	 DrawRect.right=Position.x+ColRow.x*PICTSIZE-PICTSIZE/2+(WindowRect.Width()-ColRow.x*PICTSIZE)/2+PICTSIZE;
     DrawRect.bottom=Position.y+ColRow.y*PICTSIZE-PICTSIZE/2+(WindowRect.Height()-ColRow.y*PICTSIZE)/2+PICTSIZE;
    
	 DrawSection.Unlock();
	 Push1Section.Unlock();
	 Push2Section.Unlock();
}
//********************************************************************************************
vector<TLoadedFiles> CBlockRaster::PushMatrix()
{
	Push1Section.Lock(2000);
	Push2Section.Lock(2000);
	DrawSection.Lock(2000);
/*struct TMetka
{
   CPoint ind;
   TLoadedFiles info;
   bool NoLoad;
}**Flags;*/
   int i,j;
   int BeginX,EndX;
   int BeginY,EndY;
   
   vector<TLoadedFiles> NotLoad;

   BeginX=max(0,block.x-ColOfBlocks.x/2);
   BeginY=max(0,block.y-ColOfBlocks.y/2);

   EndX  =min((int)STEPENTWO[LEVEL-1]-1,block.x+ColOfBlocks.x/2+abs(block.x-ColOfBlocks.x/2)-BeginX);
   EndY  =min((int)STEPENTWO[LEVEL-1]-1,block.y+ColOfBlocks.y/2+abs(block.y-ColOfBlocks.y/2)-BeginY);
   MassSize.x=EndX-BeginX;
   MassSize.y=EndY-BeginY;


/*   Flags=new TMetka*[EndX+2];
   for(i=0;i<EndX+1;i++)
	   Flags[i]=new TMetka[EndY+2];*/
   LeftTopBlock.x=BeginX;
   LeftTopBlock.y=BeginY;
   AllImagePtrFree();
   CString BlockName;
for(i=BeginX;i<=EndX;i++)
   {
       for(j=BeginY;j<=EndY;j++)
	   {  
		   if(PushBlock(i,j,BeginX,BeginY,NotLoad)==false)
		   {
           /*Flags[i][j].NoLoad=false;
		   Flags[i-BeginX][j-BeginY].info=NotLoad[NotLoad.size()-1];*/
		   }
		/*   else
		   {
			   Flags[i][j].NoLoad=true;
		   }*/
	   }
   }

 //////////////////////////*
/*
NotLoad.clear();


int n=EndX+1;
int m=EndY+1;
int a[340][340],num,r=1;
for(i=0;i<n;i++)
 for(j=0;j<m;j++)
	 a[i][j]=0;

for(i=0;i<n/2+n%2;i++)
{
  for(j=i;j<m-i;j++)
   {
	   	   if(a[i][j]!=0)
		     continue;
       a[i][j]=r;
	  if(Flags[i][j].NoLoad==false)
         NotLoad.push_back(Flags[i][j].info);
	   r++;
   }

   for(j=i+1;j<n-i-1;j++)
   {
	   	   if(a[j][m-i-1]!=0)
		     continue;
       a[j][m-i-1]=r;
	  if(Flags[j][m-i-1].NoLoad==false)
         NotLoad.push_back(Flags[j][m-i-1].info);
	 
	   r++;
   }

  for(j=i;j<m-i;j++)
   {
	 	   if(a[n-i-1][m-1-j]!=0)
		     continue;
		   r=r;
       a[n-i-1][m-1-j]=r;
	   r++;
	  if(Flags[n-i-1][m-1-j].NoLoad==false)
         NotLoad.push_back(Flags[n-i-1][m-i-1].info);

   }
   for(j=i+1;j<n-i-1;j++)
   {
	 	   if(a[n-j-1][i]!=0)
		   continue;
       a[n-j-1][i]=r;
	   r++;
	  if(Flags[n-j-1][i].NoLoad==false)
         NotLoad.push_back(Flags[n-j-1][i].info);

   }
}
 for(i=0;i<EndX+1;i++)
	 delete[] Flags[i];
 delete[] Flags;*/
   ///////////////////////////
	Push1Section.Unlock();
	Push2Section.Unlock();
	DrawSection.Unlock();
   return NotLoad;
}
//********************************************************************************************
CString CBlockRaster::GetBlockName(int NumX,int NumY, int level)
{
 int d;
 int i;            
 CString res;          // имя файла
 
d=STEPENTWO[level-1];  // 2^level-1
//если номер блока отрицательный, берем номер блока с зеркальным отображением
if((NumX < 0)||(d-1 < NumX))
{
	NumX= NumX%d;
	if (NumX < 0) NumX=NumX + d;
} 
res= "t"; //имя первого блока первого уровня
  for( i=2;i<=level;i++)
  {
    d/=2;//на один уровень опускаемся
	//определяем букву блока на данном уровне
	if(NumY < d)
	{
		if(NumX < d)
        res=res + "q";
      else 
	  {
  	    res= res + "r";
		NumX= NumX-d;
	  }
	}
    else
	{
		if(NumX < d)
         res= res + 't';
        else
		{
			res= res + "s";
			NumX=NumX - d;
		}
		NumY= NumY - d;
   }
   
  }
  return res;
}
//********************************************************************************************
double CBlockRaster::BitmapOrigo(int level)
{
  return STEPENTWO[level-1]* PICTSIZE/2;
}
//********************************************************************************************
double CBlockRaster::PixelsPerLonDegree(int level)
{
  return STEPENTWO[level-1]* PICTSIZE/360.0;
}
//********************************************************************************************
double CBlockRaster::PixelsPerLonRadian(int level)
{
	return STEPENTWO[level-1]* PICTSIZE/(2*M_PI);
}
//********************************************************************************************
/*CPoint CBlockRaster::GetBlockInd(int x,int y)
{
  CPoint p;
  p.x=x/PICTSIZE;
  p.y=y/PICTSIZE;
  return p;
}*/
CPoint CBlockRaster::ConvertGeogrToBlock(double Lat, double Lon,int level)
{
  CPoint result; 
  double z=sin (Lat/180.0*M_PI);
  result.x =(LONG) floor (BitmapOrigo(level) + Lon * PixelsPerLonDegree(level));
  result.y =(LONG) floor (BitmapOrigo(level) - 0.5 * log((1.0+z)/(1.0-z)) * PixelsPerLonRadian(level));
  result.x/=PICTSIZE;
  result.y/=PICTSIZE;

  return result;
}
//********************************************************************************************
bool CBlockRaster::ConvertBlockToGeogr(CPoint block,double *Lat,double *Lon,int level)
{
  block.x*=PICTSIZE;
  block.y*=PICTSIZE;
   double z=(block.y - BitmapOrigo(level)) / (-PixelsPerLonRadian(level));
   *Lon = (block.x - BitmapOrigo(level)) / PixelsPerLonDegree(level);
   *Lat = (2 * atan(exp(z)) - M_PI/2) * 180/M_PI;
   return true;
}
CPoint CBlockRaster::ConvertGeogrToScreen(double Lat, double Lon, int level)
{
  CPoint result; //координаты на мировой карте
  CPoint delta;//разность м/у клиентсвой областью окна и DrawRect
  double z=sin (Lat/180.0*M_PI);
  
  
  result.x =(LONG) floor (BitmapOrigo(level) + Lon * PixelsPerLonDegree(level));
  result.y =(LONG) floor (BitmapOrigo(level) - 0.5 * log((1.0+z)/(1.0-z)) * PixelsPerLonRadian(level));
 //вычисляем клиентские координаты данного места на области видимости
  delta.x=(DrawRect.left);
  delta.y=(DrawRect.top);

  result.x-=LeftTopBlock.x*PICTSIZE;
  result.y-=LeftTopBlock.y*PICTSIZE;
  result+=delta;

  return result;

}
void CBlockRaster::FreeMemory()
{
	int i;
	
	if(ImageList!=NULL)
	for(i=0;i<ColOfBlocks.x;i++)
		ImageList[i].Destroy();
	delete[] ImageList;
	ImageList=NULL;
	if(ImagePtr!=NULL)
	{
      for(i=0;i<ColOfBlocks.x;i++)
		  if(ImagePtr[i]!=NULL)
		  {
		    delete [] ImagePtr[i];
		    ImagePtr[i]=NULL;
		  }
      delete[]  ImagePtr;
	}
	ImageFree.clear();

	/*
	ImageList=new CImage[(ColOfBlocks.x+1)*(ColOfBlocks.y+1)];
	ImagePtr=new short*[ColOfBlocks.x+1];
	for(i=0;i<ColOfBlocks.x;i++)
      ImagePtr[i]=new short[ColOfBlocks.y+1];
	for(i=0;i<ColOfBlocks.x;i++)
		for(j=0;j<ColOfBlocks.y;j++)
           ImagePtr[i][j]=i*((short)ColOfBlocks.x)+j;
    for(i=0;i<(ColOfBlocks.x)*(ColOfBlocks.y);i++)
	   ImageFree.push_back(i);
	*/
}
//***********************************************************************************************************
void CBlockRaster::ResizeMemory(CPoint NewColBlocks)
{
	
	int i,j;
//очищаем
	FreeMemory();
//создаем новую
	
	ColOfBlocks=NewColBlocks;
    if(ColOfBlocks.x%2==0)ColOfBlocks.x+=1;
	if(ColOfBlocks.y%2==0)ColOfBlocks.y+=1;

   if(ImageList==NULL)
	ImageList=new CImage[(ColOfBlocks.x+1)*(ColOfBlocks.y+1)];
	ImagePtr=new short*[ColOfBlocks.x+1];
	for(i=0;i<ColOfBlocks.x;i++)
      ImagePtr[i]=new short[ColOfBlocks.y+1];
	for(i=0;i<ColOfBlocks.x;i++)
		for(j=0;j<ColOfBlocks.y;j++)
           ImagePtr[i][j]=i*((short)ColOfBlocks.x)+j;
    for(i=0;i<(ColOfBlocks.x)*(ColOfBlocks.y);i++)
	   ImageFree.push_back(i);


}
//*********************************************************************************************************
vector<TLoadedFiles> CBlockRaster::PushMatrixWithCopy(CPoint delta)
{
Push2Section.Lock(1000);
  int i,j;
  vector<TLoadedFiles> NotLoad;
  if(delta.x==0&&delta.y==0)
  {
	  Push2Section.Unlock();	  
	  return NotLoad;
  }
//  if((abs(delta.x)>ColOfBlocks.x)||(abs(delta.y)>ColOfBlocks.y))
  {
		//все блоки надо переписать, помечаем их:
	  AllImagePtrFree();
	/*	ImageFree.clear();
	    for(i=0;i<ColOfBlocks.x*ColOfBlocks.y;i++)
		  ImageFree.push_back(i);*/
        for(i=0;i<ColOfBlocks.x;i++)
		    for(j=0;j<ColOfBlocks.y;j++)
				ImagePtr[i][j]=-1;               
		
  }
 /* else
/////////////////
  //оставляем те метки, которые 
    if(delta.x>=0&&delta.y>0)
   {
   	for(i=0;i<ColOfBlocks.x-delta.x;i++)
		for(j=0;j<ColOfBlocks.y-delta.y;j++)
                {
                    ImagePtr[i][j]=ImagePtr[i+delta.x][j+delta.y];
                }

        for(i=ColOfBlocks.x-delta.x;i<ColOfBlocks.x;i++)
                for(j=0;j<ColOfBlocks.y;j++)
				{
					//if(ImagePtr[i][j]!=-1)
                    ImageFree.push_back(ImagePtr[i][j]);
		            ImagePtr[i][j]=-1;
				}
        for(i=0;i<ColOfBlocks.x;i++)
                for(j=ColOfBlocks.y-delta.y;j<ColOfBlocks.y;j++)
				{
					if(ImagePtr[i][j]!=-1)
                     ImageFree.push_back(ImagePtr[i][j]);
					 ImagePtr[i][j]=-1;
				}
	}
   else
   if(delta.x>=0&&delta.y<=0)
   {
	    for(i=0;i<ColOfBlocks.x-delta.x;i++)
		   for(j=0;j<ColOfBlocks.y+delta.y;j++)
                {
                        ImagePtr[i][ColOfBlocks.y-j-1]=ImagePtr[i+delta.x][ColOfBlocks.y-abs(delta.y)-j-1];
                }

        for(i=0;i<ColOfBlocks.x;i++)
                for(j=0;j<abs(delta.y);j++)
     			{
				//if(ImagePtr[i][j]!=-1)
                     ImageFree.push_back(ImagePtr[i][j]);
					 ImagePtr[i][j]=-1;
				}
        for(i=ColOfBlocks.x-delta.x;i<ColOfBlocks.x;i++)
                for(j=abs(delta.y)-1;j<ColOfBlocks.y;j++)
    			{
					if(ImagePtr[i][j]!=-1)
					{
                      ImageFree.push_back(ImagePtr[i][j]);
					  ImagePtr[i][j]=-1;
					}
				}

   }
   else
   if(delta.x<=0&&delta.y>=0)
   {
   	for(i=0;i<ColOfBlocks.x-abs(delta.x);i++)
		for(j=ColOfBlocks.y;j>=delta.y;j--)
                {
                    ImagePtr[i][j]=ImagePtr[i+abs(delta.x)][j];
                }

        for(i=0;i<abs(delta.x);i++)
                for(j=0;j<ColOfBlocks.y;j++)
				{
					if(ImagePtr[i][j]!=-1)
                     ImageFree.push_back(ImagePtr[i][j]);
					 ImagePtr[i][j]=-1;
				}
        for(i=0;i<ColOfBlocks.x;i++)
                for(j=ColOfBlocks.y-abs(delta.y);j<ColOfBlocks.y;j++)
				{
					if(ImagePtr[i][j]!=-1)
                     ImageFree.push_back(ImagePtr[i][j]);
					 ImagePtr[i][j]=-1;
				}
   }
   else
   if(delta.x<=0&&delta.y<=0)
   {
   	for(i=ColOfBlocks.x-1;i>=abs(delta.x);i--)
		for(j=ColOfBlocks.y-1;j>=abs(delta.y);j--)
                {
                    ImagePtr[i][j]=ImagePtr[i-abs(delta.x)][j-abs(delta.y)];
                }

        for(i=0;i<abs(delta.x);i++)
                for(j=0;j<ColOfBlocks.y;j++)
				{
					if(ImagePtr[i][j]!=-1)
                     ImageFree.push_back(ImagePtr[i][j]);
					 ImagePtr[i][j]=-1;
				}
        for(i=0;i<ColOfBlocks.x;i++)
                for(j=0;j<abs(delta.y);j++)
				{
					if(ImagePtr[i][j]!=-1)
                     ImageFree.push_back(ImagePtr[i][j]);
					 ImagePtr[i][j]=-1;
				}
   }
   */
 ////////////////////
/*
struct TMetka
{
   CPoint ind;
   TLoadedFiles info;
   bool NoLoad;
}**Flags;
*/
   CString file,BlockName;
      int BeginX,EndX;
   int BeginY,EndY;

   BeginX=max(0,block.x-ColOfBlocks.x/2);
   BeginY=max(0,block.y-ColOfBlocks.y/2);
 
 /*  Flags=new TMetka*[MassSize.x+2];
   for(i=0;i<MassSize.x+1;i++)
	   Flags[i]=new TMetka[MassSize.y+2];
*/
   EndX  =min((int)STEPENTWO[LEVEL-1]-1,block.x+ColOfBlocks.x/2+abs(block.x-ColOfBlocks.x/2)-BeginX);
   EndY  =min((int)STEPENTWO[LEVEL-1]-1,block.y+ColOfBlocks.y/2+abs(block.y-ColOfBlocks.y/2)-BeginY);
   MassSize.x=EndX-BeginX;
   MassSize.y=EndY-BeginY;

   LeftTopBlock.x=BeginX;
   LeftTopBlock.y=BeginY;

for(i=BeginX;i<=EndX;i++)
   {
       for(j=BeginY;j<=EndY;j++)
	   {  
		   if(PushBlock(i,j,BeginX,BeginY,NotLoad)==false)
		   {
      /*     Flags[i-BeginX][j-BeginY].NoLoad=false;
		   Flags[i-BeginX][j-BeginY].info=NotLoad[NotLoad.size()-1];
	*/	   }
	/*	   else
		   {
			   Flags[i-BeginX][j-BeginY].NoLoad=true;
		   }*/
	   }
   }
 //////////////////////////*
/*
NotLoad.clear();


int n=MassSize.x+1;
int m=MassSize.y+1;
int a[34][34],num,r=1;
for(i=0;i<n;i++)
 for(j=0;j<m;j++)
	 a[i][j]=0;

for(i=0;i<n/2+n%2;i++)
{
  for(j=i;j<m-i;j++)
   {
	   	   if(a[i][j]!=0)
		     continue;
       a[i][j]=r;
	  if(Flags[i][j].NoLoad==false)
         NotLoad.push_back(Flags[i][j].info);
	   r++;
   }

   for(j=i+1;j<n-i-1;j++)
   {
	   	   if(a[j][m-i-1]!=0)
		     continue;
       a[j][m-i-1]=r;
	  if(Flags[j][m-i-1].NoLoad==false)
         NotLoad.push_back(Flags[j][m-i-1].info);
	 
	   r++;
   }

  for(j=i;j<m-i;j++)
   {
	 	   if(a[n-i-1][m-1-j]!=0)
		     continue;
		   r=r;
       a[n-i-1][m-1-j]=r;
	   r++;
	  if(Flags[n-i-1][m-1-j].NoLoad==false)
         NotLoad.push_back(Flags[n-i-1][m-i-1].info);

   }
   for(j=i+1;j<n-i-1;j++)
   {
	 	   if(a[n-j-1][i]!=0)
		   continue;
       a[n-j-1][i]=r;
	   r++;
	  if(Flags[n-j-1][i].NoLoad==false)
         NotLoad.push_back(Flags[n-j-1][i].info);

   }
}
 for(i=0;i<MassSize.x+1;i++)
	 delete[] Flags[i];
 delete[] Flags;*/
   ///////////////////////////
/*
         for(i=0;i<ColOfBlocks.x;i++)
		    for(j=0;j<ColOfBlocks.y;j++)
				if(ImagePtr[i][j]==-1)
				{ 
   	               file=GetBlockName(i+block.x,j+block.y,LEVEL);
		           file.Delete(0,1);
	               ImagePtr[i][j]=ImageFree[0];
	   	           if(file=="")
			       {
  		                ImageList[ImageFree[0]].Destroy();
				        ImageList[ImageFree[0]].Load(ResPath+"dissappear.jpg");
			       }
		           else
	               {
	                  file=PathName+file+".jpg"; 
 		              ImageList[ImageFree[0]].Destroy();   
			          if(ImageList[ImageFree[0]].Load(file)!=NULL)
				      {
					    ImageList[ImageFree[0]].Destroy();
					    ImageList[ImageFree[0]].Load(ResPath+"dissappear.jpg");
				      }
			       }
				   if(ImageFree.size()!=0)
			       ImageFree.erase(ImageFree.begin()); 
				}*/
   Push2Section.Unlock();
   return NotLoad;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//********************************************************************************************
CPoint CBlockRaster::GetMassSize()
{
   return MassSize;
}
void CBlockRaster::SetBlock(CPoint NewBlock)
{
	this->block=NewBlock;
}
//********************************************************************************************

int CBlockRaster::GetLevel()
{
   return LEVEL;
}
void CBlockRaster::SetLevel(int level)
{
   if(level<1)level=1;
   if(level>20)level=20;
   LEVEL=level;
}
//********************************************************************************************
CPoint CBlockRaster::GetBlock()
{
  return block;
}
//********************************************************************************************
CPoint CBlockRaster::GetSdvig()
{
  return Sdvig;
}
void CBlockRaster::SetSdvig(CPoint NewSdvig)
{
	
  this->Sdvig=NewSdvig;
}
CPoint CBlockRaster::GetLeftTopBlock()
{
  return LeftTopBlock;
}
CRect CBlockRaster::GetDrawRect()
{
	
  return DrawRect;
}
CRect CBlockRaster::GetWindRect()
{
  return WindowRect;
}
void CBlockRaster::SetWindRect(CRect NewWindRect)
{
  WindowRect=NewWindRect;
}
void CBlockRaster::SetMassSize(CPoint size)
{
	this->MassSize=size;
}
void CBlockRaster::SetColOfBlocks(CPoint size)
{
	ColOfBlocks=size;

}
CPoint CBlockRaster::GetColOfBlocks()
{
  return ColOfBlocks;
}