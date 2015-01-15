#include "Raster.h"
#include "Memory.h"
#include <atlimage.h>

#ifndef _RASTER_INCLUDED
#define _RASTER_INCLUDED
// макрос для определения количества байт в выровненной по DWORD строке пикселов в DIB 
// Width - длина строки в пикселах; BPP - бит на пиксел
#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * \
(DWORD)(BPP) + 31) >> 5)) << 2) 

//****const


class CBMPRaster : public CRaster
{
   LPBITMAPINFO   m_pBmpBMI;   //указатель на описание изображения
   LPBITMAPINFO   m_pBmpMiniBMI;
   PBYTE          m_pData;     //указатель на данные проец. файла
   PBYTE          m_BmpData; //указатель на данные битмапа, который выводится на экран 

   CMemoryMappedFile MemoryF;  //объект проецирования файла
   CRect WindowRect;           //размеры окна вывода

   
   bool LoadFlag;   // загрузка прошла успешно
   bool project_type; //вид загружаемого файла, проект(не надо загружать мини-карту)
   int Rop;
   CString FileName;  // имя к файлу
   struct TColor 
   {
     unsigned int  R;
	 unsigned int  G;
	 unsigned int  B;
   }**Color;
private:
	PBYTE MiniMapMass;
    CRect MiniMapSize;
	CImage *MixedMiniMap;
	
	void RefreshMemory();
public:
  //   virtual СRect SetMiniMap(CRect MapSize);///*создает битовый массив уже отмасштабированной карты до размера MapSize   возвращает размеры из-я(окно может быть неправ. формы)*/
   virtual CRect SetMiniMap(CRect MapSize);
   virtual void DrawMiniMap(CDC *dc,CPoint pos);//рисует миникарту на  dc с отступом(со сдвигом) pos
   void DrawMiniMap(HDC hDC,CPoint pos);//рисует миникарту на  dc с отступом(со сдвигом) pos

   virtual void Resize(CRect WindRect);
   virtual bool Load(CString FileName,CRect WindRect);
   virtual void Free();
   void Draw(CRect Output,CDC *dc,CPoint pos);
   CRect SizeNormalize(CRect pict,CRect Output);
   void Clear();
   
   /*void DrawBitmap(CDC *pDC, LONG x=0, LONG y=0, LONG cx=0, LONG cy=0,
                   LONG x0=0, LONG y0=0, LONG cx0=0, LONG cy0=0,
                   int str_mode=COLORONCOLOR, DWORD rop=SRCCOPY);*/
   void PushMass(CRect oldRect,CRect newRect);
   CBMPRaster();
   ~CBMPRaster();
};
#endif
