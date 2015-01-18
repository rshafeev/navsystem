// DBaseDlg.cpp : implementation file
//


#include "stdafx.h"
#include "NavServerApp.h"
#include "DBaseDlg.h"


// CDBaseDlg dialog

IMPLEMENT_DYNAMIC(CDBaseDlg, CDialog)

CDBaseDlg::CDBaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDBaseDlg::IDD, pParent)
{

	EnableAutomation();

}

CDBaseDlg::~CDBaseDlg()
{
}
//**********************************************************************************************************************
void CDBaseDlg::ReloadList ()
   {

	/*
   TRY_ADO()
      //CHECK_ADO_CONNECTION()
      m_listCtrl.DeleteAllItems ();
      CADORecordset rs;
      rs.SetConnection (& theApp.m_connection);
	  rs.SetSQL ("select num_rec,[Hardware],[HardCaption], [Longitude], [Description],[Height],[Date] from DATA order by [Hardware]");
      rs.SetOpenMode (true);
      
	  rs.Open ();
      int nItem = 0;
      if (!rs.IsEmpty ())
         for (rs.First () ; !rs.IsEoF () ; rs.Next ())
            {
				
	          m_listCtrl.InsertItem(nItem, _T(""));
              
			  CA2CT((const char*)_bstr_t(rs.GetField("Hardware")));
              m_listCtrl.SetItemText(nItem, 1, CA2CT((const char*)_bstr_t(rs.GetField("HardCaption"))));
			  m_listCtrl.SetItemText(nItem, 2, CA2CT((const char*)_bstr_t(rs.GetField("Longitude"))));
			  m_listCtrl.SetItemText(nItem, 3, CA2CT((const char*)_bstr_t(rs.GetField("Description"))));
              m_listCtrl.SetItemText(nItem, 4, CA2CT((const char*)_bstr_t(rs.GetField("Height"))));
              m_listCtrl.SetItemText(nItem, 5, CA2CT((const char*)_bstr_t(rs.GetField("Date"))));
			  m_listCtrl.SetItemData(nItem++, (DWORD)((long)rs.GetField("num_rec")));
			  
            }       
      CATCH_ADO()
	  	  */
   }
//*******************************************************************************************************************
void CDBaseDlg::ReloadHards()
{
	CString Name,Caption;

	
//	ComboHards.AddString((LPCSTR)"sadsa");
//	int a=Com8boHards.GetCount();
//	ComboHards.ShowDropDown(1);
	/*
   TRY_ADO()
      //CHECK_ADO_CONNECTION()
	  
	  //ComboHards.Clear();
      CADORecordset rs;
      rs.SetConnection (& theApp.m_connection);
	  rs.SetSQL ("select num_rec,[Name],[Caption] from IMEI order by [Name]");
      rs.SetOpenMode (true);
	  
	  rs.Open ();
      int nItem = 0;
      if (!rs.IsEmpty ())
         for (rs.First () ; !rs.IsEoF () ; rs.Next ())
            {
				Name    =  rs.GetField("Name");
				Caption =  rs.GetField("Caption");
                Name=Name+"("+Caption+")";
				ComboHards.AddString((LPCSTR)Name);

	        }     
	   rs.Close();
      CATCH_ADO()
	
	ComboHards.SetCurSel(0);
	*/
   }
//*******************************************************************************************************************
void CDBaseDlg::OnFinalRelease()
{
	CDialog::OnFinalRelease();
}

void CDBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
	DDX_Control(pDX, IDC_LIST, m_listCtrl);
	DDX_Control(pDX, IDC_CHECK1, TimeChack);
	DDX_Control(pDX, IDC_COMBO1, ComboHards);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, DateFrom);

	m_listCtrl.InsertColumn(0, _T("Устройство"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(1, _T("Название"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(2, _T("Широта"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(3, _T("Долгота"), LVCFMT_LEFT, 180);
	m_listCtrl.InsertColumn(4, _T("Высота"), LVCFMT_LEFT, 180);
	m_listCtrl.InsertColumn(5, _T("Дата"), LVCFMT_LEFT, 180);
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_REPORT);

	ComboHards.SetItemHeight(1,30);
	ComboHards.InsertString(0,"234324235");
	ComboHards.InsertString(1,"234324235");
	ComboHards.ShowOwnedPopups(1);
	ComboHards.SetExtendedUI(1);
	//ComboHards.setsc
	ReloadList();
	ReloadHards();


	//m_ProgressCtrl
}


BEGIN_MESSAGE_MAP(CDBaseDlg, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CDBaseDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDBaseDlg, CDialog)
END_DISPATCH_MAP()


static const IID IID_IDBaseDlg =
{ 0x466AE66E, 0x293C, 0x4C32, { 0x86, 0x5D, 0xBB, 0x82, 0xBD, 0x10, 0xBE, 0x40 } };

BEGIN_INTERFACE_MAP(CDBaseDlg, CDialog)
	INTERFACE_PART(CDBaseDlg, IID_IDBaseDlg, Dispatch)
END_INTERFACE_MAP()

void CDBaseDlg::OnClose()
{
	m_listCtrl.ShowWindow(SW_HIDE);
	CDialog::OnClose();
	m_listCtrl.DeleteAllItems ();
}

void CDBaseDlg::OnBnClickedOk()
{
	m_listCtrl.ShowWindow(SW_HIDE);
	OnOK();
}
