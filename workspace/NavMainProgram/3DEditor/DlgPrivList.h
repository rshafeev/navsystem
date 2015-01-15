#pragma once
#include "afxwin.h"
#include "Linkage.h"

// CDlgPrivList dialog

class CDlgPrivList : public CDialog
{
	DECLARE_DYNAMIC(CDlgPrivList)
CLinkage *Linkage;
public:
	CDlgPrivList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPrivList();

// Dialog Data
	enum { IDD = IDD_LISTPOINTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCommand1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CListBox m_ListCtrl;
	afx_msg void OnLbnSelchangeList1();
	CEdit m_XCtrl;
	CEdit m_YCtrl;
	CEdit m_DeGCtrl;
	CEdit m_DeMCtrl;
	CEdit m_DeSCtrl;
	CEdit m_LoGCtrl;
	CEdit m_LoMCtrl;
	CEdit m_LoSCtrl;
	CButton m_DelBtn;
	CButton m_AddBtn;
	CButton m_RewriteBtn;
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	CButton m_KoefCtrl;
};
