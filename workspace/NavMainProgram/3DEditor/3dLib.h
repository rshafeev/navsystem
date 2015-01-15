//#include "3DLib.h"
#pragma once
#include "stdafx.h"
#include "afx.h"
#include <gl\glew.h>
#include <gl\gl.h>
#include <gl\glaux.h>
#include <gl\glu.h>
#include <gl\glext.h>
#include <gl\glut.h>
#include <gl\wglext.h>

#include "stdafx.h"
using namespace std;

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

#pragma comment (lib,"OpenGL32.Lib")
#pragma comment (lib,"GLu32.Lib")
#pragma comment (lib,"WinMm.Lib")
#pragma comment (lib,"GLaux.Lib")

struct TLandProperties
{//свойства  ландшафта
  CString LandFile;                         //путь к файлу высот
  CString TexFile;                          //путь к файлу текстуры
  int ElemSize;                             //в каком формате хранятся данные 
  int StepSize;                             //шаг, с которым читается файл высот
  int width,height;                         //размеры карты высот 
  int MaxHeight,MinHeight;                  //максимальная и минимальная высота на карте
  bool AutoCalcMaxMinHeight;                //макс/мин высоты вычислять программно или их задал пользователь?
  double ScaleX,ScaleY;                     // масштабирование по X,Y(растяжение или сжатие)
  int DW,DH;                                //размер с учетом StepSize(w/StepSize;h/StepSize)
  
  bool FilterTaling;                        //размытие
  bool FilterFractal;                       //фрактальное построение гор
  bool FilterAverage;                       //усреднение
  bool FilterSmooth;                        //сглаживание
};

struct CVector3{
	float x,y,z;
};

struct CVector2{
	float x,y;
};

// Это структура полигона. Она используется для индексирования массивов координат
// вершин и текстур. Эта информация сообщает нам о том, какие номера вершин в массиве
// какому полигону принадлежат. То же самое касается текстурных координат.
struct tFace
{
	int vertIndex[3];			// indicies for the verts that make up this triangle
	int coordIndex[3];			// indicies for the tex coords to texture this face
};
 
 
// Эта структура хранит информацию о материале. Это может быть текстурная карта света.
// Некоторые значения не используются, но я оставил их, чтобы могли увидеть их для
// примера.
struct tMaterialInfo
{
	char  strName[255];			// Имя текстуры
	char  strFile[255];			// Имя файла текстуры
	BYTE  color[3];				// Цвет обьекта (R, G, B)
	int   texureId;				// ID текстуры
	float uTile;				// u-tiling текстуры (Сейчас не используется)
	float vTile;				// v-tiling текстуры (Сейчас не используется)
	float uOffset;				// u-offset текстуры (Сейчас не используется)
	float vOffset;				// v-offset текстуры (Сейчас не используется)
};
 
// Содержит всю информацию о модели/сцене.
struct t3DObject
{
	int  numOfVerts;			// Число вершин в модели
	int  numOfFaces;			// Число полигонов в модели
	int  numTexVertex;			// Число текстурных координат
	int  materialID;			// ID текстуры для использования, индекс массива текстур
	bool bHasTexture;			// TRUE если есть текстурная карта для этого обьекта
	char strName[255];			// Имя обьекта
	CVector3  *pVerts;			// Массив вершин обьекта
	CVector3  *pNormals;			// Нормали обьекта
	CVector2  *pTexVerts;			// Текстурные координаты
	tFace *pFaces;				// Полигоны обьекта

	int  positionFrames;		// Число кейфреймов перемещения
	int  rotationFrames;		// Число кейфреймов вращения
	int  scaleFrames;		// Число кейфреймов масштабирования
 
	CVector3 vPivot;		// Точка опоры обьекта
 
	vector<CVector3> vPosition;	// Массив позиций обьекта
	vector<CVector3> vRotation;	// Массив вращений обьекта
	vector<CVector3> vScale;	// Массив размеров обьекта
 
	vector<float> vRotDegree;	// Углы вращения обьекта
};
 
 
 
// Содержит информацию о модели. Тоже неплохо бы обернуть в класс. 
struct t3DModel
{
	int numOfObjects;			// Число обьектов в модели
	int numOfMaterials;			// Число материалов модели
	vector<tMaterialInfo> pMaterials;	// Число обьектов материалов (текстуры и цвета)
	vector<t3DObject> pObject;		// Список обьектов в модели

	int numberOfFrames;	// Число кадров анимации для данной модели (как минимум 1)
	int currentFrame;	// Текущий кадр анимации модели
};
 
