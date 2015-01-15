// MainFrm.h : interface of the CMainFrame class
//
//#include "ServerWork.h"
#include "ServerThread.h"
#include "DBaseDlg.h"
#include "AboutDlg.h"
#pragma once

class CMainFrame : public CFrameWnd
{
	struct TMenu
	{
      bool ListView;
	  bool Started;
	}Menu;
private:
   bool IsTray;
public:
	CMainFrame();
	 
protected: 
	CListBox *ListBox;
	CDBaseDlg *DBaseDlg;	
	CServerThread *pThread;

 
    DECLARE_DYNAMIC(CMainFrame)
   
// Attributes
public:
// Operations
public:

/*	void writeDB(TFieldValue &Add);
    void readDB(TFieldValue *Add);*/
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
#endif

protected:  // control bar embedded members


// Generated message map functions
protected:


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConnect1();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMenuBaseView();
	afx_msg void OnAppAbout();
	afx_msg void OnViewList();
	afx_msg void OnUpdateViewList(CCmdUI *pCmdUI);
	afx_msg void OnBDClear();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnAppExit();
	afx_msg void OnMenuExit();
	afx_msg void OnUpdateAppExit(CCmdUI *pCmdUI);
};


