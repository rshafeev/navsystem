// ÑOpenResDlg.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "OpenResDlg.h"


// ÑOpenResDlg dialog

IMPLEMENT_DYNAMIC(ÑOpenResDlg, CDialog)

ÑOpenResDlg::ÑOpenResDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ÑOpenResDlg::IDD, pParent)
	, m_RadioBlock(false)
{

}

ÑOpenResDlg::~ÑOpenResDlg()
{
}

void ÑOpenResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ÑOpenResDlg, CDialog)
END_MESSAGE_MAP()


// ÑOpenResDlg message handlers
