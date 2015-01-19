#pragma once


// CHardToolFrame

class CHardToolFrame : public CFrameWnd
{
   DECLARE_DYNAMIC(CHardToolFrame)
   CComboBox *ComboHardware;
   CListBox  *ListBox;
   int DriveIndex;                     /*индекс( выбранного устройства) в массиве  
									     theApp.MainForm->GPS->HardwareList*/
   THardware *SelectedDrive;
   CButton   *ButtonPrevious;
   CButton   *ButtonNext;
   CButton   *ButtonFind;               //кнопка поиска устройства(абонента) на карте
   CRect ClientRect;                    //размеры клиентской области окна
   bool ConnectStatus;                //состояние подключения: true - подкл к серверу
   CDC dcMemory;
   void UpdateVirtualCanvas(CDC *pDC);        //обновляет контекст в памяти и копируем его в pDC
public:
	void UpdateInfo();                //обновить список устройств
	void UpdateWindowList(int index); //обновить список окон и другую инфу о index устройстве
	void SetDriveRedraw();            /*проверить, полученные данные с сервера принадлежат выбранному в 
									    ComboHardware  устройству(перерисовку делаем) или нет(перерисовку
										не делаем)*/
	void CloseConnect();              //вызывается если соединение было прервано(или просто откл)
	void OpenConnect();               //вызывается если выполнено соединение с сервером
	void AddToDriveList(int index);   //добавить в ComboHardware запись
	void DeleFromDriveList(int index);//удалить из ComboHardware запись
	void UpdateCanvas();
	CHardToolFrame();
	virtual ~CHardToolFrame();
    
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBoxChange();
	afx_msg void OnSessionChange();
	afx_msg void OnBtnFindClick();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};


