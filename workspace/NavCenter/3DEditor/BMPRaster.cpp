#include "stdafx.h"
#include "BMPRaster.h"
#include "Memory.h"
#include <atlimage.h>

CBMPRaster::CBMPRaster()
{

   m_pData=NULL;
   m_pBmpBMI=NULL;
   m_BmpData=NULL;
   LoadFlag=false;
   MiniMapMass=NULL;
   project_type=false;
   this->Output.left=this->Output.right=this->Output.bottom=this->Output.top=0;
}

CBMPRaster::~CBMPRaster()
{
   Clear();
   MemoryF.Close();
};
//**********************************************************************************************************
bool CBMPRaster::Load(CString FileName, CRect WindRect)
{
   CFile File;
   
   if(GetExp(FileName)=="rsmap")
   {
     project_type=true;
	 FileName=GetPuth(FileName)+"\\"+GetFileNameWithoutExp(FileName)+".bmp";
   }
   
   //ќчистим
   this->FileName=FileName;
    WindowRect=WindRect;
   //ќткрываем файл
	if(LoadFlag==true)
	 Clear();

	if(IsFileExists(FileName)==false)   throw CRasterException(CRasterException::ceFileExists);
	if(IsUsingFile(FileName)==true)     throw CRasterException(CRasterException::ceFileUsing);
	if(File.Open(FileName, CFile::modeRead)!=TRUE){throw CRasterException(CRasterException::ceLoad); return FALSE;}
    
   ////////////////////////////////////////////////////////
   //«агружаем изображение
   //„итаем заголовок файла. Ёто дает его размер и положение
   //начала данных
   BITMAPFILEHEADER   FI;
   File.Read(&FI, sizeof(BITMAPFILEHEADER));

   //ѕровер€ем, Windows Bitmap изображение ?
   if(FI.bfType!=0x4D42)
   { File.Close(); return FALSE;}

   //—мещаем позицию
   File.Seek(sizeof(BITMAPFILEHEADER), CFile::begin);

   //—читаем, что все от заголовка файла до начала растровых данных
   //есть BITMAPINFO
   //¬ыдел€ем пам€ть под заголовок
   m_pBmpBMI=(LPBITMAPINFO)new BYTE[FI.bfOffBits-sizeof(BITMAPFILEHEADER)];
   //m_pMiniBMI=(LPBITMAPINFO)new BYTE[FI.bfOffBits-sizeof(BITMAPFILEHEADER)];

   if(m_pBmpBMI==NULL) { File.Close(); return FALSE;}
   //„итаем BITMAPINFO
   File.Read(m_pBmpBMI, FI.bfOffBits-sizeof(BITMAPFILEHEADER));

   //”меем работать только с несжатыми данными
   if(m_pBmpBMI->bmiHeader.biCompression!=0)
      { File.Close(); return FALSE;}
/*   File.Seek(FI.bfOffBits, CFile::begin);
 if(m_pBMI->bmiHeader.biSizeImage==0)
      m_pBMI->bmiHeader.biSizeImage=BYTESPERLINE(m_pBMI->bmiHeader.biWidth,
                                                 m_pBMI->bmiHeader.biBitCount)*m_pBMI->bmiHeader.biHeight;
*/  
   File.Close();
   MemoryF.Open(FileName,CFile::modeRead,false);
   m_pData=(PBYTE)MemoryF.GetMemoryMapping(true);
   m_pData=m_pData+FI.bfOffBits;
   
   m_BmpData=new BYTE[WindRect.Height()*(WindRect.Width()+WindRect.Width()%2)*m_pBmpBMI->bmiHeader.biBitCount/8];
   
 	m_pBMI.width=m_pBmpBMI->bmiHeader.biWidth;   
	m_pBMI.height=m_pBmpBMI->bmiHeader.biHeight;   
	m_pBMI.bit=m_pBmpBMI->bmiHeader.biBitCount;   
	PictRect.left=0;
	PictRect.right=m_pBMI.width;
	PictRect.top=0;
	PictRect.bottom=m_pBMI.height;
	WindowRect=SizeNormalize(PictRect,WindowRect);


       m_pBmpBMI->bmiHeader.biWidth=WindRect.Width();
	   m_pBmpBMI->bmiHeader.biHeight=WindRect.Height();
	   m_pBmpBMI->bmiHeader.biPlanes=1;
	   m_pBmpBMI->bmiHeader.biSizeImage=WindRect.Width()*WindRect.Height()*m_pBmpBMI->bmiHeader.biBitCount/8;
	   m_pBmpBMI->bmiHeader.biCompression = BI_RGB; 
	   
	   
   Rop=SRCCOPY ;//SRCPAINT SRCCOPY

   LoadFlag=true;
  
   return true;
}
//**********************************************************************************************************
void CBMPRaster::Draw(CRect Output, CDC *dc,CPoint pos)
{
//   int i,j,s=0;




//   Output=SizeNormalize(PictRect,Output);
	if((Output.left)+WindowRect.Width()>(int)m_pBMI.width-90)
	{
	   Output.left=m_pBMI.width - WindowRect.Width()-90;
	   Output.right=Output.left+WindowRect.Width();
	}
	if(Output.top+WindowRect.Height()>(int)m_pBMI.height-1)
	{
	   Output.top=m_pBMI.height - WindowRect.Height()-1;
	   Output.bottom=Output.top+WindowRect.Height();
	}
	if(Output.top<3)
	{
	   Output.top=2;
	   Output.bottom=Output.top+WindowRect.Height();
	}
	if(Output.left<2)
	{
		Output.left=1;
		Output.right=Output.left+WindowRect.Width();
	}
	/*if(pos.y+Output.top+WindowRect.Height()>m_pBMI.height-70)
	   Output.top=m_pBMI.height- WindowRect.Height()-90;
*/
   this->Output.left=this->Output.right=this->Output.bottom=this->Output.top=0;
   PushMass(this->Output,Output);
   this->Output=Output;


	
   StretchDIBits(dc->GetSafeHdc(),      // дескриптор контекста устройства
		             pos.x,pos.y,     // позици€ в области назначени€
				   WindowRect.Width()+WindowRect.Width()%2,WindowRect.Height(),   // размеры обл. назначени€
                   0,0,   //позици€ в исходной области
                   WindowRect.Width()+WindowRect.Width()%2,WindowRect.Height(), //размеры исх. обл.
				   m_BmpData,  //данные
                   m_pBmpBMI,   //заголовок растра
                   DIB_RGB_COLORS,   //опции
                   SRCCOPY );     //код растровой операции

}
//**********************************************************************************************************
 void CBMPRaster::Resize(CRect WindRect)
{
WindowRect=WindRect;
WindowRect=SizeNormalize(PictRect,WindowRect);
    if(LoadFlag==true)
   {
       m_pBmpBMI->bmiHeader.biWidth=WindowRect.Width();
	   m_pBmpBMI->bmiHeader.biHeight=WindowRect.Height();
	   m_pBmpBMI->bmiHeader.biSizeImage=WindowRect.Width()*WindowRect.Height()*m_pBmpBMI->bmiHeader.biBitCount/8;
    
	   if(m_BmpData!=NULL) delete []m_BmpData;
	   m_BmpData=new BYTE[WindowRect.Height()*WindowRect.Width()*m_pBmpBMI->bmiHeader.biBitCount/8*2+1];
	  // Output.left=Output.right=Output.bottom=Output.top=0;
   }
}
//**********************************************************************************************************
void CBMPRaster::Clear()
{
  /*if(m_pData!=NULL) delete[] m_pData;
  m_pData=NULL;*/
  if(m_pBmpBMI!=NULL) delete[] m_pBmpBMI;
  m_pBmpBMI=NULL;
  if(m_BmpData!=NULL) delete[] m_BmpData;
  m_BmpData=NULL;
  
  if(LoadFlag==true)
	  MemoryF.ReleaseMemoryMapping();


}
//**********************************************************************************************************
void CBMPRaster::PushMass(CRect oldRect,CRect newRect)
{
  int i,j,k=0,s;
  CPoint delta;
  PBYTE ptr;
  int Bits=m_pBMI.bit/8;
  int Rw,Rh;
  int musor=newRect.Width() % 4;//добавл€ем в конец ненужные байты чтобы сохранилс€ правильно размер картинки
  LONG *h=&m_pBmpBMI->bmiHeader.biHeight;
  LONG *w=&m_pBmpBMI->bmiHeader.biWidth;

  delta.x=newRect.left-oldRect.left;
  delta.y=newRect.top-oldRect.top;
  if(newRect.right>m_pBMI.width||newRect.bottom>m_pBMI.height)
  {
    Rw=newRect.right=m_pBMI.width;
    Rh=newRect.bottom=m_pBMI.height;
	this->Output=newRect;
  }
  else
  {
    Rw=newRect.right;
    Rh=newRect.bottom;

  }
  if(delta.x==0&&delta.y==0) return;
  else
	  if((oldRect.Width()==0&&oldRect.Height()==0)||(abs(delta.x)>WindowRect.Width())||(abs(delta.y)>WindowRect.Height()))
  {
     for(j=newRect.top;j<Rh;j++)   	
     {   	 
	   for(i=newRect.left;i<Rw;i++)
       { 
		   ptr=&m_pData[BYTESPERLINE(m_pBMI.width,m_pBMI.bit)*j + i*Bits];
           for(s=0;s<Bits;s++)
		   { 
             m_BmpData[k+s]=*ptr;
			 ptr++;
		   } 
		   k+=Bits;
	    }
       k+=musor;

      }

  }
  else
   if(delta.x>=0&&delta.y<=0)
   {
	   int dy=abs(delta.y);
	   int dx=delta.x;
	   for(i=0;i<*w-dx;i++)
	     for(j=0;j<*h-dy;j++)
		 {
			  for(s=0;s<Bits;s++)
		      { 
               m_BmpData[BYTESPERLINE(*w,m_pBMI.bit)*(*h-j) + i*Bits+s]=m_BmpData[BYTESPERLINE(*w,m_pBMI.bit)*(*h-j-dy)+
				                                                                               (i+dx)*Bits+s];
		       } 
		   }
		 k=0;
   /* for(j=newRect.top;j<newRect.top+dy;j++)   	
     {   	 
	   for(i=newRect.left;i<newRect.right;i++)
       { 
		   ptr=&m_pData[BYTESPERLINE(m_pBMI.width,m_pBMI.bit)*j + i*Bits];
           for(s=0;s<Bits;s++)
		   { 
             m_BmpData[BYTESPERLINE(*w,m_pBMI.bit)*(j-newRect.top) + (i-newRect.left)*Bits+s+j*musor]=50;
			 ptr++;
		   } 
		   //k+=Bits;
	    }
      // k+=musor;

      }*/
   }
   /*
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
   }*/
  //       RefreshMemory();
}
CRect CBMPRaster::SizeNormalize(CRect pict, CRect Output)
{
	if(Output.left>pict.right||Output.left<0) Output.left=pict.left;
	if(Output.right>pict.right)               Output.right=pict.right;
	if(Output.top>pict.bottom||Output.top<0)    Output.top=pict.top;
	if(Output.bottom>pict.bottom)             Output.bottom=pict.bottom;
   
	return Output;

}
void CBMPRaster::DrawMiniMap(CDC *dc,CPoint pos)
{
  if(project_type==false)
  {
	m_pBmpBMI->bmiHeader.biWidth=MiniMapSize.Width()+MiniMapSize.Width()%2;   
	m_pBmpBMI->bmiHeader.biHeight=MiniMapSize.Height();   
	m_pBmpBMI->bmiHeader.biSizeImage=m_pBmpBMI->bmiHeader.biWidth*m_pBmpBMI->bmiHeader.biHeight*m_pBMI.bit/8;

  if(MiniMapMass!=NULL)
	{
  	   StretchDIBits(dc->GetSafeHdc(),      // дескриптор контекста устройства
		             pos.x,pos.y,     // позици€ в области назначени€
				   MiniMapSize.Width(),MiniMapSize.Height(),   // размеры обл. назначени€
                   0,0,   //позици€ в исходной области
				   MiniMapSize.Width(),MiniMapSize.Height(),//размеры исх. обл.
				   MiniMapMass,  //данные
                   m_pBmpBMI,   //заголовок растра
                   DIB_RGB_COLORS,   //опции
                   SRCCOPY);     //код растровой операции
	}
       m_pBmpBMI->bmiHeader.biWidth=WindowRect.Width();
	   m_pBmpBMI->bmiHeader.biHeight=WindowRect.Height();
	   m_pBmpBMI->bmiHeader.biSizeImage=WindowRect.Width()*WindowRect.Height()*m_pBmpBMI->bmiHeader.biBitCount/8;
  }
  else
  {
	  MixedMiniMap->BitBlt(dc->m_hDC,pos.x,pos.y);
	
  }
}
///////*************************************************************************************************************
void CBMPRaster::DrawMiniMap(HDC hDC,CPoint pos)
{

	m_pBmpBMI->bmiHeader.biWidth=MiniMapSize.Width()+MiniMapSize.Width()%2;   
	m_pBmpBMI->bmiHeader.biHeight=MiniMapSize.Height();   
	m_pBmpBMI->bmiHeader.biSizeImage=m_pBmpBMI->bmiHeader.biWidth*m_pBmpBMI->bmiHeader.biHeight*m_pBMI.bit/8;

  if(MiniMapMass!=NULL)
	{
  	   StretchDIBits(hDC,      // дескриптор контекста устройства
		             pos.x,pos.y,     // позици€ в области назначени€
				   MiniMapSize.Width(),MiniMapSize.Height(),   // размеры обл. назначени€
                   0,0,   //позици€ в исходной области
				   MiniMapSize.Width(),MiniMapSize.Height(),//размеры исх. обл.
				   MiniMapMass,  //данные
                   m_pBmpBMI,   //заголовок растра
                   DIB_RGB_COLORS,   //опции
                   SRCCOPY);     //код растровой операции
	}
       m_pBmpBMI->bmiHeader.biWidth=WindowRect.Width();
	   m_pBmpBMI->bmiHeader.biHeight=WindowRect.Height();
	   m_pBmpBMI->bmiHeader.biSizeImage=WindowRect.Width()*WindowRect.Height()*m_pBmpBMI->bmiHeader.biBitCount/8;
 
}
//********************************************************************************************************************
CRect CBMPRaster::SetMiniMap(CRect MapSize)
{
  if(project_type==true)
  {
		CRect r;


	  MixedMiniMap=new CImage; 
		MixedMiniMap->Load(GetPuth(FileName)+"\\"+GetFileNameWithoutExp(FileName)+".minmap");
//		if(MixedMiniMap->IsNull()!=0)
		{
		  int h=MixedMiniMap->GetHeight();
		  int w=MixedMiniMap->GetWidth();
		  r.left=0;r.right=w+5;
		r.top=0;r.bottom=h+5;
		}
	//	else
//			return r;

		return r;
  }
  CRect r;
  int i,j,s,a,b;
  int koef=m_pBMI.width/MapSize.Width();
  
  short Bits=m_pBMI.bit/8;
  long int MiddleColor[5];
  PBYTE ptr;
  MixedMiniMap=new CImage; 
  //TColor MiddleColor;
  // koef=1;
  MiniMapSize=MapSize;
  MiniMapSize.left=0;
  MiniMapSize.right=m_pBMI.width/koef;
  MiniMapSize.top=0;
  MiniMapSize.bottom=m_pBMI.height/koef;

  int w=MiniMapSize.Width()+MiniMapSize.Width()%2;

  if(MiniMapMass!=NULL) delete []MiniMapMass;
  MiniMapMass=new BYTE[MiniMapSize.Height()*(MiniMapSize.Width()+MiniMapSize.Width()%2)*Bits];
  
  int N=koef*koef/1;
  if(N<=0)N=1;
  
  for(s=0;s<Bits;s++)
     MiddleColor[s]=0;
     for(j=0;j<MiniMapSize.Height();j++)
     for(i=0;i<MiniMapSize.Width();i++)
  
	 {
		    for(b=j*koef;b<(j+1)*koef;b+=1)
              for(a=i*koef;a<(i+1)*koef;a+=1)
		    {
                    ptr=&m_pData[BYTESPERLINE(m_pBMI.width,m_pBMI.bit)*b + a*Bits];
					for(s=0;s<Bits;s++)
					{
					  MiddleColor[s]+=ptr[s];
					}
			}
			for(s=0;s<Bits;s++)
			{
					  MiniMapMass[w*Bits*j+i*Bits+s]=(BYTE)MiddleColor[s]/N;
					  MiddleColor[s]=0;
			}
		    
	 }
	 
  ptr=NULL;
 // MixedMiniMap->Create(1000,1000,Bits,0);
  MixedMiniMap->CreateEx(MiniMapSize.Width(),MiniMapSize.Height(),24,BI_RGB);
  //MixedMiniMap->Load(RES_PATH+"dissappear.jpg");
  DrawMiniMap(MixedMiniMap->GetDC(),CPoint(0,0));
  CString File=GetPuth(FileName)+"\\"+GetFileNameWithoutExp(FileName)+".minmap";
  MixedMiniMap->Save(_T(File),Gdiplus::ImageFormatBMP);
  return MiniMapSize;   
}
void CBMPRaster::Free()
{
   Clear();
  if(LoadFlag==true)
   MemoryF.Close();
}
void CBMPRaster::RefreshMemory()
{
  //MemoryF.Close();
  MemoryF.ReleaseMemoryMapping();
  //MemoryF.Open(FileName,CFile::modeRead,false);
  m_pData=(PBYTE)MemoryF.GetMemoryMapping(true);
  m_pData=m_pData+54;
 
}