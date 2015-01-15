#pragma once
#include "GlModels.h" 
#include "include.h"
#include "afx.h"
#include "3dLib.h"


class CLandshaft: public CGlModels
{
private:
bool InitFlag;
bool LoadRawFileToMass(int ***mass,CString FileName,int type);               // читает raw-файл и записывает в массив mass 
bool InitLandshaft(CString LandFile,CString TexFile);				   //выполняет полную загрузку ландшафта
void RenderLand(); //генерирует ландшафт в OpenGl
void GenVNormals();//считает вершинные нормали в массив MassVNormal ландшафта
void Tailing();    //размытие карты высот
TPoint GenTrianglePNormal(TPoint p1,TPoint p2,TPoint p3); // высчитывает нормаль к плоскости(треугольнику в 3D)
public:
   int **MassHeight,**MassTexture; // массивы: карта высот и текстуры
   TPoint **MassVNormal;            //вершинные нормали рельефа
   TLandProperties LandInfo;       //информация о ландшафте   
   GLuint GlModelList;             //память для списка ландшафта

   virtual void Draw();
   virtual void ReGenModel();
   virtual ~CLandshaft(void);

   CLandshaft(TLandProperties prop);
   CLandshaft(void);
   bool isInit()
   {
     return InitFlag;
   }
   
};
