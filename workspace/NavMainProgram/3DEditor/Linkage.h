#pragma once
#include "afx.h"
#include <vector>
#include "matrix.h"
using std::vector;
struct TPointPriviazka
{
   double Lon;        //долгота
   double Dest;       //широта
   int    MapX,MapY;  //точка на карте

};
/*
F1(x,y)=a1+a2*x+a3*y+a4*x^2+a5*x*y+a6*y^2 - De
F2(x,y)=b1+b2*x+b3*y+b4*x^2+b5*x*y+b6*y^2 - Lo

F1IsolateX1 - выражает x из F1(x,y) + перед корнем дискрименанта
F1IsolateX2 - выражает x из F1(x,y) - перед корнем дискрименанта

F1xToF2(y)=b1+b2*F1IsolateX1+b3*y+b4*F1IsolateX1^2+b5*F1IsolateX1*y+b6*y^2 - Lo
*/
class CLinkage
{
  bool bindingFlag;                                   //true - привязка выполнена  
  bool created;
  double a[7],b[7];                                   //коф-ты полином. преобразования
  vector<TPointPriviazka> PointsPriviazka;            // хранит точки привязки
  CRect MapRect;                                      //размеры карты, которую привязываем           

//функции для решения системы деления отрезка пополам
  double F1(double &x,double &y,double &De);                  //возвращает значение 1-го полинома для x,y,De
  double F1IsolateX1(double &y,double &De);                   //из уравнения F1 выражена координата x со знаком +, ф-ция возвращает её значение для заданного y
  double F1IsolateX2(double &y,double &De);                   //из уравнения F1 выражена координата x со знаком -, ф-ция возвращает её значение для заданного y,
  double FxToF2(double &y,double &De,double &Lo,int X_Ind);   //значение ф-ции F2 с подстановкой вместо x выражение F1IsolateX1(F1IsolateX2)
  double F2(double &x,double &y,double &Lo);                  //возвращает значение 2-го полинома для x,y,Lo
  CPoint SolveDivHalfMethod(double &De,double &Lo);
 
  CPoint SolveModificNewtonMethod(double &De,double &Lo);
  CPoint SolveNewtonMethod(double &De,double &Lo);
public:
    void     CreateObj(CRect MapRect);                                              //создает объект         	
//функции для привязки
    void      AddPoint(int MapX,int MapY,double GeoDest,double GeoLon);             // добавляет точку привязки
	void      AddPointFromStr(CString S);
	void      GenerateKoefPriv();                                                   // генерирует коэф-ты системы
	int       GetPointPrivCount(){return PointsPriviazka.size();}                  // возвращает кол-во точек привязки
	CString*  GetListPointPriv(int *count);                                        // возвращает массив строк(<MapX> <MapY> <GeoDe> <GeoLon>) - для вывода в файл
    void      ConvertMapToGeoCord(CPoint Point,double *GeoDest,double *GeoLon);   
    CPoint    ConvertGeoToMapCord(double GeoDest,double GeoLon);     
	bool      IsBinding();                                                         // была ли выполена привязка?
    void      RewriteElem(unsigned int ind,int MapX,int MapY,
		                  double GeoDest,double GeoLon);                           //изменяет точку ind массива PointsPriviazka
	bool      GetPointPriv(TPointPriviazka *PointPriv,unsigned int ind);
    void      DeletePoint(unsigned int ind);                                      //удалить точку с массива опорных точек
	CLinkage(void);
	~CLinkage(void);
};
