// GlLoadHgtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "GlLoadHgtDlg.h"
#include "CompLib.h"
#include "memory.h"
#include "3DLib.h"
// CGlLoadHgtDlg dialog

IMPLEMENT_DYNAMIC(CGlLoadHgtDlg, CDialog)

CGlLoadHgtDlg::CGlLoadHgtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGlLoadHgtDlg::IDD, pParent)
{
   
}

CGlLoadHgtDlg::~CGlLoadHgtDlg()
{
}

void CGlLoadHgtDlg::DoDataExchange(CDataExchange* pDX)
{

	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditPath);
	DDX_Control(pDX, IDC_CHECK1, m_FilterRazmit);
	DDX_Control(pDX, IDC_CHECK2, m_FilterShym);
	DDX_Control(pDX, IDC_EDIT5, m_StemCtrl);
	DDX_Control(pDX, IDC_CHECK3, m_FilterFractal);
	DDX_Control(pDX, IDC_CHECK4, m_sglag);
	DDX_Control(pDX, IDC_CHECK5, m_ScaleCheck);
	DDX_Control(pDX, IDC_EDIT2, m_ScaleMin);
	DDX_Control(pDX, IDC_EDIT8, m_ScaleMax);
	DDX_Control(pDX, IDC_EDIT3, m_ScaleX);
	DDX_Control(pDX, IDC_EDIT4, m_ScaleY);



	m_FilterRazmit.SetCheck(true);	
	m_FilterShym.SetCheck(true);	
	m_ScaleCheck.SetCheck(true);
	m_sglag.SetCheck(true);
	m_FilterFractal.SetCheck(true);

	m_ScaleX.SetWindowTextA("3");
	m_ScaleY.SetWindowTextA("3");
	m_ScaleMin.SetWindowTextA(" -10");
	m_ScaleMax.SetWindowTextA("1000");
	m_StemCtrl.SetWindowTextA("3");
}


BEGIN_MESSAGE_MAP(CGlLoadHgtDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CGlLoadHgtDlg::OnBnSelectPath)
	ON_BN_CLICKED(IDOK, &CGlLoadHgtDlg::OnBnClickedOk)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK5, &CGlLoadHgtDlg::OnScaleCheck)
END_MESSAGE_MAP()


// CGlLoadHgtDlg message handlers

void CGlLoadHgtDlg::OnBnSelectPath()
{
		 CFileDialog File(true,"hgt",NULL, OFN_NOLONGNAMES,"Карта высот|*.hgt||");
		if( File.DoModal()==IDOK)
		{
		  m_EditPath.SetWindowTextA(File.GetPathName());
		}

}

void CGlLoadHgtDlg::OnBnClickedOk()
{
	if(GetStrFromEdit(&m_StemCtrl)=="")
	{
          AfxMessageBox ("Введите шаг рендеринга.", MB_ICONWARNING);
         return;
	}
	if(IsFileExists(GetStrFromEdit(&m_EditPath))==true)
  {
	 int FilterType;
	 
	     if(FrameContainer->GetActiveFrame()!=NULL)
		 {
			 TLandProperties Land;
			 Land.ElemSize=2;
			 Land.StepSize=atoi((LPCSTR)GetStrFromEdit(&m_StemCtrl));
			 Land.height=1201;
			 Land.width=1201;
			 Land.LandFile=GetStrFromEdit(&m_EditPath);
			 Land.MinHeight=atoi((LPCSTR)GetStrFromEdit(&m_ScaleMin));
			 Land.MaxHeight=atoi((LPCSTR)GetStrFromEdit(&m_ScaleMax));
			 Land.FilterTaling= m_FilterRazmit.GetCheck();
			 Land.FilterFractal= m_FilterFractal.GetCheck();
			 Land.FilterAverage= m_FilterShym.GetCheck();
			 Land.FilterSmooth= m_sglag.GetCheck();
			 Land.ScaleX=atof((LPCSTR)GetStrFromEdit(&m_ScaleX));
			 Land.ScaleY=atof((LPCSTR)GetStrFromEdit(&m_ScaleY));
			 Land.AutoCalcMaxMinHeight= m_ScaleCheck.GetCheck();
		     FrameContainer->GetActiveFrame()->InitLand(Land);
		 }
         OnOK();
  }
	else
		 AfxMessageBox ("Выбранный файл не существует.", MB_ICONWARNING);
	
}

int CGlLoadHgtDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	

	return 0;
}

void CGlLoadHgtDlg::OnScaleCheck()
{
	if(m_ScaleCheck.GetCheck()==0)
	{
		m_ScaleMin.EnableWindow(1);
		m_ScaleMax.EnableWindow(1);
	}
	else
	{
		m_ScaleMin.EnableWindow(0);
		m_ScaleMax.EnableWindow(0);
	}
}
