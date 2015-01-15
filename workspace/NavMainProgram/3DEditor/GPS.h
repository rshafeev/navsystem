#pragma once
#include <vector>
using std::vector;
#include "GPSThread.h"
#include "include.h"
/*создано два вида исходящих протоколов: сетевой и файловый:

сетевой  - &#<ID>#<t1>#<t2>#<bool: 1- удалить; 0- добавить >;
файловый - &#<ID>#<t1>#<t2>#<bool: 1- удалить; 0- добавить >;^<имя(caption) устройства>.

фукнция FileToLanProtocol() - преобразовывает файловый в сетевой, т.е. удаляет <имя(caption) устройства> со строки
 в программе:
 protocol     = сетевой
 fileprotocol = файловый
*/
/*
@#<ID>#<Lo>#<De>#<Height>#<Time>;
*/

class CGPS
{
//HANDLE hMainThread;         // Дескриптор основного потока
//HANDLE hGetMes;             // Дескриптор функции, определяющей тип сообщения
WSADATA WSAData;           // Структура конфигурации DLL
SOCKET WinSocket;          // Наш сокет
sockaddr_in SockStruct;    // Структура содержит информацию о сокете

int Bind;                  // Хранит результат связывания локального сетевого адреса с сокетом
int Listen;                // Хранит результат прослушивания сокета
int Connect;               // Переменная хранит результат подключения клиента к серверу
int Send;                  // Переменная хранит результат отправки сообщения
int Recv;                  // Переменная хранит результат приема сообщения

CString List ;
CString ServIP;
bool connected;
int Port;

CGPSThread *GPSThread;

	bool ConnectByServer();
    bool LoadDataOfServ();//загружает данные о подкл с файла  RES/Settings.lan
    
public:
    vector<THardware> HardwareList; //список устройств, инфо о которых загружается с сервера 
	
	void UpdateHardwareList(int DelIndex);
	bool IsConnected();
    void StartGPS();
	void StopGPS();
	THardware* GetDrive(int index);
    void AddGPSHardware(THardware hard);
	bool LoadHardwareToList(CListBox* ListBox);
	bool LoadHardwareToList(CComboBox* ComboBox);
    bool LoadWindowOfGHardToList(int HardIndex,CListBox *ListBox);/*загружает в ListBox список окон, привяз. к этому устройству*/
	int LoadWindowOfGHardToChList(int HardIndex,CCheckListBox *ListBox);/*загружает в ListBox список ВСЕХ окон,
																 и отмечает те, к которым привязано
																 устройство HardwareList[HardIndex] возвращает длину самой длинной строки*/
	void LoadFromFile(CString FileName,CListBox *ListBox,bool IsSend);/*загружает с файла список устройств в HardwareList
																	  и выводит в ListBox`е если IsSend=true -
																	  отправить список на сервер*/
	void SaveToFile(CString FileName);/*сохраняет на диск список устройств в формате запроса */
	CString    HardwareToProtocol(THardware *hard,bool protocol_type,bool del);   //создать строку(протокол) данных об устройстве hard
    THardware* FileProtocolToHradware(CString FProtocol);  //заполнить структуру THardware из строки(ф.протокола) Protokol
	SOCKET* GetSocket();
	CString FileToLanProtocol(CString FileP);//файловый протокол в сетевой
    void SendHardwareList();//отсылает запрос на получение координат от всех устройств  HardwareList 
	bool TestIdentityHard(THardware *hard);//проверка уникальности утсройтсва(есть ли уже такое в HardwareList)
	
	THardware* GetHrdwareElem(CString IMEI);//получить уструктуру, хранящую данный IMEI
	void SendAllDelHardware(); //отправить на сервер, что клиент отключается
	CGPS(void);
	~CGPS(void);
};
