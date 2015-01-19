// MainFrm.h : interface of the CMainFrame class
//


#pragma once
#include <vector>

//#include "GPS.h"
#include "GPSDlgList.h"
#include "OpenResDlg.h"
#include "GlLoadHgtDlg.h"
#include "GlLoadRawDlg.h"
#include "GlLandTexDlg.h"

using std::vector;
class CMainFrame : public CMDIFrameWnd
{
public:
struct TMenu
{
  bool GPSConnect;	
  bool HardView;
  bool DocumentView;// окно "Документы" меню "Вид"
  bool GPSAddEnable; 
  
  bool GlGrid;     // показать/скрыть сетку
  bool Tool3D;       // показать/скрыть панель 3d карты
  short RaterCur;     //тип курсора растровой карты(0-обычный; 1-при нажатии мыши появляется окно добавления координаты)
}Menu;	
protected: 
	DECLARE_DYNAMIC(CMainFrame)
   
//**************************************************   
private:
    CGPSDlgList *GPSDlgList;
   int screenBitSize;

protected:

public:
	CGlLoadHgtDlg GlLoadHgtDlg;
    CGlLoadRawDlg GlLoadRawDlg;
    CGlLandTexDlg GlLandTexDlg;

	CStatusBar  m_StatusBar;

//**************************************************   
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    CMainFrame();
	virtual ~CMainFrame();
	
#ifdef _DEBUG
#endif
// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnMove(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMenunew();
	afx_msg void OnMenuExit();
	afx_msg void OnChildActivate();
	afx_msg void OnGpsConnect();
	afx_msg void OnUpdateGpsConnect(CCmdUI *pCmdUI);
	afx_msg void OnGPSAdd();
	afx_msg void OnUpdateGpsAdd(CCmdUI *pCmdUI);
	afx_msg void OnGpsDelete();
	afx_msg void OnUpdateGpsDel(CCmdUI *pCmdUI);
	afx_msg void OnGpsSettings();
	afx_msg void OnDocView();
	afx_msg void OnUpdateDocView(CCmdUI *pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGpsList();
	afx_msg void OnHardView();
	afx_msg void OnUpdateHardView(CCmdUI *pCmdUI);
	afx_msg void OnMenuOpenRes();
	afx_msg void OnMenuOpenProject();
	afx_msg void OnMenuRastrCurMove();
	afx_msg void OnMenuRastrCurPriv();
	afx_msg void OnUpdateMenuRastrCurPriv(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuRastrCurMove(CCmdUI *pCmdUI);
	afx_msg void OnMenuLink();
	afx_msg void OnMenuSave();
	afx_msg void OnGlGrid();
	afx_msg void OnUpdateGlGrid(CCmdUI *pCmdUI);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenu3DNew();
	afx_msg void OnMenuGlReg();
	afx_msg void OnMenu3DMapTool();
	afx_msg void OnMenu3DToolMap(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnMenuLoadLandHgt();
	afx_msg void OnMenuGlLoadRaw();
	afx_msg void OnMenuGlTexLand();
	afx_msg void OnMenuMiniMap();
	afx_msg void OnUpdateMinMap(CCmdUI *pCmdUI);
};


//*******структура файла <rsmap>:
/*
1. <тип документа>:  0 - блочная карта
                  1 - растровая
				  2 - 3д
2.0 если <тип документа> равно 0:
..
				  
2.1 если <тип документа> равно 1:
<расширение> - "bmp" "jpg" ...
*/