#include "stdafx.h"
#include "TrakerClientHandler.h"
#include "protocols.h"
#include "NavServerApp.h"

CTrakerClientHandler::CTrakerClientHandler(ClientConnection& _client)
:CAbstractClientHandler(_client, ClientType::TRACKER)
{
}


CTrakerClientHandler::~CTrakerClientHandler()
{
}

void CTrakerClientHandler::receiveMassege(CString msg)
{
	//@#<ID>#<Lo>#<De>#<Height>#;
	int i;
	CTime time;
	TGPSPoint point;
	CString buf;
	CString TimeNow;
	time = CTime::GetCurrentTime();
	TimeNow.Format("%d.%d.%d %d:%d:%d", time.GetDay(), time.GetMonth(), time.GetYear(),
		time.GetHour(), time.GetMinute(), time.GetSecond());
	i = 0;
	while (msg.GetLength() != 0 && i < msg.GetLength()) //в одном протоколе может быть несколько запросов(разелены ";")
	{
		i++;
		//загружаем строку в структуру

		msg.SpanExcluding(";");
		msg.Delete(0, 2);                   //<ID>#<Lo>#<De>#<Height>#;

		//считываем ID
		buf = msg.SpanExcluding("#");
		point.IMEI = buf;
		msg.Delete(0, buf.GetLength() + 1);  //<Lo>#<De>#<Height>#;

		//считываем Lo
		buf = msg.SpanExcluding("#");
		point.Longitude = buf;
		msg.Delete(0, buf.GetLength() + 1);  //<De>#<Height>#;

		//считываем De
		buf = msg.SpanExcluding("#");
		point.Description = buf;
		msg.Delete(0, buf.GetLength() + 1);  //<Height>#;

		//считываем Height
		buf = msg.SpanExcluding(";");
		point.Height = buf;
		msg.Delete(0, buf.GetLength() + 1);  //<Height>#;

		point.DateTime = TimeNow;
		msg.Delete(0, msg.SpanExcluding(";").GetLength() + 1);


		//отправляем координаты всем пользователям, стоящих в очереди к этому устройству

		vector<CDispatcherClientHandler*> clients = theApp.ServerManager().TrackersMap().findDiscepthersListByTracker(this->IMEI);
		for (unsigned int i = 0; i < clients.size(); i++)
		{
			clients[i]->sendMessage(point);
		}
		//заносим данные в БД
		//writeDB(Info);
		// CDBWork::writeDB(Info);
	}
}