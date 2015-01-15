#pragma once


// CGPSDlgDel dialog

class CGPSDlgDel : public CDialog
{
	DECLARE_DYNAMIC(CGPSDlgDel)

public:
	CGPSDlgDel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGPSDlgDel();

// Dialog Data
	enum { IDD = IDD_DIALOG_GPS_DEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
