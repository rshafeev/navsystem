#include "StdAfx.h"
#include "MapFrame.h"
#include "afx.h"
#include "BMPRaster.h"
#include "3DEditor.h"
#include "DlgPrivAdd.h"
#include "Geography.h"

IMPLEMENT_DYNAMIC(CMapFrame, CMDIChildWnd)
BEGIN_MESSAGE_MAP(CMapFrame, CMDIChildWnd)
		ON_WM_CREATE()
		ON_WM_KEYDOWN()
		ON_WM_SIZE()
		ON_WM_PAINT()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_MOUSEMOVE()
		ON_WM_CLOSE()
		ON_WM_SETFOCUS()
END_MESSAGE_MAP()
CMapFrame::CMapFrame(void)
{
	WindowType=1;//тип окна "растровая карта"
}
CMapFrame::CMapFrame(CString File)
{
	FileName=File;
	Mouse.flag=false;
	pDC=NULL;
	Raster=NULL;
	WindowType=1;//тип окна "растровая карта"
 
}
//================================================================================================================================
void CMapFrame::FindAbonent(TGPSPoint *Position)
{
	   CRaster::TPictInfo pictd;
      CPoint MapCoord;
      CRect RectDraw;

	  MapCoord=Linkage.ConvertGeoToMapCord(Position->Description,
						                                 Position->Longitude);
	  pictd=Raster->GetPictInfo();
	  if(MapCoord.x<0)MapCoord.x=0;
	  if(MapCoord.y<0)MapCoord.y=0;
	  if(MapCoord.x>(int)pictd.width)  MapCoord.x=(int)pictd.width-RectWnd.Width();
	  if(MapCoord.y>(int)pictd.height) MapCoord.y=(int)pictd.height-RectWnd.Height();	  
       
		RectDraw.left   = MapCoord.x-RectWnd.Width()/2;
		RectDraw.right  = MapCoord.x+RectWnd.Width()/2+RectWnd.Width()%2;
		RectDraw.top    = pictd.height - MapCoord.y-RectWnd.Height()/2;
		RectDraw.bottom = pictd.height - MapCoord.y+RectWnd.Height()/2+RectWnd.Height()%2;

		Raster->Draw(CRect(RectDraw.left,RectDraw.top,
		                   RectDraw.right,RectDraw.bottom),&dcMemory,CPoint(0,0));
		CClientDC dc(this);
		dc.BitBlt(0,0,RectWnd.Width(),RectWnd.Height(),
		  &dcMemory,0,0, SRCCOPY);
		DrawAbonents(&dc);    

/*		

			MapCoord.x-=Raster->GetOutputRect().left;
			MapCoord.y=MapCoord.y-(pictd.height-Raster->GetOutputRect().bottom);
		
   */     
}
//=========================================================================================================================
void CMapFrame::ShowMiniMap(bool visiable)
{
	FlagMiniMap=visiable;
	if(visiable==true)
		MiniMap->ShowWindow(SW_SHOW);
	else
		MiniMap->ShowWindow(SW_HIDE);
}
void CMapFrame::DrawAbonents(CDC *pDC)
{
	unsigned int i,j;
	int ind;
	CPoint MapCoord;

	if(Mouse.flag==false)
	{
		for(i=0;i<GPS->HardwareList.size();i++)
			for(j=0;j<GPS->HardwareList[i].WindowsList.size();j++)
			{
				ind=GPS->HardwareList[i].WindowsList[j];
			//	if(FrameContainer->GetFrameIndex(ind)==UnicIndex)
				if(ind==UnicIndex)
				{
					//выводим данное устройство на экран
					if(GPS->HardwareList[i].PositionFromBD.size()<=0)
					   continue;
					MapCoord=Linkage.ConvertGeoToMapCord(GPS->HardwareList[i].PositionFromBD[0].Description,
						                                 GPS->HardwareList[i].PositionFromBD[0].Longitude);
					/*MapCoord=BlocksObj->ConvertGeogrToScreen(GPS->HardwareList[i].PositionFromBD[0].Description,
						                                        GPS->HardwareList[i].PositionFromBD[0].Longitude,
						                                        BlocksObj->GetLevel());*/

					CRaster::TPictInfo pictd;
			pictd=Raster->GetPictInfo();
			/*
			geo.y=pictd.height-Wrect.Height()+point.y-Raster->GetOutputRect().top;
			geo.x=point.x+Raster->GetOutputRect().left;
			geo.y=pictd.height-Wrect.Height()+point.y-Raster->GetOutputRect().top;*/
					

					MapCoord.x-=Raster->GetOutputRect().left;
					//MapCoord.y-=Raster->GetOutputRect().top;
					int top=Raster->GetOutputRect().top;

					CRect t=Raster->GetOutputRect();
					MapCoord.y=MapCoord.y-(pictd.height-Raster->GetOutputRect().bottom);
				    if(SelectedAbonent.ID!=GPS->HardwareList[i].ID)
					  DrawPoint(pDC,MapCoord,0);
					else
					  DrawPoint(pDC,MapCoord,1);
					
				}
			}        
	}
}
//=========================================================================================================================
void CMapFrame::DrawHards()
{
 if(Mouse.flag==false)
 {
  CClientDC dc(this);
  dc.BitBlt(0, 0, RectWnd.Width(), RectWnd.Height(),
          &dcMemory, 0, 0, SRCCOPY);
  DrawAbonents(&dc);
 }
}
//=========================================================================================================================
CMapFrame::~CMapFrame(void)
{
	if(pDC!=NULL)
	  pDC->DeleteDC();
	dcMemory.DeleteDC();
	if(Raster!=NULL)
	  Raster->Free();
}
void CMapFrame::Save(CString FName)
{
  CString *str;
  CString name;
  CStdioFile File;
  int count=0;
  if(GetExp(FileName)!="rsmap")name=GetPuth(FileName)+"\\"+GetFileNameWithoutExp(FileName)+".rsmap";
  else
	  name=FileName;
  File.Open(name,CFile::modeCreate|CFile::modeWrite);
  
  File.WriteString("1\n");
  if(RasterType==0)
     File.WriteString("bmp\n");
  else
  if(RasterType==1)
     File.WriteString("jpg\n");
  
  str=Linkage.GetListPointPriv(&count);
  for(i=0;i<count;i++)
  {
	  File.WriteString(str[i]);
	  File.WriteString("\n");
  }
  File.Close();
}
void CMapFrame::ReadInfoFile(CString FileName)
{
 if(GetExp(FileName)=="rsmap")
 {//если открыт проект

	CString type,exp;
		CStdioFile F;
	if(F.Open(FileName,CFile::modeRead)==TRUE)
	{
		F.ReadString(type);
		F.ReadString(exp);
	}
	
	int prob=type.Find(' ');
	if(prob!=-1) type.Delete(prob,type.GetLength()-prob);
	if(exp=="bmp")
	{
      CBMPRaster *BMPRaster = new CBMPRaster;
	  Raster=BMPRaster;
	  RasterType=0;
	}
	 //читаем привязанные координаты
	CString S;
	while(F.ReadString(S)!=NULL)
	{ 
       if(S=="") break;
	   Linkage.AddPointFromStr(S);//добавляем координату в массив
       
	}
	F.Close();
 }   
 else
 if(GetExp(FileName)=="bmp")
 {
        CBMPRaster *BMPRaster = new CBMPRaster;
	    Raster=BMPRaster;
		RasterType=0;
 }
 else
 if(GetExp(FileName)=="jpg")
 {
        RasterType=1;
 }

 
}
int CMapFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	  CRect rect;
	   CRect R;
	  GetWindowRect(rect);
      ReadInfoFile(FileName);                                      //читаем инфу о файле
try
{
	  Raster->Load(FileName,rect);                                 //загрузили(спроектировали) файл
	  Raster->SetOutputRect(CRect(0,0,Raster->GetPictInfo().width/2,Raster->GetPictInfo().height/2));                       //начальное расположение "камеры"
      R=Raster->SetMiniMap(CRect(0,0,348,356));                    //генерирует изображение миникарты
}
catch (CRasterException& e)
{
      CString strErr= e.GetError();
      AfxMessageBox (strErr, MB_ICONWARNING); 
	  return -1;
}
	  pDC=new CClientDC(this);               //контекст вывода    
	  dcMemory.CreateCompatibleDC(pDC);	     /*контекст в памяти (борьба с мерцанием путем использования второго контекста
										   как буферного)*/
	  
      
	  MiniMap=new CMiniMapFrame(Raster,this,&dcMemory);                      //создаем окно миникарты
	  MiniMap->Create(NULL,NULL,WS_CHILD|WS_CAPTION, CRect(0,0,R.Width(),R.Height()),this,NULL);
      MiniMap->ShowWindow(SW_SHOW);
	  MiniMap->UpdateWindow();
	  
      //Создаем объект "привязка"
	  CRect MapRect;
	  CRaster::TPictInfo PictInfo;
	  PictInfo.bit=0;
	  PictInfo.height=0;
	  PictInfo.width=0;
	  PictInfo=Raster->GetPictInfo();
	  MapRect.left=0;
	  MapRect.top=0;
	  MapRect.right=PictInfo.width;
	  MapRect.bottom=PictInfo.height;
	  Linkage.CreateObj(MapRect);   
	  Linkage.GenerateKoefPriv();            //вычисляем коэ-ты системы уравнений "привязки"
      //

	  FrameContainer->AddToContainer(this);  //добавили окно в конейнер	
	return 0;
}
//=========================================================================================================================
BOOL CMapFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;



	return TRUE;
}

void CMapFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CClientDC dc(this);
	CRect RectDraw=Raster->GetOutputRect();
	if(nChar==37)
	{
		RectDraw.left-=10;
		RectDraw.right-=10;

	}
	else
	if(nChar==38)
	{
		RectDraw.bottom+=10;
		RectDraw.top+=10;
	}
	else
	if(nChar==39)
	{
		RectDraw.left+=10;
		RectDraw.right+=10;
	}
	else
	if(nChar==40)
	{
		RectDraw.bottom-=10;
		RectDraw.top-=10;
	}

	Raster->Draw(CRect(RectDraw.left,RectDraw.top,
		                  RectDraw.right,RectDraw.bottom),&dcMemory,CPoint(0,0));
	dc.BitBlt(0,0,RectWnd.Width(),RectWnd.Height(),
		  &dcMemory,0,0, SRCCOPY);
    MiniMap->SetPos(RectDraw);
	CMDIChildWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMapFrame::OnSize(UINT nType, int cx, int cy)
{
	GetClientRect(RectWnd);
   	if(Raster->GetPictInfo().width<RectWnd.Width()||Raster->GetPictInfo().height<RectWnd.Height())
	{
		CRaster::TPictInfo pict=Raster->GetPictInfo();
		RectWnd.bottom=pict.height;
		RectWnd.right=pict.width;
		MoveWindow(RectWnd);
		return;
	} 
	CMDIChildWnd::OnSize(nType, cx, cy);

	    CRect RectDraw=Raster->GetOutputRect();
    	
		RectDraw.right=RectDraw.left+RectWnd.Width();
		RectDraw.bottom=RectDraw.top+RectWnd.Height();
        Raster->SetOutputRect(RectDraw);
		Raster->Resize(RectWnd);
        GetClientRect(Wrect);
	CBitmap dcMemoryBmp;
    dcMemoryBmp.CreateCompatibleBitmap(pDC, RectWnd.Width()*3, RectWnd.Height()*3);
    dcMemory.SelectObject(&dcMemoryBmp);


	MiniMap->SetPos(RectDraw);
	Raster->Draw(CRect(RectDraw.left,RectDraw.top,
		                  RectDraw.right,RectDraw.bottom),&dcMemory,CPoint(0,0));
	
	
}

void CMapFrame::OnPaint()
{
	CPaintDC dc(this); 
	CRect RectDraw=Raster->GetOutputRect();
	/*Raster->Draw(CRect(RectDraw.left,RectDraw.top,
		                  RectDraw.right,RectDraw.bottom),&dc,CPoint(0,0));
*/
	dc.BitBlt(0,0,RectWnd.Width(),RectWnd.Height(),
		  &dcMemory,0,0, SRCCOPY);
	DrawAbonents(&dc);
	///
}

void CMapFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(MainForm->Menu.RaterCur==0)
	{
	CRect RectDraw=Raster->GetOutputRect();
	Mouse.flag=true;
	Mouse.MCord=point;
	dcMemory.Rectangle(0,0,3*RectWnd.Width(),3*RectWnd.Height());
    
	Raster->Draw(CRect(RectDraw.left,RectDraw.top,
		                  RectDraw.right,RectDraw.bottom),&dcMemory,CPoint(RectWnd.Width(),RectWnd.Height()));
	}
	else
	{
		CDlgPrivAdd *DlgPrivAdd=new CDlgPrivAdd(this,point,&Linkage);
		DlgPrivAdd->DoModal();
	}

	CMDIChildWnd::OnLButtonDown(nFlags, point);
}

void CMapFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	 
	if(Mouse.flag==true)
	{
      CClientDC dc(this);
	  Mouse.delta=Mouse.MCord-point;
	  dc.BitBlt(0,0,2*RectWnd.Width(),2*RectWnd.Height(),
		  &dcMemory,RectWnd.Width()+Mouse.delta.x,RectWnd.Height()+Mouse.delta.y, SRCCOPY);
	  DrawAbonents(&dc);
	}
	else
	{
    	CString BarStrBlock,BarStrGeogr;
		double Lsec=0.0,Dsec=0.0;
	    double Dest,Log;
			CPoint geo;
			CRaster::TPictInfo pictd;
			pictd=Raster->GetPictInfo();
			geo.x=point.x+Raster->GetOutputRect().left;
			geo.y=pictd.height-Wrect.Height()+point.y-Raster->GetOutputRect().top;

		BarStrBlock.Format(" Мышь: x: %d y: %d ",geo.x,geo.y);
        MainForm->m_StatusBar.SetPaneText(3,(LPCSTR)BarStrBlock);
		
		if(Linkage.IsBinding()==true)
		{
		  Linkage.ConvertMapToGeoCord(geo,&Dest,&Log);           //получили геогр координаты в формате double
		  BarStrGeogr="Долгота: "+ConvertGeoDoubleToStr(Dest)+" Широта: "+ConvertGeoDoubleToStr(Log);
          MainForm->m_StatusBar.SetPaneText(1,(LPCSTR)BarStrGeogr);

		  BarStrGeogr.Format("Долгота:%f  Широта:%f",Dest,Log);
		  MainForm->m_StatusBar.SetPaneText(2,(LPCSTR)BarStrGeogr);
		}

	}
	CMDIChildWnd::OnMouseMove(nFlags, point);
}

void CMapFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
   	
	
	if(Mouse.flag==true)
	{
		CRect RectDraw=Raster->GetOutputRect();
        CClientDC dc(this);
		Mouse.delta=point-Mouse.MCord;
		RectDraw.left-=Mouse.delta.x;
        RectDraw.right-=Mouse.delta.x;
		RectDraw.top+=Mouse.delta.y;
		RectDraw.bottom+=Mouse.delta.y;
        
		Raster->Draw(CRect(RectDraw.left,RectDraw.top,
		                  RectDraw.right,RectDraw.bottom),&dcMemory,CPoint(0,0));
		dc.BitBlt(0,0,RectWnd.Width(),RectWnd.Height(),
		  &dcMemory,0,0, SRCCOPY);
		Mouse.flag=false;
		DrawAbonents(&dc);
		MiniMap->SetPos(RectDraw);
		
	}
	
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}



void CMapFrame::OnClose()
{
	FrameContainer->DelFromContainer(UnicIndex);

	CFrameContainer::OnClose();
}

void CMapFrame::OnSetFocus(CWnd* pOldWnd)
{
	CFrameContainer::OnSetFocus(pOldWnd);
	
	FrameContainer->SetActive(UnicIndex);
}
