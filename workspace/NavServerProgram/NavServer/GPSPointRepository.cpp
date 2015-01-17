#include "stdafx.h"
#include "GPSPointRepository.h"
#include "NavServer.h"

GPSPointRepository::GPSPointRepository(void)
{
}

GPSPointRepository::~GPSPointRepository(void)
{
}

void GPSPointRepository::insert(TGPSPoint &Add)
{
	TRY_ADO()//макрос обработки исключений
		CADORecordset rs;
	rs.SetConnection(&theApp.m_connection);//передали указатель объекта CADOConnection
	//sql-запрос, задаем условие отбора данных и упорядочиваем их по полю Hardware(уникальный номер устройства)       
	rs.SetSQL("select num_rec,[Hardware],[HardCaption], [Longitude], [Description],[Height],[Date] from DATA order by [Hardware]");
	rs.SetOpenMode(true);// задаем динамичечкий курсор
	rs.Open();           // открываем доступ к данным
	if (!rs.IsEmpty())   // есть ли записи в БД?
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
		rs.Update();    //сохраняем изменения
	}
	catch (CADOException&)
	{
		//в случае ошибки отменяем изменения
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

void GPSPointRepository::clearDB()
{
	TRY_ADO()
		CADORecordset rs;
	rs.SetConnection(&theApp.m_connection);
	rs.SetSQL("DELETE FROM DATA WHERE Hardware = '234324235' ");
	rs.Open();
	CATCH_ADO()
}
