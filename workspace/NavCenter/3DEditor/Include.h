#pragma once



/*#include <Stdio.H>
#include <Stdlib.H>*/
#include <Math.H>
#include <vector>
using std::vector;
struct TGPSPoint
{
  double Longitude;                        // долгота
  double Description;                      // широта
  double Height;                           // высота над уровнем моря
  CString DateTime;                        // время прихода сообщения на сервер
};
struct THardware                            //хранит ифо о подкл клиенте
{
   CString Name;                            //метка<caption> утсройства
   CString ID;                              //IMEI устройства
   bool runtime;                            //загружать в реальном времени
   CString DateTime1,DateTime2;             //елси runtime=false: хранит промежуток времени 
   vector<int> WindowsList;                 //массив FrameIndex окон, в которые выводится THardware
   vector<TGPSPoint> PositionFromBD;        /*координаты в момент времени м/у DateTime1 и DateTime2
									          содержит данные если runtime=false*/
 };