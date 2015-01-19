#include "StdAfx.h"
#include "GPSThread.h"
#include "3DEditor.h"
CGPSThread::CGPSThread(SOCKET *WinSocket)
{
	this->WinSocket=WinSocket;
	Terminated=false;
}

CGPSThread::~CGPSThread(void)
{

}
BOOL  CGPSThread::InitInstance()
{
	m_bAutoDelete=true;
   return TRUE;   
}
//***********************************************************************************************************************
void CGPSThread::GetMessage(CString Msg)
{
	///////////////////////////////////////////////////
	//ВАЖНАЯ Ф-ЦИЯ - принимает сообщение от сервера  //
	//и обрабатывает его!                            //
	///////////////////////////////////////////////////

   // Msg: @#<ID>#<Lo>#<De>#<Height>#<Time>;
   unsigned int i;
   CString buf;
   THardware *Hard;
   TGPSPoint AddPosition;
    //считываем строку в структуру
   while(Msg.GetLength()!=0)
   {
     Msg.Delete(0,2);           //<ID>#<Lo>#<De>#<Height>#<Time>;
     //считываем IMEI(ID) устройства:
     buf=Msg.SpanExcluding("#");
     //ищем такое устройство в списке HardwareList
	 if((Hard=GPS->GetHrdwareElem(buf))==NULL) return;
     Msg.Delete(0,buf.GetLength()+1);

     //считываем Lo:
     buf=Msg.SpanExcluding("#");
     //ищем такое устройство в списке HardwareList
     AddPosition.Longitude=atof(buf);
     Msg.Delete(0,buf.GetLength()+1);

     //считываем De:
     buf=Msg.SpanExcluding("#");
     //ищем такое устройство в списке HardwareList
     AddPosition.Description=atof(buf);
     Msg.Delete(0,buf.GetLength()+1);

     //считываем De:
     buf=Msg.SpanExcluding("#");
     //ищем такое устройство в списке HardwareList
     AddPosition.Height=atof(buf);
     Msg.Delete(0,buf.GetLength()+1);

     //считываем time:
	 buf=Msg.SpanExcluding(";");
     //ищем такое устройство в списке HardwareList
     AddPosition.DateTime=buf;
     Msg.Delete(0,buf.GetLength()+1);

      if(Hard->runtime==false)
		 Hard->PositionFromBD.push_back(AddPosition);
  
   }
   if(Hard->runtime==true)
   {
	     Hard->PositionFromBD.clear();
		 Hard->PositionFromBD.push_back(AddPosition);
      for(i=0;i<Hard->WindowsList.size();i++)
      {
	    if(FrameContainer->GetFramePtr(Hard->WindowsList[i])!=NULL)
	       FrameContainer->GetFramePtr(Hard->WindowsList[i])->DrawHards();
      }
   }
   //выводим на все карты
    HardToolFrame->UpdateCanvas();
}

//************************************************************************************************************************
int CGPSThread::Run()
{
  // Запускаем бесконечный цикл приема сообщений
   // Делаем так, чтобы не было мусора во входящих данных

while(1)
{
  if(Terminated==true) 
		return WM_QUIT;

    for (int i=0; i<SIZE; i++)
      Buffer[i] = '\0';
    if(WinSocket==NULL)
	{
		return WM_QUIT;
	}
    // Получаем данные

    Recv = recv(*WinSocket, Buffer, SIZE, 0);
	// Если сокет отключен
    if (Recv == SOCKET_ERROR)
    {
		if(GPS->IsConnected()==true)
	     MessageBox(MainForm->m_hWnd,"Сервер отключился!","",0);
	  MainForm->Menu.GPSConnect=false;
	  GPS->StopGPS();
	  return WM_QUIT;
    }
	else
	if(Buffer[0]=='@')//значит входящщее сообщение
	{
		GetMessage(Buffer);
	}
}
	return WM_QUIT;
}
//==========================================================================================================================
void CGPSThread::Terminate()
{
  Terminated=true;

}
//==========================================================================================================================
int CGPSThread::ExitInstance()
{
  return CWinThread::ExitInstance();
}
