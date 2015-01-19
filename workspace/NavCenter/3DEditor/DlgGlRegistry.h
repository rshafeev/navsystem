#pragma once
#include "afxwin.h"


// CDlgGlRegistry dialog

class CDlgGlRegistry : public CDialog
{
	DECLARE_DYNAMIC(CDlgGlRegistry)

public:
	CDlgGlRegistry(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGlRegistry();

// Dialog Data
	enum { IDD = IDD_GL_REGISTRY };
    
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_CategoryCtrl;
	CEdit m_AddCategoryEditCtrl;
	CButton m_AddCategoryBtnCtrl;
	CButton m_SelectFileBtnCtrl;
	CEdit m_SelectFileEditCtrl;
	CEdit m_CaptionEditCtrl;
	CButton m_RegistryBtn;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnSelectFile();
	afx_msg void OnBnAddCategory();
	afx_msg void OnBnRegistry();

private:
void    ReloadList();                         //обновляет список категорий
bool    WriteCategory(CString category);       //записывает новую категорию в таблицу Category
bool    WriteFileInfoToDB(CString category,CString caption,CString FileName); //регестр. объект, т.е. записывает имя файла в БД
bool    FindCaptionInDB(CString caption);//есть ли модель с таким именем в базе данных?
};
