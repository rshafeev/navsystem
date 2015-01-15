#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CGPSDlgAdd dialog

class CGPSDlgAdd : public CDialog
{
	DECLARE_DYNAMIC(CGPSDlgAdd)

public:
	CGPSDlgAdd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGPSDlgAdd();

// Dialog Data
	enum { IDD = IDD_DIALOG_GPSADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

 
	CEdit m_ID;
	CEdit m_name;
	CDateTimeCtrl m_date1;
	CDateTimeCtrl m_time1;
	CDateTimeCtrl m_date2;
	CDateTimeCtrl m_time2;
	CButton m_RealTimeCtrl;
	CButton m_CBDTimeCtrl;
    CCheckListBox *CheckListBox;
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	char m_IDChar;
};
