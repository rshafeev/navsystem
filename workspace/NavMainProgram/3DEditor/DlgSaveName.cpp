// DlgSaveName.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "DlgSaveName.h"
#include "CompLib.h"

// CDlgSaveName dialog

IMPLEMENT_DYNAMIC(CDlgSaveName, CDialog)

CDlgSaveName::CDlgSaveName(CWnd* pParent /*=NULL*/,CString *FName /*=NULL*/)
	: CDialog(CDlgSaveName::IDD, pParent)
{
this->FName=FName;
}

CDlgSaveName::~CDlgSaveName()
{
}

void CDlgSaveName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditCtrl);
}


BEGIN_MESSAGE_MAP(CDlgSaveName, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSaveName::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgSaveName::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSaveName message handlers

void CDlgSaveName::OnBnClickedOk()
{
	if(GetStrFromEdit(&m_EditCtrl)!="")
	{
	  *FName=GetStrFromEdit(&m_EditCtrl);
	  OnOK();
	}
}

void CDlgSaveName::OnBnClickedCancel()
{
	
	OnCancel();
}
