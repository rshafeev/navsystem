// NavClientDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ClientThread.h"


// CNavClientDlg dialog
class CNavClientDlg : public CDialog
{
// Construction
public:
	CNavClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_NAVCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnConnect();


//**********************
HANDLE hMainThread;         // Дескриптор основного потока
HANDLE hGetMes;             // Дескриптор функции, определяющей тип сообщения
WSADATA WSAData;           // Структура конфигурации DLL
SOCKET WinSocket;          // Наш сокет
sockaddr_in SockStruct;    // Структура содержит информацию о сокете

int Bind;                  // Хранит результат связывания локального сетевого адреса с сокетом
int Listen;                // Хранит результат прослушивания сокета
int Connect;               // Переменная хранит результат подключения клиента к серверу
int Send;                  // Переменная хранит результат отправки сообщения
int Recv;                  // Переменная хранит результат приема сообщения

CString List ;
CString ServIP;
bool connected;
int Port;

	bool ConnectByServer();
CClientThread *ClientThread;
//**********************
	CIPAddressCtrl m_Adress;
	CButton m_Disconnect;
	CButton m_Connect;
	CEdit m_Port;
	CString m_TextPort;
	afx_msg void OnBnDisconnected();
	afx_msg void OnBnClickedCheckThread();
	CButton m_ThreadCheck;
	afx_msg void OnClose();
	CEdit m_idCtrl;
};
