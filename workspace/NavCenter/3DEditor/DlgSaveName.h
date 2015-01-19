#pragma once
#include "afxwin.h"


// CDlgSaveName dialog

class CDlgSaveName : public CDialog
{
	DECLARE_DYNAMIC(CDlgSaveName)
    CString *FName;
public:
	CDlgSaveName(CWnd* pParent = NULL,CString *FName=NULL);   // standard constructor
	virtual ~CDlgSaveName();

// Dialog Data
	enum { IDD = IDD_DIALOGSAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_EditCtrl;
};
