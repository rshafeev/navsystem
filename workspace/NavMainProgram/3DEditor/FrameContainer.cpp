#include "StdAfx.h"
#include "FrameContainer.h"
#include "3DEditor.h"

CFrameContainer *FrameContainer;
CFrameContainer::CFrameContainer(void)
{
	FlagMiniMap=false;
	activeIndex=0;
	int i;
	for(i=0;i<MAXCOL_WIND;i++)
		FramesMass[i]=NULL;
	SelectedAbonent.ID="";
}

CFrameContainer::~CFrameContainer(void)
{
}
//======================================================================================================================
int CFrameContainer::GetWindowType()
{
	return WindowType;
}
//======================================================================================================================
CFrameContainer* CFrameContainer::Find3DWindow()
{
	unsigned int i;
	for(i=0;i<Container.size();i++)
		if(Container[i].FramePtr->GetWindowType()==2)
			return Container[i].FramePtr;
	return NULL;
} 
//======================================================================================================================
void CFrameContainer::DestroyContainer()
{
	DestroyFlag=true;
	Container.clear();
	FreeIndex.clear();
    delete *this;

}
//======================================================================================================================
void CFrameContainer::DrawPoint(CDC *pDC, CPoint position, int ColorType)
{
	CPen Pen;
	int i;
	Pen.CreateStockObject(NULL_PEN);
	
    CBrush b; 
    pDC->SelectObject(&Pen);
    if(ColorType==0)
	{
	   for(i=0;i<15;i++)
	   {
		b.CreateSolidBrush(RGB(17*i,0,255-17*i)); 
	   pDC->SelectObject(&b);
	    pDC->Ellipse(position.x+i,position.y+i,position.x+30-i,position.y+30-i);
    	b.DeleteObject();
       }
	}
	else
	if(ColorType==1)
	{
	   for(i=0;i<15;i++)
	   {
		b.CreateSolidBrush(RGB(255-7*i,0,9*i)); 
	    pDC->SelectObject(&b);
	    pDC->Ellipse(position.x+i,position.y+i,position.x+30-i,position.y+30-i);
    	b.DeleteObject();
       }       
	}
}

//======================================================================================================================
CString CFrameContainer::GetWindText(int FrameIndex)
{
	CString g="";
	if(FramesMass[FrameIndex]!=NULL)
       FramesMass[FrameIndex]->GetWindowTextA(g);
	return g;
}
//======================================================================================================================
int CFrameContainer::GetIndexFromFrameIndex(int FrameIndex)
{
   int i;
   for(i=0;i<(int)Container.size();i++)
	   if(Container[i].FrameIndex==FrameIndex)
		   return i;
   return -1;
}
//======================================================================================================================
int CFrameContainer::GetFrameIndex(int Index)
{
	if(Index<Container.size())
	return Container[Index].FrameIndex;
	else
	return -1;
}
//======================================================================================================================
int CFrameContainer::GetWindowCount()
{
	return Container.size();
}
//======================================================================================================================
void CFrameContainer::InitContainer()
{
	int i;

	FreeIndex.resize(MAXCOL_WIND);
    for(i=0;i<MAXCOL_WIND;i++)
        FreeIndex[i]=i;
	
}
//======================================================================================================================
bool CFrameContainer::isDestroy()
{
   return DestroyFlag;
}
//======================================================================================================================
void CFrameContainer::AddToContainer(CFrameContainer *AddFrame)
{
	CString TitleText="",buf;
	TContainer d;
	d.FramePtr=AddFrame;
	d.FrameIndex=FreeIndex[0];
	AddFrame->UnicIndex=FreeIndex[0];
	FramesMass[FreeIndex[0]]=AddFrame;
	AddFrame->GetWindowTextA(buf);
	TitleText.Format("%s (%d)",buf,FreeIndex[0]);
    AddFrame->SetWindowTextA(TitleText);


	FreeIndex.erase(FreeIndex.begin());
	Container.push_back(d);
	if(MainForm->Menu.DocumentView==true)
	  InsertToListBox(ViewDocFrame->ListBox);

	
}
//======================================================================================================================
void CFrameContainer::DelFromContainer(int FrameIndex)
{
	unsigned int i;
	
	for(i=0;i<Container.size();i++)
		if(Container[i].FrameIndex==FrameIndex)
		{
		   if(Container.size()<=i) break; 
		   Container.erase(Container.begin()+i);
		   FreeIndex.insert(FreeIndex.begin(),FrameIndex);
		   GPS->UpdateHardwareList(FrameIndex);
		}
	if(MainForm->Menu.DocumentView==true)
		InsertToListBox(ViewDocFrame->ListBox);
		
	FramesMass[FrameIndex]=NULL;
		
}

//======================================================================================================================
CFrameContainer* CFrameContainer::GetActiveFrame()
{
	
	return FramesMass[activeIndex];
}
int CFrameContainer::GetActiveIndexFrame()
{
   return activeIndex;
}
//======================================================================================================================
int CFrameContainer::InsertToListBox(CListBox* ListBox)
{
	int i;
	int max=0;
	CString g;

	ListBox->ResetContent();
	for(i=0;i<(int)Container.size();i++)
	{

		Container[i].FramePtr->GetWindowText(g);
		ListBox->AddString((LPCSTR)g);
		if(max<g.GetLength())max=g.GetLength();
	}
	return max;
}
//======================================================================================================================
CFrameContainer::TContainer CFrameContainer::GetContainerElement(int index)
{ //получить элемент массива Container

	if(index<Container.size())
	  return Container[index];
	else
	{
	TContainer result;
	result.FrameIndex=-1;
	result.FramePtr=NULL;
	return result;
	}

} 
//======================================================================================================================