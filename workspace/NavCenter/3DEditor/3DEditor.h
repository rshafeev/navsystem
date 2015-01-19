// 3DEditor.h : main header file for the 3DEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "MainFrm.h"
#include "Resource.h"
#include "GPSDlgAdd.h"
#include "GPSDlgDel.h"
#include "GPS.h"
#include "FrameContainer.h" 
#include "HardToolFrame.h"   
#include "ViewDocFrame.h"  
#include "GlToolFrame.h"  
// CMy3DEditorApp:
// See 3DEditor.cpp for the implementation of this class
//
#include "adoclasses.h"
#include "adodefs.h"

class CMy3DEditorApp : public CWinApp
{
public:
	CMy3DEditorApp();

// Overrides
public:
   CADOConnection m_GlConnection; 
	virtual BOOL InitInstance();

// Implementation
   //	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern  CMy3DEditorApp theApp;
extern  CMainFrame* MainForm;
extern  CGPSDlgAdd* GPSDlgAdd;
extern	CGPSDlgDel* GPSDlgDel;
extern	CFrameContainer *FrameContainer;
extern	CHardToolFrame *HardToolFrame;
extern	CViewDocFrame  *ViewDocFrame;
extern	CGPS *GPS;
extern  CGlToolFrame  *GlToolFrame;