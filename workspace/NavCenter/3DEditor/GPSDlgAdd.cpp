// GPSDlgAdd.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "GPSDlgAdd.h"
#include "mathem.h"
#include "CompLib.h"
// CGPSDlgAdd dialog
CGPSDlgAdd* GPSDlgAdd;
IMPLEMENT_DYNAMIC(CGPSDlgAdd, CDialog)

CGPSDlgAdd::CGPSDlgAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CGPSDlgAdd::IDD, pParent)
	, m_IDChar(0)
{
	
CheckListBox=new CCheckListBox;

}

CGPSDlgAdd::~CGPSDlgAdd()
{
	delete    CheckListBox;
}
void CGPSDlgAdd::DoDataExchange(CDataExchange* pDX)
{


	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ID);
	DDX_Control(pDX, IDC_EDIT2, m_name);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_date1);
	DDX_Control(pDX, IDC_DATETIMEPICKER3, m_time1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_date2);
	DDX_Control(pDX, IDC_DATETIMEPICKER4, m_time2);
	DDX_Control(pDX, IDC_RADIO1, m_RealTimeCtrl);
	DDX_Control(pDX, IDC_RADIO2, m_CBDTimeCtrl);

	if(m_RealTimeCtrl.GetCheck()==0&&m_CBDTimeCtrl.GetCheck()==0)
	{
	  m_RealTimeCtrl.SetCheck(1);
	  m_CBDTimeCtrl.SetCheck(0);
	}
}


BEGIN_MESSAGE_MAP(CGPSDlgAdd, CDialog)
	ON_BN_CLICKED(IDSEND, &CGPSDlgAdd::OnBnClickedSend)
	ON_BN_CLICKED(IDCANCEL, &CGPSDlgAdd::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &CGPSDlgAdd::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CGPSDlgAdd::OnBnClickedRadio2)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CGPSDlgAdd message handlers


void CGPSDlgAdd::OnBnClickedSend()
{



	UpdateData();
	THardware hardware;
	CString buf,st;
	CTime time;
	int FrameIndex;
	int i;
	//char buf1[258],buf2[258];
	

    /*CString f=CString(m_IDChar);
	m_ID.GetLine(0,buf1);
	
	buf1[min(255,m_ID.LineLength(0))]=NULL;
    t1=CString(buf1);*/
	buf=GetStrFromEdit(&m_ID);
    if(buf=="")
	{
		AfxMessageBox("Заполните все поля",MB_USERICON|MB_OK|MB_SERVICE_NOTIFICATION_NT3X);
	    return;
	}
	hardware.ID=buf;

	buf=GetStrFromEdit(&m_name);
    if(buf=="")
	{
		AfxMessageBox("Заполните все поля",MB_USERICON|MB_OK|MB_SERVICE_NOTIFICATION_NT3X);
	    return;
	}
	hardware.Name=buf;

	int r=m_ID.GetLineCount();
	if(m_RealTimeCtrl.GetCheck()==false)
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
		AfxMessageBox("Устройство с таким же именем и с таким же временем уже есть.",MB_USERICON|MB_OK|MB_SERVICE_NOTIFICATION_NT3X);
	    return;
	}
//отмечаем, в каких окнах отображать hardware
	for(i=0;i<CheckListBox->GetCount();i++)
		if(CheckListBox->GetCheck(i)==1)
		{
			FrameIndex=FrameContainer->GetFrameIndex(i);
			hardware.WindowsList.push_back(FrameIndex);
		}

	GPS->AddGPSHardware(hardware);

	OnOK();
}

void CGPSDlgAdd::OnBnClickedCancel()
{
		OnCancel();
}

void CGPSDlgAdd::OnBnClickedRadio1()
{
	m_date1.EnableWindow(0);
	m_date2.EnableWindow(0);
	m_time1.EnableWindow(0);
	m_time2.EnableWindow(0);
	m_RealTimeCtrl.SetCheck(1);
	m_CBDTimeCtrl.SetCheck(0);
	UpdateData();
}

void CGPSDlgAdd::OnBnClickedRadio2()
{
	m_date1.EnableWindow(1);
	m_date2.EnableWindow(1);
	m_time1.EnableWindow(1);
	m_time2.EnableWindow(1);
	m_RealTimeCtrl.SetCheck(0);
	m_CBDTimeCtrl.SetCheck(1);
	UpdateData();

}

int CGPSDlgAdd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

/*	CCheckListBox CheckListBox;
	CheckListBox.Create(WS_CHILD|WS_VISIBLE|LBS_HASSTRINGS|LBS_OWNERDRAWVARIABLE  , 
		CRect(0,0,300,300), this, 188);
	CheckListBox.AddString("adas");
*/
	CheckListBox->Create(WS_CHILD|WS_VISIBLE|LBS_HASSTRINGS|WS_BORDER|LBS_DISABLENOSCROLL|WS_VSCROLL|WS_HSCROLL, 
		                 CRect(445,70,685,310), this, 188);

	FrameContainer->InsertToListBox(CheckListBox);
	
	for(int i=0;i<FrameContainer->GetWindowCount();i++)
   {
	       CheckListBox->SetCheck(i,1);
   }
	return 0;
}
