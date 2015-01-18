#include "stdafx.h"
#include "NavServerApp.h"
#include "LocationsRepository.h"

CLocationsRepository::CLocationsRepository(CAbstractDbConnectionFactory& _connectionFactory)
:connectionFactory(_connectionFactory)
{
}

CLocationsRepository::~CLocationsRepository(void)
{
}

void CLocationsRepository::insert(TGPSPoint &Add)
{
	TRY_ADO()//макрос обработки исключений
	CADORecordset rs;
	
	rs.SetConnection(connectionFactory.getConnection());//передали указатель объекта CADOConnection
	//sql-запрос, задаем условие отбора данных и упор€дочиваем их по полю Hardware(уникальный номер устройства)       
	rs.SetSQL("select num_rec,[Hardware],[HardCaption], [Longitude], [Description],[Height],[Date] from DATA order by [Hardware]");
	rs.SetOpenMode(true);// задаем динамичечкий курсор
	rs.Open();           // открываем доступ к данным
	if (!rs.IsEmpty())   // есть ли записи в Ѕƒ?
		rs.Last();        // если да, переходим в конец
	rs.AddNew();         // посылаем запрос, о том, что мы хотим добавить данные
	try
	{
		//отсылаем данные
		rs.SetField("Hardware", _variant_t(CT2CA((LPCTSTR)Add.IMEI)));
		rs.SetField("HardCaption", _variant_t(CT2CA((LPCTSTR)Add.IMEI)));
		rs.SetField("Longitude", _variant_t(CT2CA((LPCTSTR)Add.Longitude)));
		rs.SetField("Description", _variant_t(CT2CA((LPCTSTR)Add.Description)));
		rs.SetField("Height", _variant_t(CT2CA((LPCTSTR)Add.Height)));
		rs.SetField("Date", _variant_t(CT2CA((LPCSTR)Add.DateTime)));
		rs.Update();    //сохран€ем изменени€
	}
	catch (CADOException&)
	{
		//в случае ошибки отмен€ем изменени€
		try
		{
			rs.Cancel();
		}
		catch (...)
		{
		}
		throw;
	}
	rs.Close();//закрываем текущее соединение
	CATCH_ADO()
}

void CLocationsRepository::clear()
{
	TRY_ADO()
		CADORecordset rs;
	rs.SetConnection(connectionFactory.getConnection());
	rs.SetSQL("DELETE FROM DATA ");
	rs.Open();
	CATCH_ADO()
}

/*
void ClientThread::writeDB(TGPSPoint &Add)
{
	if (Add.DateTime == "" || Add.Description == "" || Add.Height == "" || Add.IMEI == "" || Add.Longitude == "")
		return;

	TRY_ADO()//макрос обработки исключений
	if (!rs.IsEmpty())   // есть ли записи в Ѕƒ?
		rs.Last();        // если да, переходим в конец
	rs.AddNew();         // посылаем запрос, о том, что мы хотим добавить данные
	try
	{
		//отсылаем данные
		rs.SetField("Hardware", _variant_t(CT2CA((LPCTSTR)Add.IMEI)));
		rs.SetField("HardCaption", _variant_t(CT2CA((LPCTSTR)Add.IMEI)));
		rs.SetField("Longitude", _variant_t(CT2CA((LPCTSTR)Add.Longitude)));
		rs.SetField("Description", _variant_t(CT2CA((LPCTSTR)Add.Description)));
		rs.SetField("Height", _variant_t(CT2CA((LPCTSTR)Add.Height)));
		rs.SetField("Date", _variant_t(CT2CA((LPCSTR)Add.DateTime)));
		rs.Update();    //сохран€ем изменени€
	}
	catch (CADOException&)
	{
		//в случае ошибки отмен€ем изменени€
		try
		{
			rs.Cancel();
		}
		catch (...)
		{
		}
		throw;
	}

	CATCH_ADO()
}*/