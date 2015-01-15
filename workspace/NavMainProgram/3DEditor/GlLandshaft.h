#pragma once
#include "include.h"
#include "Shader.h"
#include "Texture.h"
#include "3DLib.h"
class CGlLandshaft
{
struct THgtGeoInfo
{
  CVector2 Block;            //градус hgt файла. отсчет от (-180,-90)
  CVector2 GeoCord;          //градус на карте  с отсчетом от 0
  CVector2 GlobMapSize;      //размер всего земного шара в данном масштабе(1201-один градус)
}HgtGeoInfo;
private:

static WORD  sh(WORD x);                                                //меняет мистами старшие и младшие биты
bool   PointBilongTriangle(CPoint Point,CVector3 *Triangle);             //принадлежить ли точка треугольнику?
int    IsolateYFromEauation(CPoint pp,CVector3 *p);           //находим высоту для точки Point(X,Y-?,Z) 
void	     GenVNormals();                                              //высчитали вершинные нормали
void 	     LoadLandToMemory();                                         //загрузили ландшафт в память(opengl list) 
CVector3	 GenTrianglePNormal(CVector3 p1,CVector3 p2,CVector3 p3);    //вычисляет нормаль треугольника 
bool         LoadRawFileToMass(int ***mass,CString FileName,int type);   //загружает в массив mass данные из файла FileName; type - кол-во байтов под один элемент

bool InitFlag;
bool MixingFlag;                                 //была ли создана шейдерная раскраска?
TLandProperties LandInfo;                        //информация о ландшафте   
GLuint GlMemoryPt;                               // указатель на память для списка ландшафта
int **MassHeight,**MassTexture;                  // массивы: карта высот и текстуры
CVector3 **MassVNormal;                          //вершинные нормали рельефа
CVector2  ScaleTexMixing;
CShader MixingShader;                            // объект(программа) шейдер смешивания
CShader MapShader;                               // объект(программа) шейдер закрашивания одной текстурой
CShader WaterShader;
CTexture TexMixing[5];                           // 3 текстуры, которые используются для смешивания(буду ее тоже называть текстурной картоц)
int TextureDetalLevel;                           //  коэ-т детализации текстуры, больше, камера дальше
vector<CTexture> MapTextures;                    // массив текстур-карт, накладываемых на ландшафт 
int SelectMapInd;                                //  индекс текущей карты
template<class T> void         FilterDiamondSquareFractal(T** &heights,float roughness,int width,int height);
template<class T> void         FilterTailing(T** &heights,int w, int h);
template<class T> void         FilterSmooth(vector<T> &m_heights,int w,int h);
template<class T> void         FilterAverage(T** &heights,int w, int h);
CPoint MapSize;
CVector2 pictCord;
public:

int           Create();                              
int			  Create(TLandProperties Properties,CPoint MapSize);    //создаем ландшафт по заданным свойствам в структуре Properties
void		  Render();                                             //отрисовует ландшафт
bool          IsInit();                                             //был ли инифиализирован ландшафт?
void          NextMap();                                            //загрузить следующую карту-текстуру (-1 - просто цвет,0-шейдер, 1.. - MapTextures)
void	      BackMap();                                            //загрузить предыдущую карту-текстуру (-1 - просто цвет,0-шейдер)
void          SetTextureDetalization(int DetalizationKoef);
void	      AddMapMixing(CString Tex1File,CString Tex2File,CString Tex3File,CString TexWaterFile, CString TexVec);
bool          IsLandMixing();
void          GetMixingInfo(CString &Tex1File,CString &Tex2File,CString &Tex3File, CString &TexVec,CString &TexWaterFile);
void          AddMap(CString TexFileName);
bool          SaveLand(CString PathName,vector<CString> *ProtocolStr,bool rewrite);
int           GetLandGridType();
CVector2      GetHgtGeoCord(CPoint p);                      // для координат p возвращает  широту и долготк(только для hgt сетки)
int           GetHeightForPoint(CPoint p);                  // получить значение высоты для p
bool          IsPossibleMove(CPoint OldP,CPoint NewP);      // можно ли передвинуться из точки  OldP в точку NewP
void          Free();

vector<CString>  PrintTexFiles();

	CGlLandshaft(void);
	~CGlLandshaft(void);
};
