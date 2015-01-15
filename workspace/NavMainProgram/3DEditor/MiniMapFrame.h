#pragma once
#include "afx.h"
#include "Raster.h"


class CMiniMapFrame: public CFrameWnd
{
private:
	CClientDC *pDC;
	CRaster *MainMap;
	CDC MemoryDC;
	CRect WindRect;
	CRect MainRect;
	CDC *MainDC;//контекст главного окна, т.е. CMapFrame
	CMDIChildWnd *MapFrame;
	CRect Rectangle;
	CDC *VirtDC;
public:
	CMiniMapFrame(CRaster* Raster,CMDIChildWnd *MapFrame,CDC *VirtDC);
	~CMiniMapFrame(void);
    
	CDC* GetDC();
    void SetMapPt(CRaster *Raster);
	void SetPos(CRect Map);
	
	CRaster::TPictInfo PictInfo;
	
	DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnPaint();
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
