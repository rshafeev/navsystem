#pragma once
#include "Resource.h"
#include "afxwin.h"
#include "afxdtctl.h"

// CGPSDlgList dialog

class CGPSDlgList : public CDialog
{
	DECLARE_DYNAMIC(CGPSDlgList)
private:
    CCheckListBox *m_WindowList;
    CListBox m_HardList;
public:


	CGPSDlgList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGPSDlgList();

// Dialog Data
	enum { IDD = IDD_DIALOG_GPSHARDLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCheckListChange();//обработчик списка окон(при выборе элемента списка)	

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedCommand1();
	afx_msg void OnLbnDblclkList1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedAdd();

	CButton m_realTime;
	CButton m_FromBD;
	CEdit m_ID;
	CEdit m_Name;
	CDateTimeCtrl m_date1;
	CDateTimeCtrl m_date2;
	CDateTimeCtrl m_time1;
	CDateTimeCtrl m_time2;
	CButton m_SelectAllBtn;
	CButton m_NotSelectAllBtn;

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton6();

};
