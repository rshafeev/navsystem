// DlgPrivList.cpp : implementation file
//
#include "afx.h"
#include "stdafx.h"
#include "3DEditor.h"
#include "DlgPrivList.h"
#include "Geography.h"
#include "CompLib.h"
// CDlgPrivList dialog

IMPLEMENT_DYNAMIC(CDlgPrivList, CDialog)

CDlgPrivList::CDlgPrivList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrivList::IDD, pParent)
{
  
}

CDlgPrivList::~CDlgPrivList()
{
}

void CDlgPrivList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);

	Linkage=FrameContainer->GetActiveFrame()->GetLinkageObj();
	if(Linkage!=NULL)
	{
		int i;
		CString g;
		m_ListCtrl.ResetContent();
		for(i=0;i<Linkage->GetPointPrivCount();i++)
		{
			g.Format("“очка є %d",i+1);
			m_ListCtrl.AddString((LPCSTR)g);
		}
	}

	DDX_Control(pDX, IDC_EDIT1, m_XCtrl);
	DDX_Control(pDX, IDC_EDIT8, m_YCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_DeGCtrl);
	DDX_Control(pDX, IDC_EDIT3, m_DeMCtrl);
	DDX_Control(pDX, IDC_EDIT4, m_DeSCtrl);
	DDX_Control(pDX, IDC_EDIT5, m_LoGCtrl);
	DDX_Control(pDX, IDC_EDIT6, m_LoMCtrl);
	DDX_Control(pDX, IDC_EDIT7, m_LoSCtrl);
	DDX_Control(pDX, IDC_BUTTON4, m_DelBtn);
	DDX_Control(pDX, IDC_BUTTON3, m_AddBtn);
	DDX_Control(pDX, IDC_BUTTON2, m_RewriteBtn);
	DDX_Control(pDX, IDC_BUTTON5, m_KoefCtrl);
	if(m_ListCtrl.GetCount()<0||m_ListCtrl.GetCurSel()<0)
	{
		m_DelBtn.EnableWindow(0);
		m_RewriteBtn.EnableWindow(0);
	}
	if(m_ListCtrl.GetCount()==0)
		m_KoefCtrl.EnableWindow(0);
	else
		m_KoefCtrl.EnableWindow(1);

}


BEGIN_MESSAGE_MAP(CDlgPrivList, CDialog)
	ON_BN_CLICKED(IDC_COMMAND1, &CDlgPrivList::OnBnClickedCommand1)
	ON_WM_CREATE()
	ON_LBN_SELCHANGE(IDC_LIST1, &CDlgPrivList::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgPrivList::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgPrivList::OnBnClickedDel)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgPrivList::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgPrivList::OnBnClickedButton5)
END_MESSAGE_MAP()


// CDlgPrivList message handlers

void CDlgPrivList::OnBnClickedCommand1()
{
	OnOK();
}

int CDlgPrivList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDlgPrivList::OnLbnSelchangeList1()
{
	if(Linkage!=NULL&&Linkage->GetPointPrivCount()>0)
	{
	  int ind=m_ListCtrl.GetCurSel();
	  
	  TPointPriviazka Priv;
	  if(Linkage->GetPointPriv(&Priv,ind)==true&&ind>=0)
	  {
         CString buf;
		 int G,M; double S;
         //заносим значени€ широты
		 ConvertDoubleToGMS(Priv.Dest,&G,&M,&S);
		 m_DeGCtrl.Clear();
		 buf.Format("%d",G);
		 m_DeGCtrl.SetWindowTextA(buf);
		 buf.Format("%d",M);
		 m_DeMCtrl.SetWindowTextA(buf);
		 buf.Format("%f",S);
		 m_DeSCtrl.SetWindowTextA(buf);
          //заносим значени€ долготы
		 ConvertDoubleToGMS(Priv.Lon,&G,&M,&S);
		 m_LoGCtrl.Clear();
		 buf.Format("%d",G);
		 m_LoGCtrl.SetWindowTextA(buf);
		 buf.Format("%d",M);
		 m_LoMCtrl.SetWindowTextA(buf);
		 buf.Format("%f",S);
		 m_LoSCtrl.SetWindowTextA(buf);
         //заносим значени€ X Y положени€ на карте
		 buf.Format("%d",Priv.MapX);
		 m_XCtrl.SetWindowTextA(buf);
		 buf.Format("%d",Priv.MapY);
		 m_YCtrl.SetWindowTextA(buf);

		 m_DelBtn.EnableWindow(1);
		 m_RewriteBtn.EnableWindow(1);
	  }
      
	}
}

void CDlgPrivList::OnBnClickedAdd()
{
	Linkage->AddPoint(0,0,0.0,0.0);
	UpdateData();
	m_ListCtrl.SetCurSel(m_ListCtrl.GetCount()-1);
	OnLbnSelchangeList1();
}

void CDlgPrivList::OnBnClickedDel()
{
	int ind=m_ListCtrl.GetCurSel();
	if(ind>=0)
		Linkage->DeletePoint(ind);
	UpdateData();


}

void CDlgPrivList::OnBnClickedButton2()
{
		int ind=m_ListCtrl.GetCurSel();
		int X=0,Y=0;
		double De=0.0,Lo=0.0;

		X=atoi((LPCSTR)GetStrFromEdit(&m_XCtrl));
		Y=atoi((LPCSTR)GetStrFromEdit(&m_YCtrl));
		De=ConvertGMSToDouble(atoi((LPCSTR)GetStrFromEdit(&m_DeGCtrl)),
			                  atoi((LPCSTR)GetStrFromEdit(&m_DeMCtrl)),
							  atof((LPCSTR)GetStrFromEdit(&m_DeSCtrl)));
		Lo=ConvertGMSToDouble(atoi((LPCSTR)GetStrFromEdit(&m_LoGCtrl)),
			                  atoi((LPCSTR)GetStrFromEdit(&m_LoMCtrl)),
							  atof((LPCSTR)GetStrFromEdit(&m_LoSCtrl)));
	if(ind>=0)
	{
		Linkage->RewriteElem(ind,X,Y,De,Lo);
		
	}

}

void CDlgPrivList::OnBnClickedButton5()
{
	Linkage->GenerateKoefPriv();
}
