#include "StdAfx.h"
#include "WorldMapFrame.h"
#include "Memory.h"
#include "3DEditor.h"
#include "DlgSaveName.h"
#include "afxmt.h "


//ID
const int IDP_POLZ=2000;
//50 - ползунок


IMPLEMENT_DYNAMIC(CWorldMapFrame, CMDIChildWnd)
BEGIN_MESSAGE_MAP(CWorldMapFrame, CMDIChildWnd)
		ON_WM_CREATE()
		ON_WM_MOUSEWHEEL()
		ON_WM_LBUTTONDOWN()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONUP()
		ON_WM_ACTIVATE()
		ON_WM_SIZE()
		ON_WM_CHAR()
		ON_WM_KEYDOWN()
		ON_WM_PAINT()
		ON_WM_CLOSE()
		ON_WM_SETFOCUS()
		ON_WM_HSCROLL(IDP_POLZ,OnHScroll)	
END_MESSAGE_MAP()
//==========================================================================================================================
int CWorldMapFrame::LoadFileFromInet(CString FNameWithoutExp)
{

CString m_mes; // переменная в которой будут хранится сообщения
char temp[100]; // промежуточная переменная для перевода 
// данных из Int в char
CString m_path=FolderCache+FNameWithoutExp+".jpg"; // имя файла для записи
char strBody[1024]; // буфер из 1024 байт 
CString m_url=ServerUrl+FNameWithoutExp;
// создаём переменную session и открываем сессию ANDY
CInternetSession session( _T( "ANDY" ), PRE_CONFIG_INTERNET_ACCESS );

// создаём переменную pServer класса CHttpConnection
CHttpConnection* pServer = NULL;

// создаём переменную pFile класса CHttpFile
CHttpFile* pFile = NULL;

try
{
CString strServerName; // имя сервера
CString strObject; // имя объекта 
INTERNET_PORT nPort; // номер порта для связи
DWORD dwServiceType; // тип сервиса

// функция AfxParseURL получает данные с указанного URL ( у нас m_url ) об сервере,
// объекте, типе сервиса и порте 

if ( AfxParseURL( m_url, dwServiceType, strServerName, strObject, nPort ) == 0 )
{
return -1; // выход из функции OnButtonConnect() 
}
// вывод данных о сервере
/*
m_mes = "";
m_mes += "Server Name = "; 
m_mes += (CString)strServerName; m_mes += "rn";
m_mes += "Object Name = ";
m_mes += (CString)strObject; m_mes += "rn";
m_mes += "Port = ";
itoa( nPort, temp, 10 );
m_mes += (CString)&temp[0]; m_mes += "rn";
*/

// Устанавливаем подключение по HTTP протоклолу. 
pServer = session.GetHttpConnection( strServerName, nPort );

// посылаем запрос об объекте ( strObject )
pFile = pServer->OpenRequest(
CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL,
INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT
);

// Добавляем заголовок к HTTP запросу 
pFile->AddRequestHeaders( _T( "Accept: */*rnUser-Agent: ANDYrn" ) );

// посылаем запрос
pFile->SendRequest( );

DWORD dwRet; // переменная для хранения кода состояния 
pFile->QueryInfoStatusCode( dwRet ); // записываем код состояния в dwRet

// вывод данных
m_mes += "The HTTP GET returned a status code of ";
itoa( dwRet, temp, 10 ); 
m_mes += (CString)&temp[0]; m_mes += "rn"; 

CString strHeader; // переменная для хранения полученного заголовока запроса
pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strHeader); // записываем заголовок в strHeader

// вывод данных
m_mes += "Header = ";
m_mes += strHeader; 
//UpdateData( FALSE );

// если код состояния не равен 200, то выходим из функции
if( dwRet != 200 ) { m_mes += "Program terminate!";return -1; }
// ----------------------------------------------------------

// проверка выбора файла для записи 
/*
m_mes += "Starting download the file."; m_mes += "rn";

if( m_path == "" ) 
{ 
m_mes += "Error! No file to save. Choese the file.";
m_mes += "rn"; UpdateData( FALSE ); return 1; }
else 
{
m_mes += "File name to save : ";
m_mes += m_path; m_mes += "rn"; UpdateData( FALSE ); 
}
*/
CFile file2; // объявляем переменную file2 класса CFile

// открываем файл для записи в двоичном формате ( CFile::typeBinary ) !!!
file2.Open((LPCTSTR)m_path,
CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

int allRead = 0; // переменная для хранения общего числи считанных байт
int nRead = pFile->Read( strBody, 1024 ); // считываем первые 1024 байта в буфер.
// переменная nRead хранит количество
// считанных байт 

allRead += nRead; // обновляем общее число считанных байт

// вывод данных
m_mes += "Loading ";
itoa( nRead, temp, 10 );
m_mes += (CString)&temp[0]; m_mes += " bytes"; m_mes += "rn";
//UpdateData( FALSE );

// записываем буфер из nRead байт в файл
file2.Write( strBody, nRead ); 

// цикл считывания, пока nRead не будет равняться нулю
while ( nRead > 0 )
{
nRead = pFile->Read( strBody, 1024 );

if( nRead != 0 ) 
{
m_mes += "Loading ";
itoa( nRead, temp, 10 );
m_mes += (CString)&temp[0]; m_mes += " bytes"; m_mes += "rn";

file2.Write( strBody, nRead );
allRead += nRead;

}

}

// вывод данных
m_mes += "rn";
m_mes += "Total bytes = ";
itoa( allRead, temp, 10 );
m_mes += &temp[0]; m_mes += "rn";
//UpdateData( FALSE );

file2.Close(); // закрываем файл

pFile->Close(); // закрываем Internet файл
pServer->Close(); // закрываем сервер

m_mes += "Download is complete !!!"; m_mes += "rn";
//UpdateData( FALSE );
}

catch ( CInternetException* pEx )
{
// Если произошла ошибка в WinInet

// вывод ошибки
char szErr[1024];
pEx->GetErrorMessage( szErr, 1024 );

m_mes += "Error: ( ";
itoa( int(pEx->m_dwError), temp ,10 );
m_mes += (CString)&temp[0];
m_mes += " ) ";
m_mes += (CString)&szErr[0]; m_mes += "rn";
//UpdateData( FALSE );

pEx->Delete( ); // удаление переменной класса CInternetException 
DeleteFile(m_path);//!!!!!!!!!!!!!!
if ( pFile != NULL )
delete pFile; // закрываем Internet файл
if ( pServer != NULL )
delete pServer; // закрываем сервер
session.Close( ); // закрываем сессию
return -1;
}

if ( pFile != NULL )
delete pFile; // закрываем Internet файл
if ( pServer != NULL )
delete pServer; // закрываем сервер
session.Close( ); // закрываем сессию

return 0;
}
//==========================================================================================================================
CWorldMapFrame::CWorldMapFrame(void)
{
	Mutex=new CMutex;
	Coord.x=0;
	Coord.y=0;
	pDC=NULL;
	FileName="";
	WindowType=1;//тип окна - "мировая карта"
	NoLoadCount=0;
}

CWorldMapFrame::CWorldMapFrame(CString PathName)
{
	Mutex=new CMutex;
	TerminatedThread=false;
	this->PathName=PathName;
	Coord.x=0;
	Coord.y=0;
	NoLoadCount=0;
	WindowType=1;//тип окна - "мировая карта"

		ServerUrl="http://kh.google.com/kh?v=3&t=";
	FolderCache=RES_PATH+"cache\\";
	LoadContainer.resize(5000);
}

CWorldMapFrame::~CWorldMapFrame(void)
{
			TerminatedThread=true;
	WaitForSingleObject(*InetRaster,3000);
	if(pDC!=NULL)
	 pDC->DeleteDC();

}
//=======================================================================================================================
void CWorldMapFrame::ShowMiniMap(bool visiable)
{
	FlagMiniMap=visiable;
	if(visiable==true)
		MiniMap->ShowWindow(SW_SHOW);
	else
		MiniMap->ShowWindow(SW_HIDE);
}
//=======================================================================================================================
void CWorldMapFrame::SendToThreadNotLoadPict(vector<TLoadedFiles> &NotLoaded)
{
  unsigned int i,j;
  bool flag;
  CMutex Mutex;
  Mutex.Lock(2000);
  for(i=0;i<NotLoaded.size();i++)
  {
	  flag=true;
	  for(j=0;j<NoLoadCount;j++)
	  {
		  if(LoadContainer.size()<=0)
			  return;
		  if(NotLoaded[i].FName==LoadContainer[j].FName)
		  {
            flag=false;
			break;
		  }
	  }
	  if(flag==true)
	  {
		  if(NoLoadCount<5000-1)
		    NoLoadCount++;
		  else
			NoLoadCount=0;
		  TLoadedFiles Add;
		  		  if(LoadContainer.size()<=0)
			  return;
		 if(LoadContainer.size()>0)
		 {
		    Add.FName=NotLoaded[i].FName;
		    Add.level=BlocksObj->GetLevel();
		    Add.LeftTopBlock=BlocksObj->GetLeftTopBlock();
		    Add.IndexBlock=NotLoaded[i].IndexBlock;
		    LoadContainer[NoLoadCount-1]=Add;
		 }
	  }
  }
  Mutex.Unlock();
}
//================================================================================================================================
void CWorldMapFrame::DrawAbonents(CDC *pDC)
{
		unsigned int i,j;
		int ind;
		CPoint MapCoord;
		
	CriticalSection.Lock(2000);
	if(Mouse.flag==false&&redrawHards==true)
	{
		for(i=0;i<GPS->HardwareList.size();i++)
			for(j=0;j<GPS->HardwareList[i].WindowsList.size();j++)
			{
				ind=GPS->HardwareList[i].WindowsList[j];
				if(ind==UnicIndex)
				{
					//выводим данное устройство на экран
					if(GPS->HardwareList[i].PositionFromBD.size()<=0)
					   continue;
					MapCoord=BlocksObj->ConvertGeogrToScreen(GPS->HardwareList[i].PositionFromBD[0].Description,
						                                        GPS->HardwareList[i].PositionFromBD[0].Longitude,
						                                        BlocksObj->GetLevel());
					if(Mouse.flag==false)
					{
					  if(SelectedAbonent.ID!=GPS->HardwareList[i].ID)
					    DrawPoint(pDC,MapCoord,0);
					  else
					    DrawPoint(pDC,MapCoord,1);
					}
				}
			}        
	}
	CriticalSection.Unlock();
}
//================================================================================================================================
void CWorldMapFrame::Save(CString FName)
{

	
	CDlgSaveName *DlgSaveName=new CDlgSaveName(0,&FName);
 if(DlgSaveName->DoModal()!=-1)
 {
    int ind;
	CString g;
	if(FName!="")
	{
		
		ind=FName.Find(".rsmap");
		if(ind!=-1&&(ind==FName.GetLength()-7))
	   	   g=PathName+"\\"+FName;
		else
		   g=PathName+"\\"+FName+".rsmap";

	}
	else
	if(this->FileName!="")
       g=PathName+"\\"+this->FileName;
	else
	   g=PathName+"\\project1.rsmap";

	CString buf;
	CStdioFile File;
	File.Open(g,CFile::modeCreate|CFile::modeWrite,0);
	  buf="0 \n";
	  File.WriteString(buf);
	  File.Close();
	
 }
}
//================================================================================================================================
void CWorldMapFrame::FindAbonent(TGPSPoint *Position)
{
	CPoint CentralBlock;
	CentralBlock=BlocksObj->ConvertGeogrToBlock(Position->Description,Position->Longitude,BlocksObj->GetLevel());
    
  	BlocksObj->SetBlock(CentralBlock);
  redrawHards=false;
  BlocksObj->SetSdvig(CPoint(0,0));
  BlocksObj->PushMatrix();
  BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),BlocksObj->GetSdvig());
 // MiniMap->SetLevel(BlocksObj->GetLevel(), CPoint(BlocksObj->GetBlock()));
  pDC->BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			  &dcMemory, 0, 0, SRCCOPY);
  DrawAbonents(pDC);
  redrawHards=true;

}
//================================================================================================================================
void CWorldMapFrame::DrawHards()
{
	if(Mouse.flag==false)
	{
		CClientDC dc(this);
				dc.BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			        &dcMemory, 0, 0, SRCCOPY);
		DrawAbonents(&dc);
	}	
}
//================================================================================================================================
int CWorldMapFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetTimer(1, 100, NULL);
	
	if(PathName.Find("rsmap"))
	{
		FileName=GetFileName(PathName);
		PathName=GetPuth(PathName);
	}
	if(FileName=="")
		SetWindowText(PathName);
	else
		SetWindowText(FileName);

	  MiniMap=new CMiniWorldMapFrame(PathName+"\\");

	MiniMap->Create(NULL,NULL,WS_CHILD|WS_CAPTION, CRect(0,0,256,256),this,NULL);
	  
	pDC=new CClientDC(this);
	dcMemory.CreateCompatibleDC(pDC);
    
	BlocksObj=new CBlockRaster(PathName+"\\",RES_PATH);
    FrameContainer->AddToContainer(this);

	Slider=new CSliderCtrl();
	Slider->Create(WS_CHILD|WS_VISIBLE,CRect(10,10,194,34),this,IDP_POLZ);
	Slider->LockWindowUpdate();
	Slider->SetRangeMin(1,0);
	Slider->SetRangeMax(20,0);

	InetRaster= new СInetRaster();
	InetRaster->Create(BlocksObj,&TerminatedThread,&LoadContainer,&NoLoadCount,this,UnicIndex,Mutex);
	InetRaster->CreateThread();
	
	return 0;
}
//================================================================================================================================
void CWorldMapFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	int x;
	x=33;
}
//================================================================================================================================

BOOL CWorldMapFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
 // InetRaster->SuspendThread();
//	Mutex->Lock(2000);
	if(BlocksObj->GetDrawRect().left>Coord.x||BlocksObj->GetDrawRect().right<Coord.x||
		BlocksObj->GetDrawRect().top>Coord.y||BlocksObj->GetDrawRect().bottom<Coord.y)
       
	{
	//	Mutex->Unlock();
	   return CMDIChildWnd::OnMouseWheel(nFlags, zDelta, pt);
	}
  if(zDelta>0&&BlocksObj->GetLevel()<20)
  {
  	BlocksObj->SetBlock(CPoint((BlocksObj->GetLeftTopBlock().x+(Coord.x-BlocksObj->GetDrawRect().left)/PICTSIZE)*2,
				               (BlocksObj->GetLeftTopBlock().y+(Coord.y-BlocksObj->GetDrawRect().top)/PICTSIZE)*2));
	BlocksObj->SetLevel(Slider->GetPos()+1);
	Slider->SetPos(BlocksObj->GetLevel());
  }
  else
  if(zDelta<0&&BlocksObj->GetLevel()>1)
  {
	BlocksObj->SetBlock(CPoint((BlocksObj->GetLeftTopBlock().x+(Coord.x-BlocksObj->GetDrawRect().left)/PICTSIZE)/2,
				               (BlocksObj->GetLeftTopBlock().y+(Coord.y-BlocksObj->GetDrawRect().top)/PICTSIZE)/2));
	BlocksObj->SetLevel(Slider->GetPos()-1);
	Slider->SetPos(BlocksObj->GetLevel());
  }
  else
  {//Mutex->Unlock();
     return CMDIChildWnd::OnMouseWheel(nFlags, zDelta, pt);
  }
  //	block=ConvertGeogrToBlock(49.7,36.6,LEVEL);
  
  redrawHards=false;
  BlocksObj->SetSdvig(CPoint(0,0));



 //загружаем картинки 
/////////////////////////**********************!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111 

  //CriticalSection.Lock();

  vector<TLoadedFiles> NotLoaded=BlocksObj->PushMatrix();
 // InetRaster->SuspendThread();
  NoLoadCount=0;
  //SendToThreadNotLoadPict(NotLoaded);
  //InetRaster->ResumeThread();
 /////////////////////////**********************!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111 



  BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),BlocksObj->GetSdvig());
 // MiniMap->SetLevel(BlocksObj->GetLevel(), CPoint(BlocksObj->GetBlock()));


  pDC->BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			  &dcMemory, 0, 0, SRCCOPY);
    DrawAbonents(pDC);
	
	//Mutex->Unlock();
 // InetRaster->ResumeThread();  
  
  redrawHards=true;
  return CMDIChildWnd::OnMouseWheel(nFlags, zDelta, pt); 
}
//================================================================================================================================
void CWorldMapFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	Mouse.MCord=point;
	Mouse.flag=true;
	//InetRaster->SuspendThread();
	FlagDraw=true;
	CMDIChildWnd::OnLButtonDown(nFlags, point);
	SetFocus();
}
//================================================================================================================================
void CWorldMapFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	Coord=point;
    CPoint Block;
	CString BarStrBlock,BarStrGeogr;
	//1
  BarStrBlock.Format(" Блок: x: %d y: %d ",
		         BlocksObj->GetLeftTopBlock().x+(point.x-BlocksObj->GetDrawRect().left)/PICTSIZE,
		         BlocksObj->GetLeftTopBlock().y+(point.y-BlocksObj->GetDrawRect().top)/PICTSIZE);
  MainForm->m_StatusBar.SetPaneText(0,(LPCSTR)BarStrBlock);
    //2
  double Lon,Lat;
  CPoint BlockCord;
  int LonGMS[2],LatGMS[2];
  double LonSec,LatSec;
 
  BlockCord.x=point.x-BlocksObj->GetDrawRect().left-(point.x-BlocksObj->GetDrawRect().left)/PICTSIZE*PICTSIZE;
  BlockCord.y=point.y-BlocksObj->GetDrawRect().top-(point.y-BlocksObj->GetDrawRect().top)/PICTSIZE*PICTSIZE;
  Block.x=BlocksObj->GetLeftTopBlock().x+(point.x-BlocksObj->GetDrawRect().left)/PICTSIZE;
  Block.y= BlocksObj->GetLeftTopBlock().y+(point.y-BlocksObj->GetDrawRect().top)/PICTSIZE;


  BlocksObj->ConvertCoordToGeogr(Block,BlockCord,BlocksObj->GetLevel(),&Lat,&Lon);
 // ConvertDoubleToGMS(Lon,&LonGMS[0],&LonGMS[1],&LonSec);
//  ConvertDoubleToGMS(Lat,&LatGMS[0],&LatGMS[1],&LatSec);
  BarStrGeogr=" Долгота: "+ConvertGeoDoubleToStr(Lat)+" Широта: "+ConvertGeoDoubleToStr(Lon);
  /*BarStrGeogr.Format(" Широта: %d.%d.%f   Долгота: %d.%d.%f ",LatGMS[0],abs(LatGMS[1]),fabs(LonSec),
	                                                          LonGMS[0],abs(LonGMS[1]),fabs(LatSec));*/
   MainForm->m_StatusBar.SetPaneText(1,(LPCSTR)BarStrGeogr);
  //3
  BarStrGeogr.Format(" %d %d ; %d %d ",BlocksObj->GetDrawRect().left,
	                                                          BlocksObj->GetDrawRect().top,
															  BlocksObj->GetDrawRect().right,
															  BlocksObj->GetDrawRect().bottom);  
   MainForm->m_StatusBar.SetPaneText(2,(LPCSTR)BarStrGeogr);
  BarStrGeogr.Format("Мышь: %d %d",point.x,point.y);
   MainForm->m_StatusBar.SetPaneText(3,(LPCSTR)BarStrGeogr);
  if(Mouse.flag==true)
	{
	//	InetRaster->SuspendThread();

		BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),point-Mouse.MCord+BlocksObj->GetSdvig());
		pDC->BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			        &dcMemory, 0, 0, SRCCOPY);
	//	InetRaster->ResumeThread();
	}
	CMDIChildWnd::OnMouseMove(nFlags, point);
}

//================================================================================================================================
void CWorldMapFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	if(Mouse.flag==true)
	{
		BlocksObj->SetSdvig(BlocksObj->GetSdvig()+point-Mouse.MCord);
		if(abs(BlocksObj->GetSdvig().x/PICTSIZE)>0||abs(BlocksObj->GetSdvig().y/PICTSIZE)>0)
		{
			
			BlocksObj->SetBlock(CPoint(BlocksObj->GetBlock().x-BlocksObj->GetSdvig().x/PICTSIZE,
				                       BlocksObj->GetBlock().y-BlocksObj->GetSdvig().y/PICTSIZE));
			vector<TLoadedFiles> NotLoaded=BlocksObj->PushMatrixWithCopy(CPoint(-BlocksObj->GetSdvig().x/PICTSIZE,
	        			                                     -BlocksObj->GetSdvig().y/PICTSIZE));
		//	InetRaster->SuspendThread();            
			NoLoadCount=0;
			SendToThreadNotLoadPict(NotLoaded);
		//	InetRaster->ResumeThread();
			BlocksObj->SetSdvig(CPoint(BlocksObj->GetSdvig().x-BlocksObj->GetSdvig().x/PICTSIZE*PICTSIZE,
				                       BlocksObj->GetSdvig().y-BlocksObj->GetSdvig().y/PICTSIZE*PICTSIZE)); 

	   }
		BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),/*point-Mouse.MCord+*/BlocksObj->GetSdvig());
		pDC->BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			        &dcMemory, 0, 0, SRCCOPY);

		Mouse.flag=false;
		MiniMap->SetMap(BlocksObj);
		DrawAbonents(pDC);
	}
//	Mutex->Unlock();
	CMDIChildWnd::OnLButtonUp(nFlags, point);
	MiniMap->UpdateData();
    FlagDraw=false;
}
//================================================================================================================================
void CWorldMapFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CMDIChildWnd::OnActivate(nState, pWndOther, bMinimized);

}
//================================================================================================================================
void CWorldMapFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	CRect WndRect;
	GetClientRect(WndRect);
	CBitmap dcMemoryBmp;
	dcMemory.DeleteDC();
	dcMemory.CreateCompatibleDC(GetDC());
    dcMemoryBmp.CreateCompatibleBitmap(pDC, WndRect.Width(), WndRect.Height());
    
	dcMemory.SelectObject(&dcMemoryBmp);
	MiniMap->MoveWindow(0,WndRect.Height()-256,256,256,1);	

	BlocksObj->SetWindRect(WndRect);    
	BlocksObj->ResizeMemory(CPoint(BlocksObj->GetWindRect().Width()/PICTSIZE*2,
		                           BlocksObj->GetWindRect().Height()/PICTSIZE*2));
	BlocksObj->PushMatrix();
	BlocksObj->SetSdvig(CPoint(0,0));
}
//================================================================================================================================
void CWorldMapFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	CMDIChildWnd::OnChar(nChar, nRepCnt, nFlags);
}

void CWorldMapFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(nChar==37)
		BlocksObj->SetSdvig(CPoint(BlocksObj->GetSdvig().x+5,BlocksObj->GetSdvig().y));
	else
	if(nChar==38)
		BlocksObj->SetSdvig(CPoint(BlocksObj->GetSdvig().x,BlocksObj->GetSdvig().y+5));
	else
	if(nChar==39)
		BlocksObj->SetSdvig(CPoint(BlocksObj->GetSdvig().x-5,BlocksObj->GetSdvig().y));
	else
	if(nChar==40)
		BlocksObj->SetSdvig(CPoint(BlocksObj->GetSdvig().x,BlocksObj->GetSdvig().y-5));
	else
	{
      CMDIChildWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	  return;
	}
	if((abs(BlocksObj->GetSdvig().x/PICTSIZE)>0||abs(BlocksObj->GetSdvig().y/PICTSIZE)>0))
	{
		BlocksObj->SetBlock(CPoint(BlocksObj->GetBlock().x-BlocksObj->GetSdvig().x/PICTSIZE,
				                       BlocksObj->GetBlock().y-BlocksObj->GetSdvig().y/PICTSIZE));
		BlocksObj->PushMatrixWithCopy(CPoint(-BlocksObj->GetSdvig().x/PICTSIZE,
				                             -BlocksObj->GetSdvig().y/PICTSIZE));
		BlocksObj->SetSdvig(CPoint(BlocksObj->GetSdvig().x-BlocksObj->GetSdvig().x/PICTSIZE*PICTSIZE,
				                       BlocksObj->GetSdvig().y-BlocksObj->GetSdvig().y/PICTSIZE*PICTSIZE)); 
	}
	BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),BlocksObj->GetSdvig());
	pDC->BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			        &dcMemory, 0, 0, SRCCOPY);
	CMDIChildWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
//***************************************************************************************************

void CWorldMapFrame::OnPaint()
{
	
	CPaintDC dc(this);
//	InetRaster->SuspendThread();
	BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),BlocksObj->GetSdvig());
	dc.BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			        &dcMemory, 0, 0, SRCCOPY);
	DrawAbonents(&dc);
//	InetRaster->ResumeThread();
}
//================================================================================================================================
void CWorldMapFrame::OnClose()
{
	FrameContainer->DelFromContainer(UnicIndex);
	TerminatedThread=true;

	//delete InetRaster;

	CFrameContainer::OnClose();
}
//================================================================================================================================
void CWorldMapFrame::OnSetFocus(CWnd* pOldWnd)
{
	CFrameContainer::OnSetFocus(pOldWnd);
	if(MainForm->Menu.DocumentView==true)
		ViewDocFrame->SetListBoxPos(UnicIndex);
	FrameContainer->SetActive(UnicIndex);
}
