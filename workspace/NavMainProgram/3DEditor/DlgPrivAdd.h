#pragma once
#include "Raster.h"
#include "Linkage.h"
// CDlgPrivAdd dialog

class CDlgPrivAdd : public CDialog
{
	DECLARE_DYNAMIC(CDlgPrivAdd)
private:
	CLinkage *Linkage;  //указатель на объекст Raster - класс обработки изображения
public:
	CDlgPrivAdd(CWnd* pParent = NULL);   // standard constructor
	CDlgPrivAdd(CWnd* pParent,CPoint Cord,CLinkage *Linkage);   // standard constructor
	virtual ~CDlgPrivAdd();

// Dialog Data
	enum { IDD = IDD_ADDPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
