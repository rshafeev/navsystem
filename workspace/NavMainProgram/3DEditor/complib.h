
#pragma once

#include "stdafx.h"
#include "afx.h"
static CString GetStrFromEdit(CEdit *Edit);
//===========================================================================================================================
static CString GetStrFromEdit(CEdit *Edit)
{
//функция возвращает строку, введенную в CEdit
  CString result="";
int i, nLineCount = Edit->GetLineCount();
CString strText, strLine;

for (i=0; i < nLineCount; i++)
{
   int len = Edit->LineLength(Edit->LineIndex(i));
   if(len==0)continue;
   Edit->GetLine(i, strText.GetBuffer(len), len+1);
   
   strText.ReleaseBuffer(len);
   result+=strText;
}

  return result;
}

//===========================================================================================================================
class CDirDialog //класс диалогового окна "Выбор папки"
{
private:
   LPTSTR fname;
   LPTSTR PszAlloc(int cch)
   {
       return (LPTSTR) LocalAlloc(LMEM_FIXED, sizeof(TCHAR) * (cch+1));
   }
   bool PszDeAlloc(HLOCAL mem_ptr)
   {
      return (LocalFree(mem_ptr)==NULL) ? true : false;
   }

static BOOL FGetDirectory(LPTSTR szDir,HWND hWnd)
{ 
BOOL fRet;
   TCHAR szPath[MAX_PATH];
   LPITEMIDLIST pidl;
   LPITEMIDLIST pidlRoot;
   LPMALLOC lpMalloc;
   BROWSEINFO bi =
   {
       hWnd,
       NULL,
       szPath,
       "Выберите папку",
       BIF_RETURNONLYFSDIRS,
       NULL,
       0L,
       0
     };
   if (0 != SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &pidlRoot))
     return FALSE;
   if (NULL == pidlRoot)
     return FALSE;
   bi.pidlRoot = pidlRoot;
   pidl = SHBrowseForFolder(&bi);
   if (NULL != pidl)
     fRet = SHGetPathFromIDList(pidl, szDir);
   else
     fRet = FALSE; // Get the shell's allocator to free PIDLs
   if (!SHGetMalloc(&lpMalloc) && (NULL != lpMalloc))
   {
     if (NULL != pidlRoot)
     {
       lpMalloc->Free(pidlRoot);
     }
     if (NULL != pidl)
     {
       lpMalloc->Free(pidl);
     }
   lpMalloc->Release();
   }
return fRet;
}
public:
	bool DoModal(HWND pMainWND)
	{
       fname=PszAlloc(250);
       if(FGetDirectory(fname,pMainWND)==false) return false;
       
       return true;
	}

	CString GetPath()
	{
        return CString(fname);
	}

	~CDirDialog()
	{
       PszDeAlloc((HLOCAL)fname);  
	}
};
//===========================================================================================================================