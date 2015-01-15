#include "StdAfx.h"
#include "MiniMapFrame.h"
BEGIN_MESSAGE_MAP(CMiniMapFrame, CFrameWnd)
	ON_WM_CREATE()

	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
CMiniMapFrame::CMiniMapFrame(CRaster* Raster,CMDIChildWnd *MapFrame,CDC *VirtDC)
{
    MainMap=Raster;
	this->MapFrame=MapFrame;
	PictInfo=MainMap->GetPictInfo();
	Rectangle.left=Rectangle.right=Rectangle.top=Rectangle.bottom=0;
	this->VirtDC=VirtDC;
}

CMiniMapFrame::~CMiniMapFrame(void)
{

}
int CMiniMapFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	   pDC=new CClientDC(this);
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	MemoryDC.CreateCompatibleDC(GetDC());
	SetWindowText(" Мини-Карта.");

		CRect r;
	GetClientRect(r);
	if(MainMap!=NULL)
	  r=MainMap->SetMiniMap(r);
	
	CBitmap dcMemoryBmp;
    dcMemoryBmp.CreateCompatibleBitmap(pDC, r.Width(), r.Height());
    MemoryDC.SelectObject(&dcMemoryBmp);

	WindRect=r;
	MoveWindow(0,0,r.Width()+3,r.Height()+3,0);
	return 0;
}
CDC* CMiniMapFrame::GetDC()
{
   return pDC;
}
void CMiniMapFrame::SetMapPt(CRaster* Raster)
{
  MainMap=Raster;
}
void CMiniMapFrame::OnPaint()
{
	CPaintDC dc(this);
	CBrush brush;
	CPen pen;
	pen.CreatePen(PS_SOLID,3,RGB(255,0,0)); 
	brush.CreateStockObject(NULL_BRUSH);
	dc.SelectObject(&brush);
	dc.SelectObject(&pen);


	CPoint pos=CPoint(0,0);
	if(MainMap!=NULL)
	  MainMap->DrawMiniMap(&MemoryDC,pos);
	dc.BitBlt(0, 0, WindRect.Width(),WindRect.Height(),
			        &MemoryDC, 0, 0, SRCCOPY);
	dc.Rectangle(Rectangle);


}

void CMiniMapFrame::OnSize(UINT nType, int cx, int cy)
{
//	MoveWindow(0,0,r.Width()+3,r.Height()+3,0);

	CFrameWnd::OnSize(nType, cx, cy);

	CRect r;
	GetWindowRect(r);
/*	if(MainMap!=NULL)
	  r=MainMap->SetMiniMap(r);
  	  r=MainMap->SetMiniMap(r);
	MoveWindow(0,0,r.Width()+3,r.Height()+3,0);
*/
	CBitmap dcMemoryBmp;
    dcMemoryBmp.CreateCompatibleBitmap(pDC, r.Width(), r.Height());
    MemoryDC.SelectObject(&dcMemoryBmp);


	WindRect=r;


	
}
void CMiniMapFrame::SetPos(CRect Map)
{
	

	CClientDC dc(this);
	CBrush brush;
	CPen pen;
	pen.CreatePen(PS_SOLID,3,RGB(255,0,0)); 
	brush.CreateStockObject(NULL_BRUSH);
	dc.SelectObject(&brush);
	dc.SelectObject(&pen);
	dc.BitBlt(0, 0, WindRect.Width(),WindRect.Height(),
			        &MemoryDC, 0, 0, SRCCOPY);
	Rectangle.left=WindRect.Width()*Map.left/(PictInfo.width);
	Rectangle.right=WindRect.Width()*Map.right/(PictInfo.width);
	Rectangle.top=WindRect.Height()-WindRect.Height()*Map.top/(PictInfo.height);
	Rectangle.bottom=WindRect.Height()-WindRect.Height()*Map.bottom/(PictInfo.height);
    
	dc.Rectangle(Rectangle);
	MainRect=Rectangle;
    
}
 

void CMiniMapFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect RectDraw;
	RectDraw.left=point.x*(PictInfo.width)/WindRect.Width();

	RectDraw.right=MainMap->GetOutputRect().Width()+RectDraw.left;

	RectDraw.top=PictInfo.height-point.y*(PictInfo.height)/WindRect.Height();
	RectDraw.bottom=PictInfo.height+MainMap->GetOutputRect().Height()-point.y*(PictInfo.height)/WindRect.Height();

    //MapFrame-

	//MainMap->SetOutputRect(RectDraw);
	MainMap->Draw(CRect(RectDraw.left,RectDraw.top,
			            RectDraw.right,RectDraw.bottom),VirtDC,CPoint(0,0));
	
	CClientDC dc(MapFrame);
	dc.BitBlt(0,0,MainMap->GetOutputRect().Width(),MainMap->GetOutputRect().Height(),VirtDC,0,0,SRCCOPY);
	SetPos(RectDraw);
	CFrameWnd::OnLButtonDown(nFlags, point);
}
