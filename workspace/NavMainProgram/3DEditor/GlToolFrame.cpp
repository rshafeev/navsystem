// GlToolFrame.cpp : implementation file
//

#include "stdafx.h"
#include "3DEditor.h"
#include "memory.h"
#include "GlToolFrame.h"
const int IDC_CATEGORY   = 357;
const int IDC_OBJCOMBO   = 358;
const int IDC_OBJLIST    = 359;
const int IDB_BUTTONADD  = 360;
const int IDB_BUTTONFIX  = 361;
const int IDP_POLZSCALE  = 362;
// CGlToolFrame

IMPLEMENT_DYNCREATE(CGlToolFrame, CFrameWnd)
CGlToolFrame  *GlToolFrame;
BEGIN_MESSAGE_MAP(CGlToolFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_CBN_SELENDOK  (IDC_CATEGORY,&OnCategoryBoxChange)
	ON_CBN_SELENDOK  (IDC_OBJCOMBO,&OnObjComboBoxChange)
	ON_LBN_SELCHANGE (IDC_OBJLIST, &OnObjListChange)
	ON_BN_CLICKED    (IDB_BUTTONADD,&OnBtnAddClick)
	ON_BN_CLICKED    (IDB_BUTTONFIX,&OnBtnFixClick)

	ON_WM_HSCROLL    (IDP_POLZSCALE,OnHScroll)

	ON_NOTIFY(TVN_SELCHANGED, IDC_OBJLIST, &OnObjListCLick)

END_MESSAGE_MAP()
CGlToolFrame::CGlToolFrame()
{
   ComboCategory = new CComboBox();
   ComboObj      = new CComboBox();
   ListObj       = new CTreeCtrl();
   ButtonAdd     = new CButton();
   ButtonFix     = new CButton();
   Slider        = new CSliderCtrl();
   SelectInd.x=-1;
   SelectInd.y=-1;
}

CGlToolFrame::~CGlToolFrame()
{
	  delete ButtonAdd;      
      delete ButtonFix;     
	  delete ComboCategory; 
	  delete ComboObj;      
	  delete ListObj;
      delete Slider;
}
//==============================================================================================================================
CPoint CGlToolFrame::AddToObjList(CString caption)
{
 	HTREEITEM pRoot = ListObj->GetRootItem();

	int i,j;
	int ind=0;
	HTREEITEM pNode=pRoot;
	TV_INSERTSTRUCT tvstruct;
    bool f=false;
	for(i=0;i<GetLevelCount(pRoot);i++)
	{
		if(ListObj->GetItemText(pNode)==caption)
		{
          f=true;
		  break;
		}
		pNode=ListObj->GetNextItem(pNode,TVGN_NEXT);
	}
	if(f==false)
	{
        
       
        ListObj->ModifyStyle(LVS_TYPEMASK,TVS_LINESATROOT | TVS_HASLINES |
        TVS_HASBUTTONS | TVS_EDITLABELS );
        tvstruct.hParent = 0;
        tvstruct.item.mask = TVIF_TEXT ;
        tvstruct.item.pszText = (LPSTR)caption.GetBuffer();
        pNode=ListObj->InsertItem(&tvstruct);

		tvstruct.hParent = pNode;
        tvstruct.item.mask = TVIF_TEXT ;
        tvstruct.item.pszText = "1";
        ListObj->InsertItem(&tvstruct);
	}
	else
	{
        HTREEITEM pParent=pNode;
		pNode=ListObj->GetChildItem(pNode);
		
		while(pNode!=NULL)
	    {
		  CString h=ListObj->GetItemText(pNode);
		  pNode=ListObj->GetNextItem(pNode,TVGN_NEXT);
		  h=ListObj->GetItemText(pNode);
		  ind++;
	    }
		ind ++;
		CString g;
		g.Format("%d",ind);
		tvstruct.hParent = pParent;
        tvstruct.item.mask = TVIF_TEXT ;
		tvstruct.item.pszText = g.GetBuffer();
        ListObj->InsertItem(&tvstruct);
		ind--;
	}
  return CPoint(i,ind);
}
//==============================================================================================================================
int CGlToolFrame::GetLevelCount(HTREEITEM parItem)
{
   int i=0;
   while(parItem!=NULL)
   { 
       i++;
	  parItem=ListObj->GetNextItem(parItem,TVGN_NEXT);
   }
   return i;
}
//==============================================================================================================================
CString CGlToolFrame::TreeGetPathName(HTREEITEM parItem/*,CTreeCtrl m_TreeCtrl*/)
{
    CString rootItemName = ListObj->GetItemText( ListObj->GetRootItem());
    CString m_fullName="";
    while(ListObj->GetItemText(parItem)!=rootItemName)
    {
        m_fullName = ListObj->GetItemText(parItem) +"\\" + m_fullName  ;
        if(!(parItem =ListObj->GetParentItem (parItem)))
            return m_fullName;
    }
    if(m_fullName.Right(1)!="\\" && m_fullName!="")
                m_fullName+="\\";
    return m_fullName;
}
//==============================================================================================================================
CPoint CGlToolFrame::GetItemIndex(HTREEITEM parItem)
{
  CPoint ind(-1,-1);
 
  HTREEITEM Item= ListObj->GetParentItem(parItem);
  if(Item==NULL) return ind;

  CString ChildSearchStr =ListObj->GetItemText(parItem);
	int i=0,j=0;
	Item=ListObj->GetRootItem();
	CString g;
    while(ListObj->GetItemText(Item)!=ListObj->GetItemText(ListObj->GetParentItem(parItem)))
	{
     i++;
	 Item=ListObj->GetNextItem(Item,TVGN_NEXT);
	}
    Item=ListObj->GetParentItem(parItem);
	Item=ListObj->GetNextItem(Item,TVGN_CHILD);
	while(ListObj->GetItemText(Item)!=ListObj->GetItemText(parItem))
	{
       j++;
       Item=ListObj->GetNextItem(Item,TVGN_NEXT);
	}
ind.SetPoint(i,j);
  return ind;
}
//==============================================================================================================================
void CGlToolFrame::OnObjListCLick(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM tree=ListObj->GetSelectedItem();
   
	CPoint p=GetItemIndex(tree);
/*	if(p.x!=-1&&p.y!=-1)
	{
		CString str;
		str.Format("%d %d",p.x,p.y);
       AfxMessageBox((LPCSTR)str);
	}
*/
	if(p.x!=-1&&p.y!=-1)
	{
	  FrameContainer->GetActiveFrame()->SelectObj(GetItemIndex(tree));
	  FrameContainer->GetActiveFrame()->RenderScene();
	}
}
//==============================================================================================================================
void CGlToolFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	double scale;
    HTREEITEM tree=ListObj->GetSelectedItem();
	CPoint ind=GetItemIndex(tree);
    
	if(ind.x==-1||ind.y==-1) return;
	scale= ((double)Slider->GetPos())/150.0;
	FrameContainer->GetActiveFrame()->ChangeObjScale(ind,scale,scale,scale);
	FrameContainer->GetActiveFrame()->RenderScene();
}
//==============================================================================================================================
void CGlToolFrame::OnBtnFixClick()
{
	FrameContainer->GetActiveFrame()->StopMoveObj();
	FrameContainer->GetActiveFrame()->SetFocus();
	ButtonFix->EnableWindow(false);
}

//==============================================================================================================================
CString CGlToolFrame::GetFileNameFromBD(CString Category,CString Caption)
{
	CString SqlStr; 
	CString FileStr;
	SqlStr="select [Category],[Caption],[FName] from DATA WHERE [Category] = '"+Category + "' and [Caption] = '" +Caption+"'";
	int nItem = 0;
TRY_ADO()
	 CADORecordset rs;
	 rs.SetConnection (& theApp.m_GlConnection);
	 rs.SetSQL ((LPCSTR)SqlStr);
     rs.SetOpenMode (true);
     rs.Open ();
	 rs.First();
     FileStr=(const char*)_bstr_t(rs.GetField("FName"));
  	 rs.Close();
CATCH_ADO()
 
   return FileStr;
}
//==============================================================================================================================

void CGlToolFrame::OnBtnAddClick()
{
	
	CString capt,cat;
	if(ComboCategory->GetCurSel()<0||ComboObj->GetCurSel()<0) return;
	ComboCategory->GetLBText(ComboCategory->GetCurSel(),cat);
	ComboObj->GetLBText(ComboObj->GetCurSel(),capt);
	CString FileName=GetFileNameFromBD(cat,capt);
	CPoint ind=AddToObjList(capt);
	FrameContainer->GetActiveFrame()->AddObj(RES_PATH+"Models\\"+GetFileNameWithoutExp(FileName)+"\\"+FileName,capt);
	FrameContainer->GetActiveFrame()->StartMoveObj(ind);
	FrameContainer->GetActiveFrame()->SetFocus();
	ButtonFix->EnableWindow(true);
	FrameContainer->GetActiveFrame()->RenderScene();
}
//==============================================================================================================================
void CGlToolFrame::OnCategoryBoxChange()
{
	int ind=ComboCategory->GetCurSel();
	if(ind>=0)
	{
	   CString CategoryCaption;
	   ComboCategory->GetLBText(ind,CategoryCaption);
       ReLoadObj(CategoryCaption);
	}
}
//==============================================================================================================================
void CGlToolFrame::OnObjComboBoxChange()
{

}
//==============================================================================================================================
void CGlToolFrame::OnObjListChange()
{
    CPoint ind;


/*  Slider->SetPos(FrameContainer->GetActiveFrame()->GetScale(ListObj->GetCurSel())*150.0);
	FrameContainer->GetActiveFrame()->SelectObj(ListObj->GetCurSel());
	FrameContainer->GetActiveFrame()->RenderScene();*/
}
//==============================================================================================================================
int CGlToolFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ComboCategory->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,
		                  CRect(15,25,180,45),this,IDC_CATEGORY);
	ComboObj->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,
		                  CRect(15,75,180,95),this,IDC_OBJCOMBO);	
	ListObj->Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
   | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
   | TVS_DISABLEDRAGDROP | TVS_NOTOOLTIPS | TVS_EDITLABELS, CRect(15,125,200,550), this, IDC_OBJLIST);


	ButtonAdd->Create("Добавить",WS_CHILD|WS_VISIBLE,CRect(15,125,90,150),this,IDB_BUTTONADD);
	ButtonFix->Create("Фиксировать",WS_CHILD|WS_VISIBLE,CRect(95,125,200,150),this,IDB_BUTTONFIX);
	ButtonFix->EnableWindow(false);

	Slider->Create(WS_CHILD|WS_VISIBLE ,CRect(10,10,194,34),this,IDP_POLZSCALE);
	Slider->LockWindowUpdate();
	Slider->SetRangeMin(1,0);
	Slider->SetRangeMax(1000,0);

	ReLoadCategory();
	
	return 0;
}
//==============================================================================================================================
void CGlToolFrame::OnPaint()
{
	CPaintDC dc(this); 
		CRect rect;
    CFont f1,f2;
	CBrush b; 
	CString StrBuf;
		
	GetWindowRect(rect);
    f1.CreateFont(-MulDiv(11,dc.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 700, 1, 0,
		                    0, RUSSIAN_CHARSET, 0, 0, 0 , 0,"Times New Roman");
	f2.CreateFont(-MulDiv(11,dc.GetDeviceCaps(LOGPIXELSY), 72), 0, 0, 0, 700, 0, 0,
		                    0, RUSSIAN_CHARSET, 0, 0, 0 , 0,"Times New Roman");
	dc.SelectObject(&f1);
	dc.SetBkMode(TRANSPARENT); 
	b.CreateSolidBrush(RGB(92,120,200)); 
	dc.SelectObject(&b);
	dc.Rectangle(0,0,rect.Width()+2,rect.Height()+2);
	b.DeleteObject();
	b.CreateSolidBrush(RGB(255,255,255)); 
	dc.SelectObject(&b);
}
//==============================================================================================================================
void CGlToolFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	GetClientRect(RectWnd);
    

	ListObj->MoveWindow(15,165,RectWnd.Width()-30,RectWnd.Height()-165-200,1);
	Slider->MoveWindow(15,RectWnd.Height()-200+30,150,20);
}
//==============================================================================================================================
void CGlToolFrame::ReLoadCategory()
{
	ComboCategory->Clear();
	ComboCategory->ResetContent();
	TRY_ADO()
	 CADORecordset rs;
	 rs.SetConnection (& theApp.m_GlConnection);
	 rs.SetSQL ("select [Category]from CATEGORY order by [Category]");
     rs.SetOpenMode (true);
     rs.Open ();
     int nItem = 0;
     if (!rs.IsEmpty ())
     for (rs.First () ; !rs.IsEoF () ; rs.Next ())
     {
		 ComboCategory->AddString((const char*)_bstr_t(rs.GetField("Category")));
     }
	 rs.Close();
      CATCH_ADO()

}
//==========================================================================================================================
void CGlToolFrame::ReLoadObj(CString category)
{
	
	CString SqlStr; 
		
 	ComboObj->Clear();
	ComboObj->ResetContent();
	TRY_ADO()
	 CADORecordset rs;
	 rs.SetConnection (& theApp.m_GlConnection);
	 SqlStr="select [Category],[Caption] from DATA WHERE [Category] = '"+category + "'" ;
	 rs.SetSQL ((LPCSTR)SqlStr);
     rs.SetOpenMode (true);
     rs.Open ();
     int nItem = 0;
     if (!rs.IsEmpty ())
     for (rs.First () ; !rs.IsEoF () ; rs.Next ())
     {
		 ComboObj->AddString((const char*)_bstr_t(rs.GetField("Caption")));
     }
	 rs.Close();
      CATCH_ADO()


}
//==========================================================================================================================
void CGlToolFrame::ReloadObjList(vector<CString> strings)
{
	/*if(strings.size()==0)
	ListObj->ResetContent();
	
	for(unsigned int i=0;i<strings.size();i++)
	{
		ListObj->AddString(strings[i]);
	}
*/
}
//==========================================================================================================================








