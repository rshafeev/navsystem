#pragma once


#include "afx.h"
#include "Geography.h"
#include <atlimage.h>

#include "afxinet.h"
#include "afxwin.h "
#include "afxmt.h "
//#include "mathem.h"

const short PICTSIZE=256;

const unsigned int STEPENTWO[21]={1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,
                                  16384,32768,65536,131072,262144,524288,1048576};//до 17
using std::vector;

struct TLoadedFiles
{
  CString FName;//без расширения
  int level;
  CPoint LeftTopBlock;
  CPoint IndexBlock;  
  bool loaded;
};

class CBlockRaster
{
private:
	CString PathName;//путь к папке, в которой хранятся блоки мировой карты в формате jpg
	CString ProgramPath;//путь к папке, в которой находится запущенная программа
	double BitmapOrigo(int level);
	double PixelsPerLonDegree(int level);
	double PixelsPerLonRadian(int level);

    bool created;//объект создан
    //CPoint GetBlockInd(int x,int y);

	//CPoint GetBlock(CPoint MouseCord);
    CPoint MassSize;//размеры массива ImagePtr, который нужно использовать
	CPoint LeftTopBlock;//левый верхний блок( хранит его положение на карте)
	CPoint block;
	CPoint Sdvig;
	CPoint ColOfBlocks;

	CRect WindowRect; //размеры окна
	CRect DrawRect;   
    int LEVEL;

	CImage LoadingPict,DissapearPict;
	CImage *ImageList;

	short **ImagePtr;//матрица указателей на ячейки с нужным иображением

    vector<short> ImageFree;//ячейка, в которую можно загружать новый Image 
CMutex Push1Section,Push2Section,DrawSection;
//CMutex MutexDraw;
    bool PushBlock(int i,int j,int BeginX,int BeginY,vector<TLoadedFiles> &NotLoad);
public:
    int GetLevel();
	CPoint GetMassSize();
    CPoint GetBlock();
    CPoint GetSdvig();
    CPoint GetLeftTopBlock();
	CPoint GetColOfBlocks();
    CRect GetDrawRect();
    CRect GetWindRect();
    
	void SetLevel(int level);
	void SetMassSize(CPoint size);
    void SetBlock(CPoint NewBlock);
    void SetSdvig(CPoint NewSdvig);
	void SetWindRect(CRect NewWindRect);
	void SetColOfBlocks(CPoint size);

	bool Create(CString PathName,CString ProgramPath);
	void FreeMemory();//очищает память
	vector<TLoadedFiles> PushMatrix();//заполнить матрицу
	vector<TLoadedFiles> PushMatrixWithCopy(CPoint delta);//копирует часть массива ImagePtt
    void DrawMatrix(CDC *pDC,CPoint ColRow,CPoint Position);//вывести матрицу. возвращает положение матрицы

	void ResizeMemory(CPoint NewColBlocks);//очищаем память и создаем массивы нового размера(окно изменило размер)
    void AllImagePtrFree();//заносит в ImageFree все указатели	
	CString GetBlockName(int NumX,int NumY,int level);

   void AddImage(TLoadedFiles Add);

	CPoint ConvertGeogrToBlock(double Lat,double Lon,int level);//геогр. координаты переводит в номер блока на карте
	CPoint ConvertGeogrToScreen(double Lat,double Lon,int level);//геогр. координаты переводит в номер блока на карте
	bool ConvertBlockToGeogr(CPoint block,double *Lat,double *Lon,int level);
	bool ConvertCoordToGeogr(CPoint block, CPoint BlockPoint, int level, double *Lat, double *Lon);

	CBlockRaster(CString PathName,CString ProgramPath);
	~CBlockRaster(void);

//	CMutex BlockRaterMutex; 
};
