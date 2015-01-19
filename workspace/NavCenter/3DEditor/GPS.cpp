#include "StdAfx.h"
#include "GPS.h"
#include "3DEditor.h"


CGPS *GPS;

CGPS::CGPS(void)
{
	GPSThread=NULL;
	connected=false;
}

CGPS::~CGPS(void)
{
	
	for(unsigned int i=0;i<HardwareList.size();i++)
	{
		HardwareList[i].WindowsList.clear();
		HardwareList[i].PositionFromBD.clear();
	}
	HardwareList.clear();


}
THardware* CGPS::GetHrdwareElem(CString IMEI)
{
   unsigned int i;
   for(i=0;i<HardwareList.size();i++)
   {
	   if(HardwareList[i].ID==IMEI)
		   return &HardwareList[i];
   }

   return NULL;
}
//************************************************************************************************************************
void CGPS::SendAllDelHardware()
{
  	if(connected==true)
	{
	  unsigned int i;
	  CString protocol;
	  for(i=0;i<HardwareList.size();i++)
	  {
		  protocol=HardwareToProtocol(&HardwareList[i],false,true);
		  send(WinSocket,(LPCSTR)protocol,protocol.GetLength(),0);
	  }
	}
}
//************************************************************************************************************************

bool CGPS::TestIdentityHard(THardware *hard)
{
  int i;
  for(i=0;i<(int)HardwareList.size();i++)
  	  if(HardwareList[i].ID==hard->ID)
        return false;
  return true;
}
//***************************************************************************************************************
void CGPS::SendHardwareList()
{
	if(connected==true)
	{
	  unsigned int i;
	  CString protocol;
	  for(i=0;i<HardwareList.size();i++)
	  {
		  protocol=HardwareToProtocol(&HardwareList[i],false,false);
		  send(WinSocket,(LPCSTR)protocol,protocol.GetLength(),0);
	  }
	}
}
//***************************************************************************************************************
CString CGPS::FileToLanProtocol(CString FileP)
{
  CString Protocol;
  Protocol=FileP.SpanExcluding("^");

   return Protocol;

}
//***************************************************************************************************************
THardware* CGPS::FileProtocolToHradware(CString FProtocol)
{
//&#<ID>#<t1>#<t2>#0;^<caption>  -структура протокола

	THardware *Hardware=new THardware;
    CString buf;
	//считываем ID
	FProtocol.Delete(0,2);                    //<ID>#<t1>#<t2>#0;^<caption>.
	buf=FProtocol.SpanExcluding("#");
	Hardware->ID=buf;
	FProtocol.Delete(0,buf.GetLength()+1);      //<t1>#<t2>#0;^<caption>.
	//считываем time1
	buf=FProtocol.SpanExcluding("#");
	Hardware->DateTime1=buf;
	FProtocol.Delete(0,buf.GetLength()+1);      //#<t2>#0;^<caption>.
	//считываем time2
	buf=FProtocol.SpanExcluding("#");
	Hardware->DateTime2=buf;
	FProtocol.Delete(0,buf.GetLength()+1);      //#0;^<caption>.
    //считываем Name
	buf=FProtocol.SpanExcluding("^");
	FProtocol.Delete(0,buf.GetLength()+1);      //<caption>.
	buf=FProtocol.SpanExcluding(".");
	Hardware->Name=buf;

	if(Hardware->DateTime1=="."||Hardware->DateTime2==".")
		Hardware->runtime=true;
	else
		Hardware->runtime=false;

    

	return Hardware;
}
//***************************************************************************************************************
CString CGPS::HardwareToProtocol(THardware *hard,bool protocol_type,bool del)
{
	///////////////////////////////////
	//  protocol_type:true-файловый  //
	//  protocol_type:false-сетевой  //
	///////////////////////////////////
CString message="";

    if(hard!=NULL)
	message="$";
	message+="#";
	message+=hard->ID;
	message+="#";
	if(hard->runtime==true)
	{
   		message+=".#.";
        if(del==false)
			message+="#0;";
		else
			message+="#1;";

	}
	else
	{
		message+=hard->DateTime1+"#"+hard->DateTime2;
	    if(del==false)
			message+="#0;";
		else
			message+="#1;";

	}
	if(protocol_type==true)
	{
		message+="^"+hard->Name+".";
	}
    return message;
}
//***************************************************************************************************************
void CGPS::LoadFromFile(CString FileName,CListBox *ListBox,bool IsSend)
{
  CStdioFile File;
  CString str;
  if(connected==true)
	   GPSThread->SuspendThread();//приостановили поток приема сообщений(для синхронизации)
  
  if(File.Open(FileName,CFile::modeRead)!=TRUE)return;
  HardwareList.clear();//очистили список устройств
  //читаем файл и заносим в массив HardwareList
  while(File.ReadString(str)==TRUE)
  {
	  if(str!="")
	  {
	    THardware *Hardware;
	    Hardware=FileProtocolToHradware(str);
	    HardwareList.push_back(*Hardware);
	    //отсылаем новое устройство на сервер, если нужно:
        if(IsSend==true&&connected==true)
		{
		   str=FileToLanProtocol(str);
	       send(WinSocket, (LPCSTR)str,str.GetLength(), 0);
		}
	  }
  }
  
  LoadHardwareToList(ListBox);
  if(connected==true)
   GPSThread->ResumeThread();
}
//***************************************************************************************************************
void CGPS::SaveToFile(CString FileName)
{
 CStdioFile File;
 
 if(File.Open(FileName,CFile::modeCreate|CFile::modeWrite)!=TRUE) return;
 unsigned int i;
 CString str;
 for(i=0;i<HardwareList.size();i++)
 {
	 str=HardwareToProtocol(&HardwareList[i],true,false);
	 str+="\n";
	 if(str!="")
		 File.WriteString((LPCSTR)str);

 }
 File.Close();
 
}
//***************************************************************************************************************
int CGPS::LoadWindowOfGHardToChList(int HardIndex,CCheckListBox *CheckListBox)
{
      CString g,result;
      int ind,maxLength,i;
	  unsigned int j;
	  maxLength=FrameContainer->InsertToListBox(CheckListBox);
	  if((int)HardwareList.size()<=HardIndex) return maxLength;
	  if(HardwareList[HardIndex].WindowsList.size()<=0) return maxLength;

	  for(i=0;i<FrameContainer->GetWindowCount();i++)
		  CheckListBox->SetCheck(i,0);
	  for(j=0;j<HardwareList[HardIndex].WindowsList.size();j++)
	  {
		  ind=HardwareList[HardIndex].WindowsList[j];
		  
	      for(i=0;i<FrameContainer->GetWindowCount();i++)
	      {
		     if(ind==FrameContainer->GetFrameIndex(i))
			    CheckListBox->SetCheck(i,1);
	       }
	  }
       return maxLength;
}
bool CGPS::LoadWindowOfGHardToList(int HardIndex,CListBox *ListBox)
{
   	   CString g,result;
	   int FrameIndex;
	   unsigned int i;
	   ListBox->ResetContent();
	   for(i=0;i<GPS->HardwareList[HardIndex].WindowsList.size();i++)
	   {  
		  FrameIndex=GPS->HardwareList[HardIndex].WindowsList[i];
		  g=FrameContainer->GetWindText(FrameIndex);
		  ListBox->AddString(g);
	   }
       return true;
}
//***************************************************************************************************************
THardware* CGPS::GetDrive(int index)
{
	if(index<(int)HardwareList.size())
	  return &HardwareList[index];

	return NULL;
}
//***************************************************************************************************************
bool CGPS::LoadHardwareToList(CListBox* ListBox)
{
  if(ListBox->m_hWnd!=NULL)
  {
   if(this!=NULL)
   {
       unsigned int i;
	   ListBox->ResetContent();
	   int s=HardwareList.size();
	   CString g;
     for(i=0;i<HardwareList.size();i++)
     { 
	     g=HardwareList[i].Name+"("+HardwareList[i].ID+")";
	     ListBox->AddString(g);

     }
	 return true;
   }
  }
   return false;
}
bool CGPS::LoadHardwareToList(CComboBox* ComboBox)
{
   if(this!=NULL)
   {
	     int ind=ComboBox->GetCurSel();  

       unsigned int i;
	   ComboBox->ResetContent();
	   CString g;
     for(i=0;i<HardwareList.size();i++)
     { 
	     g=HardwareList[i].Name+"("+HardwareList[i].ID+")";
	     ComboBox->AddString(g);

     }
	 if(ComboBox->GetCount()<1)
		  return false;
	    if(ind<ComboBox->GetCount()&&ind>-1)
          ComboBox->SetCurSel(ind);
       else
          ComboBox->SetCurSel(0);
	 return true;
   }
   return false;
}
//***************************************************************************************************************
bool CGPS::LoadDataOfServ()
{

  CStdioFile f;
  CString FStr;
  f.Open(RES_PATH+"Client\\Settings.lan",CFile::modeRead);
  f.ReadString(FStr);
  ServIP=FStr;
  f.ReadString(FStr);
  Port=atoi((LPCSTR)FStr);
  return true;
}
//***************************************************************************************************************
bool CGPS::ConnectByServer()
{
    // Конфигурация DLL версии 2.1
    if (WSAStartup(MAKEWORD(2,1), &WSAData) != 0)
	{
		//MessageBox("ChoosePixelFormat failed");
		MessageBox(MainForm->m_hWnd,"ОШИБКА: Невозможно инициализировать библиотеку сокетов!","Error!",0);
		return false;
	}
// Создаем сокет
 WinSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 if (WinSocket == SOCKET_ERROR)
 {
		MessageBox(MainForm->m_hWnd,"ОШИБКА: Невозможно создать сокет!","Error!",0);
		return false;
 }
   // Заполняем поля структуры для сокета
 SockStruct.sin_family = AF_INET;
 SockStruct.sin_port = Port;
 SockStruct.sin_addr.s_addr = inet_addr((LPCSTR)ServIP);
// ListBox->AddString("Подключение...");
         // Подключаемся к сокету
 Connect = connect(WinSocket, (sockaddr*)&SockStruct, sizeof(SockStruct));
 if (Connect == SOCKET_ERROR)
 {
        MessageBox(MainForm->m_hWnd,"ОШИБКА: Невозможно подключиться к серверу!","Error",0);
        return false;
 }
 connected=true;
 MainForm->Menu.GPSConnect=true;
 //send(WinSocket, (LPCSTR)g,g.GetLength(), 0);
 return true;
}
//***************************************************************************************************************
void CGPS::StartGPS()
{
  LoadDataOfServ();                                 //загрузили инфу подкл-я(порт и ip-адрес сервера)
  if(ConnectByServer()==false){StopGPS();return;}   //соеденились с сервером
  SendHardwareList();                               //отсылаем список устройств
  GPSThread=new CGPSThread(&WinSocket);             //размещаем поток в памяти
  GPSThread->CreateThread();                        //создаем поток приема сообщений от сервера
}
//***************************************************************************************************************
void CGPS::StopGPS()
{
	if( connected==true)
	{
		connected=false;
		MainForm->Menu.GPSConnect=false;
		if(GPSDlgAdd->m_hWnd!=NULL)
		  GPSDlgAdd->EndDialog(1);
		if(GPSDlgDel->m_hWnd!=NULL)
		  GPSDlgDel->EndDialog(1);
		GPSThread->Terminate();
		
		closesocket(WinSocket);
		WinSocket=NULL;
		WaitForSingleObject(*GPSThread,3000);
		GPSThread=NULL;
	}
    connected=false;
}
//***************************************************************************************************************
bool CGPS::IsConnected()
{
  return connected;
}
SOCKET* CGPS::GetSocket()
{
	return (&WinSocket);
}
void CGPS::AddGPSHardware(THardware hard)
{
		HardwareList.push_back(hard);
 	if(MainForm->Menu.HardView==true)
	  HardToolFrame->AddToDriveList(HardwareList.size()-1);
	
	if(IsConnected()==true)
    {
		CString Protokol;
		Protokol=HardwareToProtocol(&hard,false,false);
		if(Protokol!="")
		   send(WinSocket,(LPCSTR)Protokol,Protokol.GetLength()+1,0);
	}
}
//***************************************************************************************************************
void CGPS::UpdateHardwareList(int DelIndex)
{
	//если окно закрыто, удаляем его у всех устройств, инфа которых выводилась в него 
	unsigned int i,j;
	for(i=0;i<HardwareList.size();i++)
	{
		for(j=0;j<HardwareList[i].WindowsList.size();j++)
	       if(HardwareList[i].WindowsList[j]==DelIndex)
		   {
			   HardwareList[i].WindowsList.erase(HardwareList[i].WindowsList.begin()+j);
			   
		   }
       HardToolFrame->UpdateWindowList(i);
	}
}