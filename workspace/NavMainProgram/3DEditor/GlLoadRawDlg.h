#pragma once
#include "afxwin.h"


// CGlLoadRawDlg dialog

class CGlLoadRawDlg : public CDialog
{
	DECLARE_DYNAMIC(CGlLoadRawDlg)

public:
	CGlLoadRawDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGlLoadRawDlg();
   int w,h;
   int fSize;
// Dialog Data
	enum { IDD = IDD_GL_DIALOG_LAND2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnLoadLand();
	afx_msg void OnBnSelect();
	CEdit m_EditPath;
	CEdit m_w;
	CEdit m_h;
	CEdit m_Step;

	CButton m_FilterTailing;
	CButton m_FilterEverage;
	CButton m_FilterFractal;
	CButton m_FilterSmesh;
	CEdit m_ScaleX;
	CEdit m_ScaleY;
	CEdit m_ScaleMin;
	CEdit m_ScaleMax;
};
