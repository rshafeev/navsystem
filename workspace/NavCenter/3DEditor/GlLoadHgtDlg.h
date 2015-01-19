#pragma once
#include "afxwin.h"


// CGlLoadHgtDlg dialog

class CGlLoadHgtDlg : public CDialog
{
	DECLARE_DYNAMIC(CGlLoadHgtDlg)

public:
	CGlLoadHgtDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGlLoadHgtDlg();

// Dialog Data
	enum { IDD = IDD_GL_DIALOG_LAND1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnSelectPath();
	CEdit m_EditPath;
	afx_msg void OnBnClickedOk();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CEdit m_StemCtrl;


	CButton m_sglag;
	CButton m_FilterFractal;
	CButton m_FilterRazmit;
	CButton m_FilterShym;
	CButton m_ScaleCheck;
	CEdit m_ScaleMin;
	CEdit m_ScaleMax;
	CEdit m_ScaleX;
	CEdit m_ScaleY;
	afx_msg void OnScaleCheck();
};
