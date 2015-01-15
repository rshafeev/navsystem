#pragma once
#include "afxwin.h"
#include "BlockRaster.h"
#include "afxmt.h "
#include <vector>
#include "afxinet.h"
using std::vector;

class ÑInetRaster :	public CWinThread
{

bool *Terminated;
CString FolderCache;
CString ServerUrl;
CBlockRaster *BlockRaster;
int LoadFileFromInet(CString FNameWithoutExp);
vector<TLoadedFiles> *LoadedContainer;
int *LoadedCount;
CWnd *pFrame;
int UnicWindIndex;
void Draw(TLoadedFiles LoadedFile);
   CMutex *InetMutex;
public:
void Connect();
	void Create(CBlockRaster *BlockRaster,bool *TerminatedThread,vector<TLoadedFiles>* LoadedContainer,
		int *LoadedCount,CWnd *pDC,int UnicWindIndex,CMutex *Mutex);
	
	ÑInetRaster();
	~ÑInetRaster(void);
	virtual BOOL InitInstance();
	virtual int Run();





	
};
