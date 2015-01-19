#include "stdafx.h"
#include "DispatcherClientHandler.h"
#include "protocols.h"

CDispatcherClientHandler::CDispatcherClientHandler(ClientConnection& _client)
:CAbstractClientHandler(_client, ClientType::DISPETCHER)
{
}


CDispatcherClientHandler::~CDispatcherClientHandler()
{
}

void CDispatcherClientHandler::receiveMassege(CString msg)
{
	TDispetcherProtocol protocol;
	CString buf;
	
	while (msg.GetLength() != 0) //в одном протоколе может быть несколько запросов(разелены ";")
	{
		msg.SpanExcluding(";");
		//считываем ID
		msg.Delete(0, 2);                    //<ID>#<t1>#<t2>#0;
		buf = msg.SpanExcluding("#");
		protocol.IMEI = buf;
		msg.Delete(0, buf.GetLength() + 1);      //<t1>#<t2>#0;
		//считываем time1
		buf = msg.SpanExcluding("#");
		protocol.DTime1 = buf;
		msg.Delete(0, buf.GetLength() + 1);      //#<t2>#0;
		//считываем time2
		buf = msg.SpanExcluding("#");
		protocol.DTime2 = buf;
		msg.Delete(0, buf.GetLength() + 1);      //#0;
		//считываем посл. сивол-идентификатор
		buf = msg.SpanExcluding(";");
		protocol.type = atoi(buf);
		if (protocol.DTime1 == "." || protocol.DTime2 == ".")
			protocol.RunTime = true;
		else
			protocol.RunTime = false;

		if (protocol.type == 1)  //если запрос на остановку рассылки от данного устройства
		{
			//DelteFromMass(protocol.IMEI, userIndex);

		}
		else
		{
			if (protocol.RunTime == 1)         // если в реальном времени
			{

				//AddToMass(Prl.IMEI, userIndex);
			}
			else                           // высылаем данные с БД
			{
				//LoadDataFromDB(&(user->Accept),Prl);
			}
		}
		msg.Delete(0, msg.SpanExcluding(";").GetLength() + 1);
	}

}

bool CDispatcherClientHandler::sendMessage(TGPSPoint& location)
{
	CString msgToDispetcher;
	msgToDispetcher.Format("@#%s#%s#%s#%s#%s;", location.IMEI, location.Longitude, location.Description, location.Height, location.DateTime);
	return CAbstractClientHandler::sendMessage(msgToDispetcher);
}

/*
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
void ClientThread::AddToMass(CString ID, int indexUser)
{
	unsigned int i, j;


	for (i = 0; i<RunTimeUsers.size(); i++)
	{
		if (RunTimeUsers[i].IMEI == ID)
		{
			for (j = 0; j<RunTimeUsers[i].users.size(); j++)
			if (RunTimeUsers[i].users[j] == indexUser)// проверяем, может такой запрос уже поступал от этого клиента
				return;
			RunTimeUsers[i].users.push_back(indexUser);
			return;
		}
	}
	TRunTimeUsers add;
	add.IMEI = ID;
	add.users.push_back(indexUser);
	RunTimeUsers.push_back(add);

}
//============================================================================================================================
void ClientThread::DelteFromMass(CString ID, int indexUser)
{
	unsigned int i, j;

	for (i = 0; i<RunTimeUsers.size(); i++)
	{
		if (RunTimeUsers[i].IMEI == ID)
		{
			for (j = 0; j<RunTimeUsers[i].users.size(); j++)
			{
				if (RunTimeUsers[i].users[j] == indexUser)
				{
					RunTimeUsers[i].users.erase(RunTimeUsers[i].users.begin() + j);
					return;
				}
			}
		}
	}
}*/