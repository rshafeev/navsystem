#pragma once
#include "Raster.h"
#include "MiniMapFrame.h"
#include "FrameContainer.h" 
#include "Linkage.h"
class CMapFrame :public CFrameContainer
{
	struct TBlockMouse
   {
      bool flag;//нажата ли кнопка мыши
      CPoint delta;
      CPoint MCord;

	} Mouse;
		DECLARE_DYNAMIC(CMapFrame)
   
	CRaster *Raster;
	CRect Wrect;
	CLinkage Linkage;
	CString FileName;
	CClientDC  *pDC;
	
    int i;
	int RasterType;
	CRect RectWnd;
	void DrawAbonents(CDC *pDC);
	CMiniMapFrame *MiniMap;	
public:
CDC dcMemory;

public:


    void ReadInfoFile(CString FileName);
  
	CMapFrame(void);
	CMapFrame(CString File);

	virtual ~CMapFrame(void);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void Save(CString FName="");
	virtual CLinkage* GetLinkageObj(){return &Linkage;}
	virtual void DrawHards();
	virtual void FindAbonent(TGPSPoint *Position);
    virtual void ShowMiniMap(bool visiable);                                 //показать/скрыть мини-карту

	DECLARE_MESSAGE_MAP()
	 afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
     afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	 afx_msg void OnSize(UINT nType, int cx, int cy);
	 afx_msg void OnPaint();
	 afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	 afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	 afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	 afx_msg void OnClose();
	 afx_msg void OnSetFocus(CWnd* pOldWnd);
};
