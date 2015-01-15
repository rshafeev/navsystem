#pragma once
#include "afx.h"
#include "3DLib.h"
#include <vector>
#include "3ds.h"
#include "GlLandshaft.h"

using std::vector;

//контейнер 3ds объектов, некоторых других моделей(сетка, местоположение...) 
class CGlContainer
{
struct TModelCord
{
	
  CVector3 ScaleKoef;          //масштабирование модели
  CVector3 pRotate;            //коэ-ты поворота 
  CVector3 pPos;               //позиция модели
  double   pAlphaValue;        //угол поворота модели
};

struct TStaticModel
{
 GLuint ModelMemory;
 CString caption;
 int type;            //тип модели (пока что не использую)  
 vector< TModelCord> MassCoord; // список положений моделей(модель в памяти одна, а отображается на многих позициях, например куча деревьев, а модель дерева одна)
};

struct TGridModel
{
 GLuint ModelMemory;
 CString caption;
 int type;            //тип модели (пока что не использую)  
 CVector3 Pos;
 CVector3 ScaleKoef;          //масштабирование модели

};
private:
vector<int>             FreeUnicObjInd;      //уникальный индекс каждого объекта  в памяти opengl 
vector<TStaticModel>	ModelsList;         //хранит все 3ds модели в памяти
TGridModel              GridModel;          // сетка в памяти
CLoad3DS                g_Load3ds;		    //Наш 3DS класс.
CPoint                  selected_ind;       //индекс выбранного объекта
public:

void       AnimateModel(t3DModel *pModel, t3DObject *pObject);
void       InitStaticObj();                                   //
void       ShowGrid(bool show,int Width,int Height,int Step); //добавляет/удаляет из памяти объект сетка
void       ShowLand(bool show);                               //добавляет/удаляет из памяти объект ландшафт
void       RenderObjects();
int        GetGridInd();
void       DeleteObj(int index);
void       SetPosition(CVector3 position);                       //выводит положение на карте(две линии) 
CPoint     LoadModel(CString FileName,CString Caption,CVector3 pos);       //загружает новую модель
void       FixMovingModel(CVector3 position);                     //фиксирует добавленную модель(т.е. записывает в память и в  ModelsList)
void       MovingNewModel(CVector3 position);                   //двигает новую подргуженную модель
void       GetObjString(vector<CString> *StrList);            //формирует список строк(caption) названий объектов
void       ChangeObjPos(CPoint ind,CVector3 position);             //задает позицию фигуре
void       ChangeScale(CPoint ind,CVector3 scale);//масштабирует объект
void       Destroy();
void       SelectObj(CPoint ind);
bool       GetSaveProtocol(vector<CString> *ProtocolStr,bool rewrite);//обавляет инфу в ProtocolStr: массив строк, запис. в файл сохранения
//финкции отправки данных во внешнюю среду
CVector3     GetPosition(CPoint index);
CVector3     GetScale(CPoint index);


CGlContainer(void);
~CGlContainer(void);
};
