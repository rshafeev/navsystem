// NavClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NavClient.h"
#include "NavClientDlg.h"
#include "afx.h"
#include "winsock2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
//===========================================================================================================================
static CString GetStrFromEdit(CEdit *Edit)
{
//функция возвращает строку, введенная в CEdit
  CString result="";
int i, nLineCount = Edit->GetLineCount();
CString strText, strLine;
char *buf=new char[3333];
for (i=0; i < nLineCount; i++)
{
   int len = Edit->LineLength(Edit->LineIndex(i));
   if(len==0)continue;
   Edit->GetLine(i, strText.GetBuffer(len), len+3);
   //Edit->GetLine(i,buf);

   strText.ReleaseBuffer(len);
   result=result+strText;
}
if(result=="") return "";
  return result;
}
//===========================================================================================================================
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CNavClientDlg dialog




CNavClientDlg::CNavClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNavClientDlg::IDD, pParent)
	, m_TextPort(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNavClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_Adress);
	DDX_Control(pDX, IDC_BUTTON3, m_Disconnect);
	DDX_Control(pDX, IDC_BUTTON2, m_Connect);
	DDX_Control(pDX, IDC_EDIT1, m_Port);
	m_Adress.SetAddress(127,0,0,1);
	//m_Adress.SetAddress(77,122,35,49);

	m_TextPort="5400";
	DDX_Text(pDX, IDC_EDIT1, m_TextPort);


	DDX_Control(pDX, IDC_CHECK1, m_ThreadCheck);
	DDX_Control(pDX, IDC_EDIT2, m_idCtrl);
	m_idCtrl.SetWindowTextA("234324235");
}

BEGIN_MESSAGE_MAP(CNavClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CNavClientDlg::OnBnConnect)
	ON_BN_CLICKED(IDC_BUTTON3, &CNavClientDlg::OnBnDisconnected)
	ON_BN_CLICKED(IDC_CHECK1, &CNavClientDlg::OnBnClickedCheckThread)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CNavClientDlg message handlers

BOOL CNavClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	
	
	//Ctrl.ShowWindow(0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNavClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNavClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNavClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNavClientDlg::OnBnConnect()
{
	BYTE b1,b2,b3,b4;
	m_Adress.GetAddress(b1,b2,b3,b4);
	Port=atoi((LPCSTR)m_TextPort);
	ServIP.Format("%d.%d.%d.%d",(int)b1,(int)b2,(int)b3,(int)b4);
	if(ConnectByServer()==true)
	{
		m_Disconnect.EnableWindow(1);
		m_Connect.EnableWindow(0);
	ClientThread=new CClientThread(WinSocket);
	ClientThread->SetIMEI(GetStrFromEdit(&m_idCtrl));
	ClientThread->CreateThread();
	}
	else
	{
       m_Disconnect.EnableWindow(0);
	   m_Connect.EnableWindow(1);
	}

}
//***************************************************************************************************************
bool CNavClientDlg::ConnectByServer()
{
    // Конфигурация DLL версии 2.1
    if (WSAStartup(MAKEWORD(2,1), &WSAData) != 0)
	{
		//MessageBox("ChoosePixelFormat failed");
		MessageBox("ОШИБКА: Невозможно инициализировать библиотеку сокетов!",0);
		return false;
	}
// Создаем сокет
	WinSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 if (WinSocket == SOCKET_ERROR)
 {
		MessageBox("ОШИБКА: Невозможно создать сокет!",0);
		return false;
 }
   // Заполняем поля структуры для сокета
 SockStruct.sin_family = AF_INET;
 SockStruct.sin_port = Port;
 SockStruct.sin_addr.s_addr = inet_addr((LPCSTR)ServIP);
 
// ListBox->AddString("Подключение...");
         // Подключаемся к сокету
 Connect = connect(WinSocket, (sockaddr*)&SockStruct, sizeof(SockStruct));
 if (Connect == SOCKET_ERROR)
 {
        MessageBox("ОШИБКА: Невозможно подключиться к серверу!",0);
        return false;
 }
 connected=true;
 return true;
}
//***************************************************************************************************************
void CNavClientDlg::OnBnDisconnected()
{
	 ClientThread->SuspendThread();
	 delete ClientThread;
	 m_ThreadCheck.SetCheck(0);
 
   closesocket(WinSocket);
	   WinSocket=NULL;

      m_Disconnect.EnableWindow(0);
	   m_Connect.EnableWindow(1);


}
static UINT Threared(LPVOID pParam)
{
	int x;
  return 0;
}
void CNavClientDlg::OnBnClickedCheckThread()
{
	
	/*if(m_ThreadCheck.GetCheck()==1)
	{
		for(int i=0;i<1;i++)
		{
			ClientThread=new CClientThread(WinSocket); ClientThread->CreateThread();
		}
	}  
	else
	if(m_ThreadCheck.GetCheck()==0)
	{ 
     ClientThread->SuspendThread();
	 delete ClientThread;

	}*/
}

void CNavClientDlg::OnClose()
{
	// ++  ClientThread->Terminate=true;
	   closesocket(WinSocket);
	   WinSocket=NULL;
	 //  WaitForSingleObject(*ClientThread,200);

	CDialog::OnClose();
}
