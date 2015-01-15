#pragma once
#include "afx.h"
#include "BlockRaster.h"
class CMiniWorldMapFrame:  public/* CWorldMapFrame*/CFrameWnd
{
private:

    struct TBlockMouse
   {
      bool flag;//нажата ли кнопка мыши
      CPoint delta;
      CPoint MCord;
    } Mouse;
    CString PathName;    //путь к папке, в которой хранятся блоки мировой карты в формате jpg
	CDC dcMemory;
    CClientDC *pDC;

	CMiniWorldMapFrame* MiniMap;
	CPoint Coord;
    
private:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
    void SetLevel(int LevelInWorldMap,CPoint BlockInWorldMap,CPoint sdvig);//передает LevelInWorldMap-
	                                                          //уровень дет-ции на главной карты
	void SetMap(CBlockRaster *MainMap);
    CBlockRaster *BlocksObj;
	CMiniWorldMapFrame(void);
	CMiniWorldMapFrame(CString Folder);
	~CMiniWorldMapFrame(void);

		DECLARE_MESSAGE_MAP()
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

		afx_msg void OnPaint();
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
