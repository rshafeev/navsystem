#pragma once


// CViewDocFrame frame

class CViewDocFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CViewDocFrame)
protected:

	virtual ~CViewDocFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CListBox *ListBox;
	void SetListBoxPos(int FrameIndex);
	
	CViewDocFrame();           // protected constructor used by dynamic creation
    
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSessionChange();
};


