// GlLandTexDlg.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "GlLandTexDlg.h"
#include "CompLib.h"
#include "memory.h"

// CGlLandTexDlg dialog

IMPLEMENT_DYNAMIC(CGlLandTexDlg, CDialog)

CGlLandTexDlg::CGlLandTexDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGlLandTexDlg::IDD, pParent)
{

}

CGlLandTexDlg::~CGlLandTexDlg()
{
}

void CGlLandTexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_BnLoad);
	DDX_Control(pDX, IDC_BUTTON4, m_Tex1);
	DDX_Control(pDX, IDC_BUTTON5, m_BnTex2);
	DDX_Control(pDX, IDC_BUTTON6, m_BnTex3);
	DDX_Control(pDX, IDC_BUTTON7, m_BnVec);
	DDX_Control(pDX, IDC_CHECK3, m_CheckShade);
	DDX_Control(pDX, IDC_BUTTON3, m_BnAddTex);
	DDX_Control(pDX, IDC_BUTTON2, m_BnDelTex);
	DDX_Control(pDX, IDC_LIST1, m_ListTex);
	DDX_Control(pDX, IDC_EDIT2, m_EditTex1);
	DDX_Control(pDX, IDC_EDIT9, m_EditTex2);
	DDX_Control(pDX, IDC_EDIT10, m_EditTex3);
	DDX_Control(pDX, IDC_EDIT11, m_EditVec);
}


BEGIN_MESSAGE_MAP(CGlLandTexDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4, &CGlLandTexDlg::OnBnTex1)
	ON_BN_CLICKED(IDC_BUTTON5, &CGlLandTexDlg::OnBnTex2)
	ON_BN_CLICKED(IDC_BUTTON6, &CGlLandTexDlg::OnBnTex3)
	ON_BN_CLICKED(IDC_BUTTON7, &CGlLandTexDlg::OnBnVec3)
	ON_BN_CLICKED(IDOK, &CGlLandTexDlg::OnBnLoad)
	ON_BN_CLICKED(IDC_BUTTON3, &CGlLandTexDlg::onBnAdd)
END_MESSAGE_MAP()


// CGlLandTexDlg message handlers

void CGlLandTexDlg::OnBnTex1()
{
		 CFileDialog File(true,"bmp",NULL, OFN_NOLONGNAMES,"Текстуры|*.bmp||");
		if( File.DoModal()==IDOK)
		{
		  m_EditTex1.SetWindowTextA(File.GetPathName());
		  stex1=File.GetPathName();
         }
}
void CGlLandTexDlg::OnBnTex2()
{
		 CFileDialog File(true,"bmp",NULL, OFN_NOLONGNAMES,"Текстуры|*.bmp||");
		if( File.DoModal()==IDOK)
		{
		  m_EditTex2.SetWindowTextA(File.GetPathName());
		  stex2=File.GetPathName();
         }
}

void CGlLandTexDlg::OnBnTex3()
{
		 CFileDialog File(true,"bmp",NULL, OFN_NOLONGNAMES,"Текстуры|*.bmp||");
		if( File.DoModal()==IDOK)
		{
		  m_EditTex3.SetWindowTextA(File.GetPathName());
		  stex3=File.GetPathName();
         }
}

void CGlLandTexDlg::OnBnVec3()
{
		 CFileDialog File(true,"bmp",NULL, OFN_NOLONGNAMES,"Текстуры|*.bmp||");
		if( File.DoModal()==IDOK)
		{
		  m_EditVec.SetWindowTextA(File.GetPathName());
		  svec=File.GetPathName();
         }
}

void CGlLandTexDlg::OnBnLoad()
{
	if(IsFileExists(stex1)&&
	   IsFileExists(stex2)&&
	   IsFileExists(stex3)&&
	   IsFileExists(svec))
	{
		FrameContainer->GetActiveFrame()->LandMixing(stex1,stex2,stex3,RES_PATH+"land\\textureWater1.bmp",svec);
	}
	OnOK();
}

void CGlLandTexDlg::onBnAdd()
{
		 CFileDialog File(true,"bmp",NULL, OFN_NOLONGNAMES,"Текстуры|*.bmp||");
		if( File.DoModal()==IDOK)
		{
		
		

			if(IsFileExists(File.GetPathName())==true)
			{
				FrameContainer->GetActiveFrame()->AddMapToLand(File.GetPathName());
               m_ListTex.AddString(File.GetPathName());
			}
         //   
		}
}
