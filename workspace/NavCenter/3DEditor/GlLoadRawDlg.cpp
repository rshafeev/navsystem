// GlLoadRawDlg.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "GlLoadRawDlg.h"
#include "CompLib.h"
#include "memory.h"

// CGlLoadRawDlg dialog

IMPLEMENT_DYNAMIC(CGlLoadRawDlg, CDialog)

CGlLoadRawDlg::CGlLoadRawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGlLoadRawDlg::IDD, pParent)
{

}

CGlLoadRawDlg::~CGlLoadRawDlg()
{
}

void CGlLoadRawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditPath);
	DDX_Control(pDX, IDC_EDIT2, m_w);
	DDX_Control(pDX, IDC_EDIT3, m_h);
	DDX_Control(pDX, IDC_EDIT5, m_Step);
	DDX_Control(pDX, IDC_CHECK5, m_FilterTailing);
	DDX_Control(pDX, IDC_CHECK2, m_FilterEverage);
	DDX_Control(pDX, IDC_CHECK3, m_FilterFractal);
	DDX_Control(pDX, IDC_CHECK4, m_FilterSmesh);
	DDX_Control(pDX, IDC_EDIT4, m_ScaleX);
	DDX_Control(pDX, IDC_EDIT6, m_ScaleY);
	DDX_Control(pDX, IDC_EDIT8, m_ScaleMin);
	DDX_Control(pDX, IDC_EDIT12, m_ScaleMax);

	m_Step.SetWindowTextA("6");
	m_ScaleX.SetWindowTextA("1");
	m_ScaleY.SetWindowTextA("1");
	m_ScaleMin.SetWindowTextA("0");
	m_ScaleMax.SetWindowTextA("255");
	m_FilterTailing.SetCheck(1);


}


BEGIN_MESSAGE_MAP(CGlLoadRawDlg, CDialog)
	ON_BN_CLICKED(IDOK2, &CGlLoadRawDlg::OnBnLoadLand)
	ON_BN_CLICKED(IDC_BUTTON1, &CGlLoadRawDlg::OnBnSelect)
END_MESSAGE_MAP()


// CGlLoadRawDlg message handlers

void CGlLoadRawDlg::OnBnLoadLand()
{
	
  if(IsFileExists(GetStrFromEdit(&m_EditPath))==false)
  {
     AfxMessageBox ("Выбранный файл не существует.", MB_ICONWARNING);
	 return;
  }
  if(GetStrFromEdit(&m_Step)=="")
  {
     AfxMessageBox ("Задайте шаг рендеринга.", MB_ICONWARNING);
	 return;

  }
   	CString WS,HS;
	WS=GetStrFromEdit(&m_w);
	HS=GetStrFromEdit(&m_h);
	w=atoi((LPCSTR)WS);
	h=atoi((LPCSTR)HS);
   
  if(w*h!=fSize)
  {
     AfxMessageBox ("Высота или ширина введены неправильно.", MB_ICONWARNING);
	 return;
  }

  if(FrameContainer->GetActiveFrame()!=NULL)
  {
			 TLandProperties Land;
			 Land.ElemSize=1;//+
			 Land.StepSize=atoi((LPCSTR)GetStrFromEdit(&m_Step));//+
			 Land.width=w; //+
			 Land.height=h;//+
			 Land.LandFile=GetStrFromEdit(&m_EditPath);//+
			 Land.MinHeight=atoi((LPCSTR)GetStrFromEdit(&m_ScaleMin));
			 Land.MaxHeight=atoi((LPCSTR)GetStrFromEdit(&m_ScaleMax));
			 Land.FilterTaling= m_FilterTailing.GetCheck();
			 Land.FilterFractal= m_FilterFractal.GetCheck();
			 Land.FilterAverage= m_FilterEverage.GetCheck();
			 Land.FilterSmooth= m_FilterSmesh.GetCheck();
			 Land.ScaleX=atof((LPCSTR)GetStrFromEdit(&m_ScaleX));
			 Land.ScaleY=atof((LPCSTR)GetStrFromEdit(&m_ScaleY));
			 Land.AutoCalcMaxMinHeight= 1;
           FrameContainer->GetActiveFrame()->InitLand(Land);
/*	FrameContainer->GetActiveFrame()->InitLand(GetStrFromEdit(&m_EditPath),1,w,h,atoi((LPCSTR)GetStrFromEdit(&m_Step)),
		m_FilterTailing.GetCheck(),m_FilterFractal.GetCheck(),m_FilterEverage.GetCheck(),m_FilterSmesh.GetCheck());
 */
  }
	OnOK();
}

void CGlLoadRawDlg::OnBnSelect()
{
	 CFileDialog File(true,"raw",NULL, OFN_NOLONGNAMES,"Карта высот|*.raw||");
		if( File.DoModal()==IDOK)
		{
		  CString str;
		  m_EditPath.SetWindowTextA(File.GetPathName());
		  fSize=GetFileSize(File.GetPathName());
		  double f=sqrt((double) fSize);
          
		  if(int(f*f)==fSize)
		  {
			  w=(int)f;
			  h=(int)f;
		     str.Format("%d",(int)f);
		     m_h.SetWindowTextA(str);
		     str.Format("%d",(int)f);
		     m_w.SetWindowTextA(str);
		  }
		  else
		  {

		  }

		}
}

