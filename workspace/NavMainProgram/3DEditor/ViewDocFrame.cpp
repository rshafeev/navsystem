// ViewDocFrame.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"

const  int IDC_LIST=1000;

// CViewDocFrame
CViewDocFrame  *ViewDocFrame;
IMPLEMENT_DYNCREATE(CViewDocFrame, CFrameWnd)

CViewDocFrame::CViewDocFrame()
{

}

CViewDocFrame::~CViewDocFrame()
{
}


BEGIN_MESSAGE_MAP(CViewDocFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_LIST, OnSessionChange)
END_MESSAGE_MAP()


// CViewDocFrame message handlers
void CViewDocFrame::SetListBoxPos(int FrameIndex)
{
	int ind=FrameContainer->GetIndexFromFrameIndex(FrameIndex);
	ListBox->SetCurSel(ind);
}
void CViewDocFrame::OnSessionChange()
{
	
	int y=ListBox->GetCurSel();
	if(y>=0)
	{
		FrameContainer->GetContainerElement(y).FramePtr->SetFocus();
		//FrameContainer->GetContainerElement(y).FramePtr->ShowWindow(SW_RESTORE);
		
	}
}

int CViewDocFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	ListBox=new CListBox;
	ListBox->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|
		LBS_WANTKEYBOARDINPUT|LBS_DISABLENOSCROLL|WS_VSCROLL|LBS_NOTIFY|CBS_DROPDOWNLIST, 
		                 CRect(0,0,200,200), this, IDC_LIST);
    
	SetWindowText("Документы");
	MainForm->Menu.DocumentView=true;
	FrameContainer->InsertToListBox(ListBox);

	
	return 0;
}

void CViewDocFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	MainForm->Menu.DocumentView=false;
}

void CViewDocFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

    CRect r;
	GetClientRect(r);
	ListBox->MoveWindow(0,0,r.Width(),r.Height(),1);
}
