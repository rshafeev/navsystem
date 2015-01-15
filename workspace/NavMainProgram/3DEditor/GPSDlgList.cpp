// GPSDlgList.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "GPSDlgList.h"
#include "CompLib.h"

const int IDC_CHECKWINDLIST=1001; 
// CGPSDlgList dialog
IMPLEMENT_DYNAMIC(CGPSDlgList, CDialog)
BEGIN_MESSAGE_MAP(CGPSDlgList, CDialog)
	ON_WM_CREATE()
	ON_LBN_DBLCLK(IDC_LIST1, &CGPSDlgList::OnLbnDblclkList1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CGPSDlgList::OnLbnSelchangeList1)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON5, &CGPSDlgList::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CGPSDlgList::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON1, &CGPSDlgList::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_COMMAND1, &CGPSDlgList::OnBnClickedCommand1)
	ON_LBN_SELCHANGE(IDC_CHECKWINDLIST, &CGPSDlgList::OnCheckListChange)
	ON_BN_CLICKED(IDC_BUTTON3, &CGPSDlgList::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CGPSDlgList::OnBnClickedButton6)
END_MESSAGE_MAP()

	
CGPSDlgList::CGPSDlgList(CWnd* pParent /*=NULL*/)
	: CDialog(CGPSDlgList::IDD, pParent)
{
	m_WindowList=new CCheckListBox;
}

CGPSDlgList::~CGPSDlgList()
{
}
void CGPSDlgList::OnCheckListChange()
{
    int i;
	 int HardInd=m_HardList.GetCurSel();
	
	if(HardInd>=(int)GPS->HardwareList.size())
	{
       int f;
	   f=33;
	}
	if(HardInd>=0)
	{
	  GPS->HardwareList[HardInd].WindowsList.clear();
	  for(i=0;i<m_WindowList->GetCount();i++)
	  {
		  if(m_WindowList->GetCheck(i)==1)
		  {
			 int Wind=FrameContainer->GetFrameIndex(i);
			 GPS->HardwareList[HardInd].WindowsList.push_back(Wind);
		  }
	  }
	}
	HardToolFrame->UpdateWindowList(HardInd);

}
void CGPSDlgList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST1, m_HardList);
	GPS->LoadHardwareToList(&m_HardList);

	DDX_Control(pDX, IDC_RADIO1, m_realTime);
	DDX_Control(pDX, IDC_RADIO2, m_FromBD);
	if(m_realTime.GetCheck()==0&&m_FromBD.GetCheck()==0)
	{
		m_realTime.SetCheck(1);
		m_FromBD.SetCheck(0);
	}

	DDX_Control(pDX, IDC_EDIT1, m_ID);
	DDX_Control(pDX, IDC_EDIT2, m_Name);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_date2);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_time1);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_time2);
	DDX_Control(pDX, IDC_BUTTON6, m_SelectAllBtn);
	DDX_Control(pDX, IDC_BUTTON7, m_NotSelectAllBtn);
	if(m_WindowList->IsWindowEnabled()==TRUE&&m_WindowList->GetCount()>0)
	{
		m_SelectAllBtn.EnableWindow(TRUE);
		m_NotSelectAllBtn.EnableWindow(TRUE);
	}
	else
	{
		m_SelectAllBtn.EnableWindow(FALSE);
		m_NotSelectAllBtn.EnableWindow(FALSE);

	}

}





// CGPSDlgList message handlers

int CGPSDlgList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
    int size;
//создаем объекты и контролы
	m_WindowList->Create(WS_CHILD|WS_VISIBLE|LBS_HASSTRINGS|LBS_NOTIFY, 
		                 CRect(235,28,440,360), this, IDC_CHECKWINDLIST);
 //загружаем список окон к устройству номер 0
	size=GPS->LoadWindowOfGHardToChList(0,m_WindowList);
	
//зададим горизонтальную прокрутку	
	
	CSize      sz;
	CDC*       pDC = m_WindowList->GetDC();
	sz=pDC->GetTextExtent("w");
	size*=sz.cx;
	m_WindowList->SetHorizontalExtent(size);
	m_WindowList->EnableWindow(FALSE);
	return 0;
}

void CGPSDlgList::OnLbnDblclkList1()
{

}

void CGPSDlgList::OnLbnSelchangeList1()
{
	int Cur=m_HardList.GetCurSel();
	if(Cur>=0)
	{
	  int size;
	  CSize      sz;
	  CDC*       pDC = m_WindowList->GetDC();
	
	  size=GPS->LoadWindowOfGHardToChList(Cur,m_WindowList);
	  sz=pDC->GetTextExtent("w");
	  size*=sz.cx;
	  m_WindowList->SetHorizontalExtent(size);
	  m_WindowList->EnableWindow(TRUE);
	  RedrawWindow(CRect(10,10,220,345));

	if(m_WindowList->IsWindowEnabled()==TRUE&&m_WindowList->GetCount()>0)
	{
		m_SelectAllBtn.EnableWindow(TRUE);
		m_NotSelectAllBtn.EnableWindow(TRUE);
	}
	else
	{
		m_SelectAllBtn.EnableWindow(FALSE);
		m_NotSelectAllBtn.EnableWindow(FALSE);

	}
	}

}

void CGPSDlgList::OnPaint()
{
	CPaintDC dc(this);

	CFont f1,f2;
	CString StrBuf;
	
	f1.CreateFont(-MulDiv(8,dc.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 700, 1, 0,
		                    0, RUSSIAN_CHARSET, 0, 0, 0 , 0,"Times New Roman");
	dc.SelectObject(&f1);
	dc.SetBkMode(TRANSPARENT); 

	int Cur=m_HardList.GetCurSel();
	if(Cur>=0)
	{
		if(GPS->HardwareList[Cur].runtime==true)
		dc.TextOutA(90,302,"В реальном времени");
	   else
	   {
         dc.TextOutA(90,302,"С БД сервера");
		 StrBuf.Format("С  : %s ",GPS->HardwareList[Cur].DateTime1);
		 dc.TextOutA(90,322,StrBuf);
		 StrBuf.Format("До : %s ",GPS->HardwareList[Cur].DateTime2);
	     dc.TextOutA(90,332,StrBuf);
	   }
	}
}

void CGPSDlgList::OnBnClickedSave()
{
	CFileDialog File(false,"hwd",NULL,OFN_OVERWRITEPROMPT,"Список устройства|*.hwd||");
	File.DoModal();
	GPS->SaveToFile(File.GetPathName());
}

void CGPSDlgList::OnBnClickedLoad()
{
	GPS->SendAllDelHardware();
	CFileDialog File(true,"hwd",NULL,OFN_OVERWRITEPROMPT,"Список устройства|*.hwd||");
	File.DoModal();
	GPS->LoadFromFile(File.GetPathName(),&m_HardList,true);
	HardToolFrame->UpdateInfo();
}

void CGPSDlgList::OnBnClickedAdd()
{
  	UpdateData();
	THardware hardware;
	CString buf,st;
	CTime time;

	buf=GetStrFromEdit(&m_ID);
    if(buf=="")
	{
		AfxMessageBox("Заполните все поля",MB_USERICON|MB_OK|MB_SERVICE_NOTIFICATION_NT3X);
	    return;
	}
	hardware.ID=buf;

	buf=GetStrFromEdit(&m_Name);
    if(buf=="")
	{
		AfxMessageBox("Заполните все поля",MB_USERICON|MB_OK|MB_SERVICE_NOTIFICATION_NT3X);
	    return;
	}
	hardware.Name=buf;

	int r=m_ID.GetLineCount();
	if(m_realTime.GetCheck()==false)
	{
		hardware.runtime=false;
	m_date1.GetTime(time);
	buf.Format("%d.%d.%d %d:%d:%d",time.GetDay(),time.GetMonth(),time.GetYear(),time.GetHour(),time.GetMinute(),time.GetSecond());
    hardware.DateTime1=buf;

	m_date2.GetTime(time);
	buf.Format("%d.%d.%d %d:%d:%d",time.GetDay(),time.GetMonth(),time.GetYear(),time.GetHour(),time.GetMinute(),time.GetSecond());
	hardware.DateTime2=buf;
	}
	else
		hardware.runtime=true;
	
	if(GPS->TestIdentityHard(&hardware)==false)
	{
		AfxMessageBox("Устройство с таким же именем и с таким же временем загрузки уже добавлено.",MB_USERICON|MB_OK|MB_SERVICE_NOTIFICATION_NT3X);
	    return;
	}
//отмечаем, в каких окнах отображать hardware
/*	for(i=0;i<CheckListBox->GetCount();i++)
		if(CheckListBox->GetCheck(i)==true)
		{
			FrameIndex=FrameContainer->GetFrameIndex(i);
			hardware.WindowsList.push_back(FrameIndex);
		}
*/
	GPS->AddGPSHardware(hardware); 
	UpdateData();
}
//*********************************************************************************************************************
void CGPSDlgList::OnBnClickedCommand1()
{
	OnOK();
}

void CGPSDlgList::OnBnClickedButton3()
{
   
   unsigned int i;
   int j;
 if(m_HardList.GetCount()>0)
 {
     for(i=0;i<GPS->HardwareList.size();i++)   
     {
   	     GPS->HardwareList[i].WindowsList.clear();
	     for(j=0;j<m_WindowList->GetCount();j++)
	     {
		  	 int Wind=FrameContainer->GetFrameIndex(j);
			 GPS->HardwareList[i].WindowsList.push_back(Wind);
	     }
	  }
	 for(j=0;j<m_WindowList->GetCount();j++)
		 m_WindowList->SetCheck(j,1);
 }

}

void CGPSDlgList::OnBnClickedButton6()
{
       int j;
		 for(j=0;j<m_WindowList->GetCount();j++)
		 m_WindowList->SetCheck(j,1);
		 OnCheckListChange();
}
