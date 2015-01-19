#include "afx.h"
#include "BlockRaster.h"
#include "MiniWorldMapFrame.h"
#include "FrameContainer.h" 
#include "InetRaster.h"
#pragma once

class CWorldMapFrame :public CFrameContainer
{

CString FolderCache;
CString ServerUrl;

DECLARE_DYNAMIC(CWorldMapFrame)
int LoadFileFromInet(CString FNameWithoutExp);
private:
  СInetRaster *InetRaster;	
  CMutex *Mutex;
  CMutex CriticalSection;
  vector<TLoadedFiles> LoadContainer;
  int NoLoadCount;
  bool TerminatedThread;
 bool FlagDraw;

   struct TBlockMouse
   {
      bool flag;//нажата ли кнопка мыши
      CPoint delta;
      CPoint MCord;
    } Mouse;
	CSliderCtrl *Slider;
    CString PathName;    //путь к папке, в которой хранятся блоки мировой карты в формате jpg
	CString FileName;
	CDC dcMemory;
    CClientDC *pDC;
    CBlockRaster *BlocksObj;
	
	CMiniWorldMapFrame* MiniMap;
	CPoint Coord;
    void DrawAbonents(CDC *pDC); /*выводит абонентов на карту(DrawHards - общая ф-ция,
	                               а DrawAbonents - указывает на какой именно контекст выводить)*/
    void SendToThreadNotLoadPict(vector<TLoadedFiles> &NotLoaded); 

public:
	
	bool redrawHards;
	virtual void DrawHards();
	virtual void FindAbonent(TGPSPoint *Position);
	virtual void Save(CString FName="");
	virtual void ShowMiniMap(bool visiable);
	virtual void     Repaint()
	{
		if(FlagDraw==true)return;
			CClientDC dc(this);
	   BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),BlocksObj->GetSdvig());
	   dc.BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			        &dcMemory, 0, 0, SRCCOPY);
	   DrawAbonents(&dc);
	}
	CWorldMapFrame(CString PathName);
	CWorldMapFrame(void);
	virtual ~CWorldMapFrame(void);

	DECLARE_MESSAGE_MAP()
	 afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	 afx_msg void OnHScroll(UINT nSBCode,UINT nPos,CScrollBar *pScrollBar);
	 afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	 afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	 afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	 afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	 afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	 afx_msg void OnSize(UINT nType, int cx, int cy);
	 afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	 afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	 afx_msg void OnPaint();
	 afx_msg void OnClose();
	 afx_msg void OnSetFocus(CWnd* pOldWnd);
};
