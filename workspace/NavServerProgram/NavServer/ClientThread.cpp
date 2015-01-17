#include "StdAfx.h"
#include "ClientThread.h"
#include <vector>
#include <winsock2.h>
#include "NavServer.h"

using namespace std;
//const int ClientThread::SIZE=2048;
//const int MAXELEM=2000;



ClientThread::ClientThread(CListBox *ListBox,User *user,int UserIndex)
{
	this->ListBox=ListBox;
	this->user=user;
	m_bAutoDelete=true;
	this->userIndex=UserIndex;
	Terminated=false;
	IP=inet_ntoa(user->AcpStruct.sin_addr);
	port=user->AcpStruct.sin_port;
}

ClientThread::~ClientThread(void)
{
}
//==========================================================================================================
void ClientThread::Terminate()
{
   Terminated=true;
}
//==========================================================================================================
bool ClientThread::SendGPSPoint(int *SOCKET,TGPSPoint GPSPoint)
{

 if(SOCKET!=NULL&&*SOCKET!=NULL)
 {
	 CString SendStr;//@#<ID>#<Lo>#<De>#<Height>#<Time>;
	 SendStr="@#"+GPSPoint.IMEI+"#"+GPSPoint.Longitude+"#"+GPSPoint.Description+"#"+
		     GPSPoint.Height+"#"+GPSPoint.DateTime+";";
	 if(send(*SOCKET,(LPCSTR)SendStr,SendStr.GetLength(),0)==S_OK){ return false;}
	 return true;

 }
// terminate();
 return false;
}
//==========================================================================================================
void ClientThread::LoadDataFromDB(int *Accept,TEditorProtocol EditorProtocol)
{
   	  CString sqlStr;
      int nItem = 0;     
	  TGPSPoint SendInfo;
	  
	TRY_ADO()
       
      CADORecordset rs;
      rs.SetConnection (& theApp.m_connection);

	  sqlStr="select num_rec,[Hardware],[HardCaption], [Longitude], [Description],[Height],[Date] from DATA";
	  sqlStr+=" WHERE Hardware = '"+EditorProtocol.IMEI+"' ";
	 
	  rs.SetSQL((LPCSTR)sqlStr);
      rs.SetOpenMode (true);
      rs.Open ();
	  rs.First();
      
      try
         {

            if (!rs.IsEmpty ())
            for (rs.First () ; !rs.IsEoF () ; rs.Next ())
            {
				
			  SendInfo.IMEI       =(const char*)_bstr_t(rs.GetField("Hardware"));
			  SendInfo.Description=(const char*)_bstr_t(rs.GetField("Description"));
			  SendInfo.Longitude  =(const char*)_bstr_t(rs.GetField("Longitude"));
			  SendInfo.Height     =(const char*)_bstr_t(rs.GetField("Height"));
			  SendInfo.DateTime   =(const char*)_bstr_t(rs.GetField("Date"));
              
              
			  if(SendGPSPoint(Accept,SendInfo)==false) break;
			  nItem++;
			  if(Terminated==true) break;
            }   
         }
      catch (CADOException&)
         {
         try
            {
            rs.Cancel ();
            }
         catch (...)
            {
            }
         throw;
         }
	  rs.Close();
     CATCH_ADO()
	

 /////////
}
//============================================================================================================================
void ClientThread::AddToMass(CString ID,int indexUser)
{
   unsigned int i,j;
  

   for(i=0;i<RunTimeUsers.size();i++)
   {
	   if(RunTimeUsers[i].IMEI==ID)
	   {
		   for(j=0;j<RunTimeUsers[i].users.size();j++)
			   if(RunTimeUsers[i].users[j]==indexUser)// провер€ем, может такой запрос уже поступал от этого клиента
				   return;
		   RunTimeUsers[i].users.push_back(indexUser);
		   return;
	   }
   }
  TRunTimeUsers add;
   add.IMEI=ID;
   add.users.push_back(indexUser);
   RunTimeUsers.push_back(add);

}
//============================================================================================================================
void ClientThread::DelteFromMass(CString ID,int indexUser)
{
   unsigned int i,j;

   for(i=0;i<RunTimeUsers.size();i++)
   {
	   if(RunTimeUsers[i].IMEI==ID)
	   {
		   for(j=0;j<RunTimeUsers[i].users.size();j++)
		   {
			   if(RunTimeUsers[i].users[j]==indexUser)
			   {
				   RunTimeUsers[i].users.erase(RunTimeUsers[i].users.begin()+j);
				   return;
			   }
		   }
	   }
   }
}
//============================================================================================================================
void ClientThread::EditorMessage(CString protocol)
{
   TEditorProtocol Prl;
   CString buf;
   
   while(protocol.GetLength()!=0) //в одном протоколе может быть несколько запросов(разелены ";")
   {
    buf=protocol.SpanExcluding(";");
	  	//считываем ID
	protocol.Delete(0,2);                    //<ID>#<t1>#<t2>#0;
	buf=protocol.SpanExcluding("#");
	Prl.IMEI=buf;
	protocol.Delete(0,buf.GetLength()+1);      //<t1>#<t2>#0;
	//считываем time1
	buf=protocol.SpanExcluding("#");
	Prl.DTime1=buf;
	protocol.Delete(0,buf.GetLength()+1);      //#<t2>#0;
	//считываем time2
	buf=protocol.SpanExcluding("#");
	Prl.DTime2=buf;
	protocol.Delete(0,buf.GetLength()+1);      //#0;
    //считываем посл. сивол-идентификатор
	buf=protocol.SpanExcluding(";");
	Prl.type=atoi(buf);
	if(Prl.DTime1=="."||Prl.DTime2==".")
		Prl.RunTime=true;
	else
		Prl.RunTime=false;

	if(Prl.type==1)  //если запрос на остановку рассылки от данного устройства
	{
		DelteFromMass(Prl.IMEI,userIndex);
	}
	else
	{
		if(Prl.RunTime==1)         // если в реальном времени
		{
			AddToMass(Prl.IMEI,userIndex);
		}
		else                           // высылаем данные с Ѕƒ
		{
			//LoadDataFromDB(&(user->Accept),Prl);
		}
	}
	protocol.Delete(0,protocol.SpanExcluding(";").GetLength()+1);
   }
}
//============================================================================================================================
void ClientThread::MobileMessage(CString protocol)
{
	//@#<ID>#<Lo>#<De>#<Height>#;
   int i;
   CTime time;
   TGPSPoint Info;
   CString buf;
   CString TimeNow;
   time=CTime::GetCurrentTime();
   TimeNow.Format("%d.%d.%d %d:%d:%d",time.GetDay(),time.GetMonth(),time.GetYear(),
	                                  time.GetHour(),time.GetMinute(),time.GetSecond());
   i=0;
   while(protocol.GetLength()!=0&&i<protocol.GetLength()) //в одном протоколе может быть несколько запросов(разелены ";")
   {
	   i++;
//загружаем строку в структуру
	  
    buf=protocol.SpanExcluding(";");
	protocol.Delete(0,2);                   //<ID>#<Lo>#<De>#<Height>#;

	//считываем ID
	buf=protocol.SpanExcluding("#");
	Info.IMEI=buf;
	protocol.Delete(0,buf.GetLength()+1);  //<Lo>#<De>#<Height>#;

	//считываем Lo
	buf=protocol.SpanExcluding("#");
	Info.Longitude=buf;
	protocol.Delete(0,buf.GetLength()+1);  //<De>#<Height>#;

	//считываем De
	buf=protocol.SpanExcluding("#");
	Info.Description=buf;
	protocol.Delete(0,buf.GetLength()+1);  //<Height>#;

	//считываем Height
	buf=protocol.SpanExcluding(";");
	Info.Height=buf;
	protocol.Delete(0,buf.GetLength()+1);  //<Height>#;
    
	Info.DateTime=TimeNow;
  protocol.Delete(0,protocol.SpanExcluding(";").GetLength()+1);


//отправл€ем координаты всем пользовател€м, сто€щих в очереди к этому устройству
  CString Message;
  int ind;
  unsigned int i,j;

  Message.Format("@#%s#%s#%s#%s#%s;",Info.IMEI,Info.Longitude,Info.Description,Info.Height,Info.DateTime);
  for(i=0;i<RunTimeUsers.size();i++)
  {
	 if(RunTimeUsers[i].IMEI==Info.IMEI)//ищем наше устройство
	 {
		 for(j=0;j<RunTimeUsers[i].users.size();j++)
		 {
           ind=RunTimeUsers[i].users[j];
		   send(users[ind].Accept,(LPCSTR)Message,Message.GetLength(),0);
		 }
	 }
  }
  //заносим данные в Ѕƒ
  //writeDB(Info);
  // CDBWork::writeDB(Info);
  
}	
}
//============================================================================================================================
int ClientThread::Run()
{
  char Buffer[20000]; // Ѕуфер вход€щих данных
  CString f;
 while (true)                                  // «апускаем бесконечный цикл приема сообщений
 {
	 for (int i=0; i<SIZE; i++)                // ƒелаем так, чтобы не было мусора во вход€щих данных
      Buffer[i] = '\0';
    Recv = recv(user->Accept,Buffer, SIZE, 0);  // ѕолучаем данные
    f=CString(Buffer);
	if(Terminated==true){Stop(); break;}
 	if (Recv == SOCKET_ERROR||Recv == 0)
	  {
		  CString str;
	         str.Format(" лиент отключилс€.  IP: %s ѕорт: %d",IP,port);
				   ListBox->AddString((LPCSTR) str);
		  Stop();
		  PostQuitMessage(WM_QUIT);
		  break;
	  }
 	else
	if(f!="")
	{
	  if(f[0]=='$')                              //клиент Navagation GIS
         EditorMessage(f);
	  else
	  if(f[0]=='@')                              //клиент GPS(абонент)
		  MobileMessage(f);
 	  //f.Insert(0,">>  ");
	  //ListBox->AddString(f);
   	}
}
return WM_QUIT;
}
//============================================================================================================================
BOOL  ClientThread::InitInstance()
{
	SetThreadPriority(THREAD_PRIORITY_LOWEST);
	m_bAutoDelete=true;

	/*
      rs.SetConnection (&theApp.m_connection);//передали указатель объекта CADOConnection
      //sql-запрос, задаем условие отбора данных и упор€дочиваем их по полю Hardware(уникальный номер устройства)       
	  rs.SetSQL ("select num_rec,[Hardware],[HardCaption], [Longitude], [Description],[Height],[Date] from DATA order by [Hardware]");
      rs.SetOpenMode (true);// задаем динамичечкий курсор
      rs.Open ();           // открываем доступ к данным
  
*/


   return true;   
}
//============================================================================================================================
void ClientThread::Delete()
{
	unsigned int i,j;
	for(i=0;i<RunTimeUsers.size();i++)
		  {
			  for(j=0;j<RunTimeUsers[i].users.size();j++) 
				  if(RunTimeUsers[i].users[j]==userIndex)
					  RunTimeUsers[i].users.erase(RunTimeUsers[i].users.begin()+j);
		  }
}
//============================================================================================================================
void ClientThread::Stop()
{
		  
	      user->active = false;
		  Delete();
		  closesocket(user->Accept);
		  user->Accept=NULL;
		  Terminated=true;

		  
}
//======================================================================================================================
void ClientThread::writeDB(TGPSPoint &Add)
{
	if(Add.DateTime==""||Add.Description==""||Add.Height==""||Add.IMEI==""||Add.Longitude=="")
      return;

   TRY_ADO()//макрос обработки исключений
      if (!rs.IsEmpty ())   // есть ли записи в Ѕƒ?
         rs.Last ();        // если да, переходим в конец
      rs.AddNew ();         // посылаем запрос, о том, что мы хотим добавить данные
      try
         {
			 //отсылаем данные
			 rs.SetField ("Hardware", _variant_t(CT2CA((LPCTSTR)Add.IMEI)));
			 rs.SetField ("HardCaption", _variant_t(CT2CA((LPCTSTR)Add.IMEI)));
			 rs.SetField ("Longitude", _variant_t(CT2CA((LPCTSTR)Add.Longitude)));
			 rs.SetField ("Description", _variant_t(CT2CA((LPCTSTR)Add.Description)));
			 rs.SetField ("Height", _variant_t(CT2CA((LPCTSTR)Add.Height)));
			 rs.SetField ("Date", _variant_t(CT2CA((LPCSTR)Add.DateTime)));
           rs.Update ();    //сохран€ем изменени€
         }
      catch (CADOException&)
         {
		 //в случае ошибки отмен€ем изменени€
         try
            {
            rs.Cancel ();
            }
         catch (...)
            {
            }
         throw;
         }

  CATCH_ADO()
}
//======================================================================================================================
int ClientThread::ExitInstance()
{
	  //rs.Close();//закрываем текущее соединение

	return CWinThread::ExitInstance();
}
