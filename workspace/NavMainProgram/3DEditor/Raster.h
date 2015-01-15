#pragma once

#include <vector>
using std::vector;
//============================================================================================================================

class CRaster
{
public:
    struct TPictInfo
   {
      unsigned int width;
	  unsigned int height;
      unsigned int bit;

   };
private:
protected:
	bool LoadFlag;                                 //true - карта загружена, всё ок
	TPictInfo m_pBMI;                              // инфа о картинке(размер, биты)
	CRect PictRect;
	CRect Output;                                  //размеры области картинки, которая выведена(выведится) на экран  
   
public:

// функции относительно карты(изображения)
	TPictInfo GetPictInfo(){return m_pBMI;}       // возвращает инфу о загр. изображении
	void SetOutputRect(CRect rect){Output=rect;}  // заносит в объект размеры области картинки для обработки(например облать вывода на экран)
	CRect GetOutputRect(){ return Output;}        // возвращает размеры области вывода

// вирт функии для потомков
	virtual void Draw(CRect Output,CDC *dc,CPoint pos){}
	virtual bool Load(CString FileName,CRect WindRect){return true;}
	virtual void Resize(CRect WindRect){}
	virtual void DrawMiniMap(CDC *dc,CPoint pos){}
	virtual CRect SetMiniMap(CRect MapSize){CRect r; return r;}
	virtual void Free(){}
// сохранение проекта
	virtual bool     LoadFromFile(){return true;}
	virtual bool     SaveToFile(){return true;}

	CRaster(CString FileName);
	CRaster(void);
	~CRaster(void);
};

//============================================================================================================================
//класс обработки ошибок
class CRasterException
{
public:
	enum _errs{ceUnknown,ceLoad,ceFileExists,ceFileUsing};
private:
	_errs Error;
public:    
	CString GetError()
	{
		switch (Error)
        {
        case ceUnknown                  : return "Ошибка не имеет описания";
        case ceLoad                     : return "Ошибка при загрузке объекта.";
		case ceFileExists               : return "Данный файл не существует";
		case ceFileUsing                : return "Данный файл уже используется";

        default : break;
        }
        return "Неизвестная ошибка";
	}
	CRasterException(_errs Error){this->Error=Error;}
};
//============================================================================================================================

//операторы обработки ошибок
#define TRY_RASTER() \
   try \
      { 
#define CATCH_RASTER() \
      } \
   catch (CRasterException& e) \
      { \
	    CString strErr= e.GetError();\
	    AfxMessageBox (strErr, MB_ICONWARNING); \
      } \


//============================================================================================================================
