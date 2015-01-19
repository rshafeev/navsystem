// DlgPrivAdd.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "DlgPrivAdd.h"


// CDlgPrivAdd dialog

IMPLEMENT_DYNAMIC(CDlgPrivAdd, CDialog)

CDlgPrivAdd::CDlgPrivAdd(CWnd* pParent /*=NULL*/,CPoint Cord,CLinkage *Linkage)
: CDialog(CDlgPrivAdd::IDD, pParent)
{
  this->Linkage=Linkage;
}
CDlgPrivAdd::CDlgPrivAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrivAdd::IDD, pParent)
{
  this->Linkage=NULL;
}

CDlgPrivAdd::~CDlgPrivAdd()
{
}

void CDlgPrivAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPrivAdd, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPrivAdd::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPrivAdd message handlers

void CDlgPrivAdd::OnBnClickedOk()
{
   if(Linkage!=NULL)
	Linkage->AddPoint(1,1,1.0,1.0);
	OnOK();
}
