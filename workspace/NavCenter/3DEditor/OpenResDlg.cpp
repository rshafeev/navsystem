// ÑOpenResDlg.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "OpenResDlg.h"

#include "MapFrame.h"
#include "WorldMapFrame.h"

#include "Memory.h"
#include "CompLib.h"
// ÑOpenResDlg dialog

IMPLEMENT_DYNAMIC(ÑOpenResDlg, CDialog)

ÑOpenResDlg::ÑOpenResDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ÑOpenResDlg::IDD, pParent)
{
WayBlock="";
WayRastr="";
}

ÑOpenResDlg::~ÑOpenResDlg()
{
}

void ÑOpenResDlg::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO1, m_radioBlock);
	DDX_Control(pDX, IDC_RADIO2, m_radioRastr);
	DDX_Control(pDX, IDC_EDIT1, m_Way);

	
	if(m_radioBlock.GetCheck()==0&&m_radioRastr.GetCheck()==0)
		m_radioRastr.SetCheck(1);

}


BEGIN_MESSAGE_MAP(ÑOpenResDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &ÑOpenResDlg::OnBnSelectPuth)
	ON_BN_CLICKED(1, &ÑOpenResDlg::OnBnClicked1)
	ON_BN_CLICKED(IDC_RADIO1, &ÑOpenResDlg::OnBnClickedRadioBlock)
	ON_BN_CLICKED(IDC_RADIO2, &ÑOpenResDlg::OnBnClickedRadioRastr)
END_MESSAGE_MAP()


// ÑOpenResDlg message handlers


void ÑOpenResDlg::OnBnSelectPuth()
{
 if(m_radioRastr.GetCheck()==1)
 {
	 CFileDialog File(true,"bmp",NULL,OFN_NOLONGNAMES   ,"Èçîáðàæåíèå|*.bmp||");
	 if(File.DoModal()==IDOK)
     {
		 WayRastr= File.GetPathName();
		 m_Way.SetWindowTextA(WayRastr);
	}
 }
 else
 if(m_radioBlock.GetCheck()==1)
 {
	 CDirDialog DirDialog;
	 if(DirDialog.DoModal(this->m_hWnd)==true)
		 WayBlock=DirDialog.GetPath();
	  m_Way.SetWindowTextA(WayBlock);
 }
 
}

void ÑOpenResDlg::OnBnClicked1()
{
 if(m_radioRastr.GetCheck()==1)
 {
	 if(WayRastr=="")
	 {
	    MessageBox("Ïîæàëóéñòà,âûáåðèòå ðåñóðñ äëÿ åãî çàãðóçêè èëè íàæìèòå 'Îòìåíà'","Ïðåäóïðåæäåíèå!",0);
		return;
	 }
	 if(IsFileExists(WayRastr)==false)
	 {
	    MessageBox("Âûáðàííûé ôàéë íå ñóùåñòâóåò.Ïîæàëóéñòà, âûáåðèòå ñóùåñòâóþùèé ãðàôè÷åñêèé ôàéë èëè íàæìèòå 'Îòìåíà'","Ïðåäóïðåæäåíèå!",0);
		return;
	 }
	 if(IsUsingFile(WayRastr)==true)
	 {
	    MessageBox("Âûáðàííûé ôàéë óæå îòêðûòü èëè èñïîëüçóåòñÿ äðóãîé ïðîãðàììîé. Ïîæàëóéñòà, âûáåðèòå äðóãîé ãðàôè÷åñêèé ôàéë","Ïðåäóïðåæäåíèå!",0);
		return;
	 }
	 //TRY_RASTER()
	 CMapFrame *pMapFrame=new CMapFrame(WayRastr);
	   if(pMapFrame->Create(NULL,NULL,
		               WS_CHILD|WS_MINIMIZEBOX|WS_SYSMENU|WS_CAPTION|WS_THICKFRAME|WS_MAXIMIZEBOX|WS_MAXIMIZE|WS_CLIPCHILDREN,
					   CRect(0,0,800,700),MainForm,NULL)==TRUE)
	   pMapFrame->ShowWindow(SW_SHOW);

 }
 else
 if(m_radioBlock.GetCheck()==1)
 {
	 WayBlock=GetStrFromEdit(&m_Way);//~~~!!!!

	 if(WayBlock=="")
	 {
		 MessageBox("Ïîæàëóéñòà,âûáåðèòå ðåñóðñ äëÿ åãî çàãðóçêè èëè íàæìèòå 'Îòìåíà'","Ïðåäóïðåæäåíèå!",MB_ICONWARNING);
		return;
	 }
	 CWorldMapFrame *pWorldMapFrame=new CWorldMapFrame(WayBlock+"\\");

	  pWorldMapFrame->Create(NULL,NULL,
		               WS_CHILD|WS_MINIMIZEBOX|WS_SYSMENU|WS_CAPTION|WS_THICKFRAME|WS_MAXIMIZEBOX|WS_MAXIMIZE|WS_CLIPCHILDREN ,
		              CRect(0,0,260*3,260*3),MainForm,NULL);
	  pWorldMapFrame->ShowWindow(SW_SHOW);

 }
 OnOK();
}

void ÑOpenResDlg::OnBnClickedRadioBlock()
{
	 m_Way.SetWindowTextA(WayBlock);
	
}

void ÑOpenResDlg::OnBnClickedRadioRastr()
{
 m_Way.SetWindowTextA(WayRastr);
}
