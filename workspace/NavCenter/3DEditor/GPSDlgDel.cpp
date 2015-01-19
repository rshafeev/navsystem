// GPSDlgDel.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "GPSDlgDel.h"


// CGPSDlgDel dialog
CGPSDlgDel* GPSDlgDel;
IMPLEMENT_DYNAMIC(CGPSDlgDel, CDialog)

CGPSDlgDel::CGPSDlgDel(CWnd* pParent /*=NULL*/)
	: CDialog(CGPSDlgDel::IDD, pParent)
{

}

CGPSDlgDel::~CGPSDlgDel()
{
}

void CGPSDlgDel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGPSDlgDel, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CGPSDlgDel::OnTvnSelchangedTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CGPSDlgDel::OnNMDblclkTree1)
END_MESSAGE_MAP()


// CGPSDlgDel message handlers

void CGPSDlgDel::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CGPSDlgDel::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
