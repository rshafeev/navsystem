#pragma once
#include "afxwin.h"


// CGlLandTexDlg dialog

class CGlLandTexDlg : public CDialog
{
	DECLARE_DYNAMIC(CGlLandTexDlg)
    CString stex1,stex2,stex3,svec;
public:
	CGlLandTexDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGlLandTexDlg();

// Dialog Data
	enum { IDD = IDD_GL_DIALOG_TEX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnTex1();
	afx_msg void OnBnTex2();
	afx_msg void OnBnTex3();
	afx_msg void OnBnVec3();
	afx_msg void OnBnLoad();
	CButton m_BnLoad;
	CButton m_Tex1;
	CButton m_BnTex2;
	CButton m_BnTex3;
	CButton m_BnVec;
	CButton m_CheckShade;
	CButton m_BnAddTex;
	CButton m_BnDelTex;
	CListBox m_ListTex;
	CEdit m_EditTex1;
	CEdit m_EditTex2;
	CEdit m_EditTex3;
	CEdit m_EditVec;
	afx_msg void onBnAdd();
};
