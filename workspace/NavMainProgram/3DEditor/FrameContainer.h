#pragma once
#include "afxwin.h"
#include <vector>
#include "afx.h"
#include "include.h"
#include "Linkage.h"
#include "3DLib.h"
using std::vector;
const unsigned short MAXCOL_WIND=2000;// максимальное кол-во одновременно открытых окон
//Index      - индекс окна в массиве  
//FrameIndex - уникальный индекс окна(храниться в самои объекте "окно" и в структуре TContainer)
class CFrameContainer :	public CMDIChildWnd
{
	struct TContainer
	{
       CFrameContainer* FramePtr;
	   int FrameIndex;
	};
protected:
	unsigned int activeIndex;// уникальный индекс активного окна
    void DrawPoint(CDC *pDC,CPoint position,int ColorType);
	int UnicIndex;  //уникальный индекс окна(для потомков)
    THardware SelectedAbonent;//выбранный абонент(красный кружочек)
	unsigned int WindowType;
	bool FlagMiniMap;
private:
	bool DestroyFlag;
    vector<TContainer> Container;
	vector<unsigned short> FreeIndex;
	CFrameContainer *FramesMass[MAXCOL_WIND+1];/* i-й	 элемент массива( i-является уникальным индексом окна)  содержит
											   указатель на  окно по его уник. индексу   FramesMass[i] -  получаем указатель
   											   на окно с уник. индексом i*/ 
public:
CFrameContainer(void);


void             InitContainer();
void             AddToContainer(CFrameContainer* AddFrame);
void             DelFromContainer(int FrameIndex);
void             SetActive(int FrameIndex){activeIndex=FrameIndex;}          //занести в нонтейнер информацию, что окно FrameIndex стало активное 
void             SetSelectAbonent(THardware Hardware){this->SelectedAbonent=Hardware;}
int              InsertToListBox(CListBox* ListBox);                         //загружает в ListBox список окон,возвращает длину самого длинного заголовка 
void             DestroyContainer();

int              GetActiveIndexFrame();
int              GetWindowCount();
int              GetFrameIndex(int Index);                                   //получить уникальный индекс окна по индексу массива Container
int              GetIndexFromFrameIndex(int FrameIndex);                     //получить индекс массива по уникальному индексу окна
CFrameContainer* GetActiveFrame();
TContainer       GetContainerElement(int index);                             //получить элемент массива Container 
CFrameContainer* GetFramePtr(int FrameIndex){return FramesMass[FrameIndex];}
CString          GetWindText(int FrameIndex);                                //получить текст заголовка окна по уникальный индексу окна
THardware        GetSelectAbonent(){return SelectedAbonent;}                 //возвращает структру абонента, который наблюдается пользователем(красный кружочек)
bool             isDestroy();
CFrameContainer* Find3DWindow();                                             //ищет в контейнере окно "3д-карта"(оно может быть открыто только одно)
int              GetWindowType();                                            //возвращает тип окна: 0,1,2
bool             IsMiniMapVisiable(){return FlagMiniMap;}                    //видна ли мини-карта в активном окне?

virtual void     Repaint(){}
virtual CLinkage* GetLinkageObj(){return NULL;}                               //высчитываем коэф-ты карты(т.е. привязываем ее с помощью опорных точек)
virtual void      DrawHards(){}                                               //отобразить на карте всех абонентов
virtual void      FindAbonent(TGPSPoint *Position){}                          //поиск абонента на карте с переходом на середину положения абонента
virtual void      Save(CString FName=""){}                                    //сохранить проект
virtual           ~CFrameContainer(void);
virtual void      ShowMiniMap(bool visiable){}                                 //показать/скрыть мини-карту
//виртуальные функции исключительно для окна CGlFrame

virtual void     ShowGrid(bool show){}                                         //видна/скрыта сетка
virtual bool     IsGridShow(){return false;}	                               //Есть ли сетка в gl окне?
virtual void     AddObj(CString FileName,CString Caption){}                    //добавить объект в окно
virtual void     StartMoveObj(CPoint ind){}                                       //начать движение объекта
virtual void     StopMoveObj(){}                                               //остановить движение объекта
virtual void     ChangeObjScale(CPoint ind,double &scaleX,double &scaleY,double &scaleZ){}                        //изменяет масштаб(размер) объекта
virtual void     RenderScene(){}                                               //обновить opengl сцену
virtual double   GetScale(CPoint ind){return 0.0;}                                //масштаб объекта
virtual void     GetObjPos(CPoint ind,double &x,double &y,double &z){}      //позиция объекта
virtual void     SelectObj(CPoint ind){}                                          //выбирает ind объект(3ds), после чего он выделяется синим
virtual void     InitLand(TLandProperties LandProperties){}//инифиализация ландшафта


virtual void     LandMixing(CString Tex1File,CString Tex2File,CString Tex3File,CString WaterFile,CString TexVecFile){} //загрузка текстур для разукраски шейдером
virtual void     AddMapToLand(CString FileName){}                    

};
