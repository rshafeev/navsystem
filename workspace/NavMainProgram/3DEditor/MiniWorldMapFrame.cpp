#include "StdAfx.h"
#include "MiniWorldMapFrame.h"
BEGIN_MESSAGE_MAP(CMiniWorldMapFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

CMiniWorldMapFrame::CMiniWorldMapFrame(void)
{
}
CMiniWorldMapFrame::CMiniWorldMapFrame(CString Folder)
{
	this->PathName=Folder;
}
CMiniWorldMapFrame::~CMiniWorldMapFrame(void)
{
}
BOOL CMiniWorldMapFrame::PreCreateWindow(CREATESTRUCT &cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}
int CMiniWorldMapFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	pDC=new CClientDC(this);
	dcMemory.CreateCompatibleDC(pDC);
	BlocksObj=new CBlockRaster(PathName,RES_PATH);
	
	CRect WndRect;
	GetClientRect(WndRect);
	CBitmap dcMemoryBmp;
    dcMemoryBmp.CreateCompatibleBitmap(pDC, WndRect.Width(), WndRect.Height());
    dcMemory.SelectObject(&dcMemoryBmp);
	
	
	BlocksObj->SetWindRect(WndRect);    
	BlocksObj->ResizeMemory(CPoint(BlocksObj->GetWindRect().Width()/PICTSIZE*2+2,
		                           BlocksObj->GetWindRect().Height()/PICTSIZE*2+2));
    BlocksObj->SetSdvig(CPoint(0,0));
	//BlocksObj->SetMassSize(CPoint(3,3));
   
	BlocksObj->PushMatrix();

	return 0;
}


void CMiniWorldMapFrame::OnPaint()
{
	CPaintDC dc(this);

		BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),BlocksObj->GetSdvig());
		dc.BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			        &dcMemory, 0, 0, SRCCOPY);
}

void CMiniWorldMapFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnMouseMove(nFlags, point);
}

void CMiniWorldMapFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CMiniWorldMapFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFrameWnd::OnLButtonUp(nFlags, point);
}
void CMiniWorldMapFrame::SetLevel(int LevelInWorldMap,CPoint BlockInWorldMap,CPoint sdvig)
{
	if(LevelInWorldMap<3)
	{
		LevelInWorldMap=1;
	    BlockInWorldMap.x=0;
	    BlockInWorldMap.y=0;
	/*		BlocksObj->SetLevel(LevelInWorldMap);
	BlocksObj->PushMatrix();
	BlocksObj->DrawMatrix(pDC,BlocksObj->GetMassSize(),CPoint(0,0));*/
	}
   else
   {
	   LevelInWorldMap-=2;
/*	   BlockInWorldMap.x/=4;
	   BlockInWorldMap.y/=4;
	   sdvig.x=sdvig.x/4;
	   sdvig.y=sdvig.y/4;*/
   
	}
 	BlocksObj->SetSdvig(sdvig);
	BlocksObj->SetBlock(BlockInWorldMap);
	BlocksObj->SetLevel(LevelInWorldMap);
	
	BlocksObj->PushMatrix();
	BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),BlocksObj->GetSdvig());
	  pDC->BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			  &dcMemory, 0, 0, SRCCOPY);
	

}
//***************************************************************************************************
void CMiniWorldMapFrame::SetMap(CBlockRaster *MainMap)
{
	const int LevelDelta=2;
	CRect MDrawRect=MainMap->GetDrawRect();
	CPoint MCentralPoint;
	CPoint MCBlockPoint=MainMap->GetLeftTopBlock();
	CPoint MSdvig=MainMap->GetSdvig();

	MCBlockPoint.x*=256;
	MCBlockPoint.y*=256;



    MCentralPoint.x=(MDrawRect.right+MDrawRect.left)/2+(MCBlockPoint.x-MDrawRect.left);
	MCentralPoint.y=(MDrawRect.bottom+MDrawRect.top)/2+(MCBlockPoint.y-MDrawRect.top);

    MCentralPoint.x/=(int)STEPENTWO[LevelDelta];
	MCentralPoint.y/=(int)STEPENTWO[LevelDelta];
	
	BlocksObj->SetBlock(CPoint(MCentralPoint.x/PICTSIZE,MCentralPoint.y/PICTSIZE));
	BlocksObj->SetLevel(MainMap->GetLevel()-LevelDelta);
	MCentralPoint.x=MSdvig.x/int(STEPENTWO[int(LevelDelta)]);
	MCentralPoint.y=MSdvig.x/int(STEPENTWO[int(LevelDelta)]);



	BlocksObj->PushMatrix();
	BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),MCentralPoint);
	//BlocksObj->DrawMatrix(&dcMemory,BlocksObj->GetMassSize(),CPoint(0,0));
	pDC->BitBlt(0, 0, BlocksObj->GetWindRect().Width(), BlocksObj->GetWindRect().Height(),
			  &dcMemory, 0, 0, SRCCOPY);
    
	CBrush brush;
	CPen pen;
	pen.CreatePen(PS_SOLID,3,RGB(255,0,0)); 
	brush.CreateStockObject(NULL_BRUSH);
	pDC->SelectObject(&brush);
	pDC->SelectObject(&pen);
	//pDC->Rectangle(10,10,100,100);
}
	