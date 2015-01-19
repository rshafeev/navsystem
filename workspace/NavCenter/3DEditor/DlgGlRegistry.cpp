// DlgGlRegistry.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "DlgGlRegistry.h"
#include "3ds.h"
#include "CompLib.h"
#include "memory.h"

#include "3DLib.h"
#include "adoclasses.h"
#include "adodefs.h"
// CDlgGlRegistry dialog

IMPLEMENT_DYNAMIC(CDlgGlRegistry, CDialog)

CDlgGlRegistry::CDlgGlRegistry(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGlRegistry::IDD, pParent)
{

}

CDlgGlRegistry::~CDlgGlRegistry()
{
}

void CDlgGlRegistry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_CategoryCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_AddCategoryEditCtrl);
	DDX_Control(pDX, IDC_BUTTON1, m_AddCategoryBtnCtrl);
	DDX_Control(pDX, IDC_BUTTON2, m_SelectFileBtnCtrl);
	DDX_Control(pDX, IDC_EDIT3, m_SelectFileEditCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_CaptionEditCtrl);
	DDX_Control(pDX, IDOK, m_RegistryBtn);

	ReloadList();
    CString g="Ввидете название модели";
	m_CaptionEditCtrl.SetWindowTextA(g);
	m_CaptionEditCtrl.SetFocus();
	
	if(m_CategoryCtrl.GetCount()>0)
		m_CategoryCtrl.SetCurSel(0);
} 


BEGIN_MESSAGE_MAP(CDlgGlRegistry, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDlgGlRegistry::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgGlRegistry::OnBnSelectFile)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgGlRegistry::OnBnAddCategory)
	ON_BN_CLICKED(IDOK, &CDlgGlRegistry::OnBnRegistry)
END_MESSAGE_MAP()


// CDlgGlRegistry message handlers

void CDlgGlRegistry::OnBnClickedCancel()
{
	
	OnCancel();
}

void CDlgGlRegistry::OnBnSelectFile()
{
	CFileDialog FileDlg(true,"3ds",NULL,OFN_NOLONGNAMES   ,"Объект 3ds|*.3ds||");
	if(FileDlg.DoModal()==IDOK)
	{
		m_SelectFileEditCtrl.SetWindowTextA(FileDlg.GetPathName());
	}
}

void CDlgGlRegistry::OnBnAddCategory()
{
	WriteCategory(GetStrFromEdit(&m_AddCategoryEditCtrl));
	m_CategoryCtrl.AddString(GetStrFromEdit(&m_AddCategoryEditCtrl));

}
//=======================================================================================================================
bool CDlgGlRegistry::FindCaptionInDB(CString caption)
{
	CString captDB="";
	CString sqlStr;
	sqlStr="select [Caption] from DATA WHERE Caption='"+caption;
	sqlStr+="'";
   TRY_ADO()
      CADORecordset rs;
  
      rs.SetConnection (&theApp.m_GlConnection);

	  rs.SetSQL (sqlStr);
      rs.SetOpenMode (true);
      rs.Open ();
 if(rs.IsEmpty()==true) return false;
   else
	    return true; 
 /*captDB=(const char*)((_bstr_t)rs.GetField("Caption"));
	  rs.Close();*/
     CATCH_ADO()
  if(captDB!="")
    return true;
 return false;
}
//=======================================================================================================================
void CDlgGlRegistry::OnBnRegistry()
{
	
	CString caption=GetStrFromEdit(&m_CaptionEditCtrl);
	CString FulFileName;
	FulFileName=GetStrFromEdit(&m_SelectFileEditCtrl);
//	CString NewFileName=RES_PATH+"\\Models\\"+GetFileName(FulFileName);
	CString FileName=GetFileName(FulFileName);
	CString OldPath=GetPuth(FulFileName)+"\\",NewPath=RES_PATH+"\\Models\\"+GetFileNameWithoutExp(FileName)+"\\";

	if(caption=="")
	{
		AfxMessageBox("Пожалуйста, задайте имя новому объекту. ");
        return;
	}
	if(FulFileName=="")
	{
		AfxMessageBox("Пожалуйста, выберите файл. ");
        return;
	}
	if(IsFileExists(NewPath+FileName)==true)
	{
		AfxMessageBox("Файл с таким именем уже добавлен в БД. ");
        return;
	}
	if(m_CategoryCtrl.GetCurSel()<0)
	{
		AfxMessageBox("Выберите категорию. ");
        return;
	}
    if(IsFileExists(FulFileName)==false)
	{
		AfxMessageBox("Выбранный файл не существует. ");
        return;
	}
	if(FindCaptionInDB(caption)==true)
	{
		AfxMessageBox("Модель с таким названием уже есть в базе данных. ");
        return;
	}
//загружаем объект для проверки...
	CLoad3DS dsTest;
	t3DModel  Model;

	if(dsTest.Import3DS(&Model,(LPCSTR)FulFileName)!=true)
	{
		AfxMessageBox("Выбранный файл не имеет неправильную структуру. Объект не зарегестрирован.");
	    Model.pMaterials.clear();
	    Model.pObject.clear();
		return;          
	}
		CString f;

	for(int i=0;i<Model.pMaterials.size();i++)
	{
		f=Model.pMaterials[i].strFile;
        
		if(f!="" && IsFileExists(OldPath+"\\"+f)==false)
		{
			CString g;
			g.Format("В папке с объектом отсутствует текстура  %s .\n Добавьте в папке текстуру.\n Объект не зарегестрирован.",Model.pMaterials[i].strFile);
		   AfxMessageBox((LPCSTR)g);
	       Model.pMaterials.clear();
	       Model.pObject.clear();
		   return;       
		}
	}
/*копируем файлы в \\Models\\  */
  CreateDirectory((LPCSTR)NewPath,NULL);
	
  CopyFile(FulFileName,NewPath+FileName,0);
	 for(int i=0;i<Model.pMaterials.size();i++)
	 {
		f=Model.pMaterials[i].strFile;
		if(f!="")
		  CopyFile(OldPath+f,NewPath+f,0);
	 }
	
	CString category;
	m_CategoryCtrl.GetText(m_CategoryCtrl.GetCurSel(),category);
	WriteFileInfoToDB(category,caption,GetFileName(FulFileName));
	GlToolFrame->ReLoadCategory();

	Model.pMaterials.clear();
	Model.pObject.clear();
	OnOK();
}
//=======================================================================================================================
bool CDlgGlRegistry::WriteCategory(CString category)
{
	if(category=="") return false;
	
   TRY_ADO()
      CADORecordset rs;
      rs.SetConnection (&theApp.m_GlConnection);
	  rs.SetSQL ("select [Category]from CATEGORY order by [Category]");
      rs.SetOpenMode (true);
      rs.Open ();
	
      if (!rs.IsEmpty ())
         rs.Last ();
      rs.AddNew ();
   
      try
         {
			 rs.SetField ("Category", _variant_t(CT2CA((LPCTSTR)category)));

           rs.Update ();
         }
      catch (CADOException&)
         {
         try
            {
            rs.Cancel ();
            }
         catch (...)
            {
            }
         throw;
         }
	  rs.Close();
	  
     CATCH_ADO()

  return true;
}
//=======================================================================================================================
void CDlgGlRegistry::ReloadList ()
{
	
   TRY_ADO()
	 m_CategoryCtrl.ResetContent();
     CADORecordset rs;
	 rs.SetConnection (& theApp.m_GlConnection);
	 rs.SetSQL ("select [Category]from CATEGORY order by [Category]");
     rs.SetOpenMode (true);
     rs.Open ();
     int nItem = 0;
     if (!rs.IsEmpty ())
     for (rs.First () ; !rs.IsEoF () ; rs.Next ())
     {
		 m_CategoryCtrl.AddString((const char*)_bstr_t(rs.GetField("Category")));
     }
	 rs.Close();
      CATCH_ADO()
	  	  
   }
//=======================================================================================================================
bool CDlgGlRegistry::WriteFileInfoToDB(CString category, CString caption, CString FileName)
{
 if((category=="")||(caption=="")||(FileName==""))
	 return false;
   TRY_ADO()
      CADORecordset rs;
      rs.SetConnection (&theApp.m_GlConnection);
	  rs.SetSQL ("select [FName], [Caption], [Category] from DATA ");
      rs.SetOpenMode (true);
      rs.Open ();
	
      if (!rs.IsEmpty ())
         rs.Last ();
      rs.AddNew ();
   
      try
         {
			 rs.SetField ("FName", _variant_t(CT2CA((LPCTSTR)FileName)));
			 rs.SetField ("Caption", _variant_t(CT2CA((LPCTSTR)caption)));
			 rs.SetField ("Category", _variant_t(CT2CA((LPCTSTR)category)));
             rs.Update ();
         }
      catch (CADOException&)
         {
         try
            {
            rs.Cancel ();
            }
         catch (...)
            {
            }
         throw;
         }
	  rs.Close();
	  
     CATCH_ADO()
 return true;
}
//=======================================================================================================================