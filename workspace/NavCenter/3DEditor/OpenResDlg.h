#pragma once
#include "afxwin.h"
///////////////////////////////////
/*
Диалоговое окно загрузки ресурсов
*/
///////////////////////////////////

// СOpenResDlg dialog

class СOpenResDlg : public CDialog
{
	CString WayBlock;
	CString WayRastr;
	DECLARE_DYNAMIC(СOpenResDlg)

public:

	СOpenResDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~СOpenResDlg();

// Dialog Data
	enum { IDD = IDD_CREATEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnSelectPuth();
	CButton m_radioBlock;
	CButton m_radioRastr;
	CEdit m_Way;
	afx_msg void OnBnClicked1();
	afx_msg void OnBnClickedRadioBlock();
	afx_msg void OnBnClickedRadioRastr();
};
