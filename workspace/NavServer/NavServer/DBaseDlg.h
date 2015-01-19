#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxdtctl.h"


// CDBaseDlg dialog

class CDBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CDBaseDlg)

public:
	CDBaseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDBaseDlg();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_BASEDIALOG};
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	 void ReloadList ();
	 void ReloadHards();

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:



	 CListCtrl m_listCtrl;
	 CButton TimeChack;
	 CComboBox ComboHards;
	 CDateTimeCtrl DateFrom;
	 afx_msg void OnClose();
	 afx_msg void OnBnClickedOk();
};
