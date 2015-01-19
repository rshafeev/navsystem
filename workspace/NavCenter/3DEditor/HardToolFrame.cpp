// HardToolFrame.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "HardToolFrame.h"
#include "Geography.h"

const int IDC_HARD       =  500; //комбобокс устройств
const int IDC_LIST       =  501; //листбокс окон
const int IDB_PREVIOUS   =  502; //кнопка назад("дата")
const int IDB_NEXT       =  503; //кнопка назад("дата")
const int IDB_FIND       =  504; //кнопка назад("дата")
// CHardToolFrame
CHardToolFrame *HardToolFrame;

IMPLEMENT_DYNAMIC(CHardToolFrame, CFrameWnd)
BEGIN_MESSAGE_MAP(CHardToolFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_CBN_SELENDOK  (IDC_HARD,&OnBoxChange)  
	ON_LBN_SELCHANGE(IDC_LIST, &OnSessionChange)
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDB_FIND,&OnBtnFindClick)
END_MESSAGE_MAP()
void CHardToolFrame::OnBtnFindClick()
{
	//возникает при нажатии на кнопку ButtonFind (поиск абонента на карте)
  int ind;
  if(ComboHardware->GetCurSel()>=0)
  {
	  if((ind=ListBox->GetCurSel())>=0)
	  {
          /*отсылаем окну инфу о абоненте(устройство,которое выбрано в списке ComboHardware), которое обраотает данную инфу и 
		  выведет положение абонента(если возможно конечно)
		  */
		  if(SelectedDrive->PositionFromBD.size()>0)
		  {
		     //указали на устройство, которое нужно найти на карте
			 FrameContainer->GetFramePtr(SelectedDrive->WindowsList[ind])->SetSelectAbonent(*SelectedDrive);
			 //выводим на экран ту часть карты, где находится абонент
		     FrameContainer->GetFramePtr(SelectedDrive->WindowsList[ind])->FindAbonent(&SelectedDrive->PositionFromBD[0]);  
		  }
	  }
  }
}
CHardToolFrame::CHardToolFrame()
{
	ComboHardware  = new CComboBox;
	ListBox        = new CListBox;
	ButtonPrevious = new CButton;
	ButtonNext     = new CButton;
	ButtonFind     = new CButton;
	SelectedDrive=NULL;
	//ComboHardware=NULL;
    //ListBox=NULL;
	ConnectStatus=false;
}

CHardToolFrame::~CHardToolFrame()
{
	delete ComboHardware;
	delete ListBox;
	delete ButtonPrevious;
	delete ButtonNext;
	delete ButtonFind;

	SelectedDrive=NULL;
//	dcMemory.DeleteDC();
}

void CHardToolFrame::OnSessionChange()
{
	int ind=ListBox->GetCurSel();
	if(ind<=-1) return;
	int y=SelectedDrive->WindowsList[ind];
	if(y>=0)
	  FrameContainer->GetFramePtr(y)->SetFocus();
	
}


BOOL CHardToolFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style=WS_OVERLAPPED|WS_EX_TRANSPARENT|WS_THICKFRAME|WS_SYSMENU;
	
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}


//ON_CBN_SELENDOK ON_CBN_DROPDOWN

// CHardToolFrame message handlers
void CHardToolFrame::OpenConnect()
{
   ConnectStatus=true;

}
void CHardToolFrame::CloseConnect()
{
   ConnectStatus=false;
}
void CHardToolFrame::OnBoxChange()
{
   
	DriveIndex=ComboHardware->GetCurSel();
	SelectedDrive=&GPS->HardwareList[DriveIndex];

	UpdateWindowList(DriveIndex);
    
	//  CenterWindow();
}
int CHardToolFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	dcMemory.CreateCompatibleDC(GetDC());
	ComboHardware->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,
		                  CRect(15,25,220,45),this,IDC_HARD);
	
	ListBox->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_WANTKEYBOARDINPUT|LBS_DISABLENOSCROLL|
		            WS_VSCROLL|LBS_NOTIFY, CRect(15,75,220,650), this, IDC_LIST);
	
	
	ButtonPrevious->Create("<<",WS_CHILD|WS_VISIBLE,CRect(15,295,50,312),this,IDB_PREVIOUS);
	ButtonNext->Create(">>",WS_CHILD|WS_VISIBLE,CRect(70,295,105,312),this,IDB_NEXT);
	ButtonFind->Create("Перейти",WS_CHILD|WS_VISIBLE,CRect(150,295,215,320),this,IDB_FIND);
	UpdateInfo();
	ButtonNext->EnableWindow(0);
	ButtonPrevious->EnableWindow(0);
	//MainForm->Menu.HardView=true;
	return 0;
}

void CHardToolFrame::OnPaint()
{
	CPaintDC dc(this); 
	CRect rect;
    CFont f1,f2;
	CBrush b; 
	CString StrBuf;
		
	GetWindowRect(rect);
    f1.CreateFont(-MulDiv(11,dc.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 700, 1, 0,
		                    0, RUSSIAN_CHARSET, 0, 0, 0 , 0,"Times New Roman");
	f2.CreateFont(-MulDiv(11,dc.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 700, 0, 0,
		                    0, RUSSIAN_CHARSET, 0, 0, 0 , 0,"Times New Roman");
	dc.SelectObject(&f1);
	dc.SetBkMode(TRANSPARENT); 
	b.CreateSolidBrush(RGB(92,120,200)); 
	dc.SelectObject(&b);
	dc.Rectangle(0,0,rect.Width()+2,rect.Height()+2);
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255)); 
	dc.SelectObject(&b);
	dc.TextOutA(5,5,"Имя устройства:");
	dc.TextOutA(5,50,"Вывод в окна:");
	dc.TextOutA(5,335,"Получение данных:");

	UpdateVirtualCanvas(&dc);
}
void CHardToolFrame::UpdateInfo()
{
	if(this!=NULL&&ComboHardware!=NULL&&this->m_hWnd!=NULL)
   {
    if(GPS->LoadHardwareToList(ComboHardware)==true)
      OnBoxChange();
   }

}
//************************************************************************************************************
void CHardToolFrame::UpdateWindowList(int index)
{
	
 	if(DriveIndex==index)
	{
		GPS->LoadWindowOfGHardToList(DriveIndex,ListBox);
	   RedrawWindow(CRect(0,355,300,700));
    }
}

//************************************************************************************************************
void CHardToolFrame::AddToDriveList(int index)
{
	
	SelectedDrive=GPS->GetDrive(index);
	CString g=SelectedDrive->Name+"("+SelectedDrive->ID+")";
	ComboHardware->InsertString(index,(LPCSTR)g);
	
}
void CHardToolFrame::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);

	
}

void CHardToolFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

   	GetClientRect(ClientRect);
	
	ComboHardware->MoveWindow(15,25,ClientRect.Width()-25,45,1);
	ListBox->MoveWindow(15,75,ClientRect.Width()-25,220,1);

	CDC *pDC=GetDC();
	CBitmap dcMemoryBmp;
    dcMemoryBmp.CreateCompatibleBitmap(pDC, ClientRect.Width(), ClientRect.Height());
    dcMemory.SelectObject(&dcMemoryBmp);
	UpdateCanvas();
//	wndNoTopMost.m_nFlags=SWP_NOREDRAW;
 /* CRect WRect;
  GetWindowRect(WRect); //получаем прямоугольник окна
  DrawFocusRect(GetDC()->m_hDC,WRect); //АПИ функция, рисующая рамку
*/

/*	  
  GetWindowRect(Handle, &WRect); //получаем прямоугольник окна
  DrawFocusRect(GetDC(0), &WRect); //АПИ функция, рисующая рамку*/
}

void CHardToolFrame::OnClose()
{
	//MainForm->Menu.HardView=false;

	CFrameWnd::OnClose();
}

void CHardToolFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();
	
	MainForm->Menu.HardView=false;
}
void CHardToolFrame::UpdateCanvas()
{
   CClientDC dc(this);
   UpdateVirtualCanvas(&dc);
}
void CHardToolFrame::UpdateVirtualCanvas(CDC *pDC)
{
	
	CRect rect;
    CFont f1,f2;
	CBrush b; 
	int g,m;
	double s;
	CString StrBuf;
		
		                  

if(SelectedDrive!=NULL)
{
	  GetWindowRect(rect);
      f1.CreateFont(-MulDiv(11,dcMemory.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 700, 1, 0,
		                    0, RUSSIAN_CHARSET, 0, 0, 0 , 0,"Times New Roman");
	  f2.CreateFont(-MulDiv(11,dcMemory.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 700, 0, 0,
		                     0, RUSSIAN_CHARSET, 0, 0, 0 , 0,"Times New Roman");
	dcMemory.SetBkMode(TRANSPARENT); 
	b.CreateSolidBrush(RGB(92,120,200)); 
	dcMemory.SelectObject(&b);
	dcMemory.Rectangle(0,0,rect.Width()+2,rect.Height()+2);

	if(SelectedDrive->runtime==true)
	{

		  dcMemory.TextOutA(20,370,"В реальном времени.");
		  //выводим значение долготы
		  if(SelectedDrive->PositionFromBD.size()>0)
		  {
			   StrBuf=ConvertGeoDoubleToStr(SelectedDrive->PositionFromBD[0].Longitude);
		/*	  CString secS;
			  secS.Format("%11.4f",(int(s*1000.0))/1000.0);
			  secS.TrimLeft();

			  ConvertDoubleToGMS(SelectedDrive->PositionFromBD[0].Longitude,&g,&m,&s);
			  StrBuf.Format("Долгота : %d:%d:%f",g,m,(int(s*1000.0))/1000.0);*/
		  }
		  else
			  StrBuf="Долгота :  - ";
	  	 dcMemory.TextOutA(25,419,StrBuf);
	     //выводим значение широты
		  if(SelectedDrive->PositionFromBD.size()>0)
		  {
			  StrBuf=ConvertGeoDoubleToStr(SelectedDrive->PositionFromBD[0].Description);
		/*	  CString secS;
			  secS.Format("%11.4f",(int(s*1000.0))/1000.0);
			  secS.TrimLeft();
			  ConvertDoubleToGMS(SelectedDrive->PositionFromBD[0].Description,&g,&m,&s);
			  StrBuf.Format("Широта : %d:%d:%s",g,m,secS);
			 */
		  }
		  else
			  StrBuf="Широта :  - ";
		  dcMemory.TextOutA(25,435,StrBuf);
		  //выводим значение высоты
		  if(SelectedDrive->PositionFromBD.size()>0)
		  {
			 // ConvertDoubleToGMS(SelectedDrive->PositionFromBD[0].Height,&g,&m,&s);
		      StrBuf.Format("Высота  : %f",SelectedDrive->PositionFromBD[0].Height);
		  }
		  else
			  StrBuf="Высота  :  - ";
	  	 dcMemory.TextOutA(25,450,StrBuf);
	     // выводим время получения информации от сервера
		 //dc.TextOutA(25,450,"Высота  :");
		// dcMemory.SelectObject(&f2);
		 if(SelectedDrive->PositionFromBD.size()>0)
		 {  
			 StrBuf.Format("Время  : %s",SelectedDrive->PositionFromBD[0].DateTime);
		 }
		 else
		   StrBuf.Format("Время  : -");

			 dcMemory.TextOutA(25,470,StrBuf);
	}
	else
	{
		  dcMemory.TextOutA(10,370,"Получены с БД.");
		  
	}
	pDC->BitBlt(0, 390, 300,200,
			        &dcMemory, 0,370, SRCCOPY);
}
	

}