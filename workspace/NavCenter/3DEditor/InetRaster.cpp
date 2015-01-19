#include "StdAfx.h"
#include "InetRaster.h"

#include "afxwin.h "
#include "3DEditor.h"

СInetRaster::СInetRaster()
{
	
	ServerUrl="http://kh.google.com/kh?v=3&t=";
	FolderCache=RES_PATH+"cache\\";
}

СInetRaster::~СInetRaster(void)
{

}
//==========================================================================================================================
void СInetRaster::Connect()
{


}
//==========================================================================================================================
BOOL СInetRaster::InitInstance()
{
	SetThreadPriority(THREAD_PRIORITY_LOWEST); 

    return true;
}
//==========================================================================================================================
int СInetRaster::Run()
{
	  srand(static_cast<unsigned int>(time(0)));
	unsigned int i=0;
	while(1)
	{
		
      if(*Terminated==true)
		  break;
/*	  int count=*LoadedCount;
      
	  if(*LoadedCount>4000)
	  {
            int s;
			s=33;
	  }
	  for(i=0;i<count;i++)
	  {
          if(*Terminated==true)
		  break;
            if(*LoadedCount==0)
				break;
			Sleep(400+rand()*1400/RAND_MAX);
			//Sleep(1000);
		  if(LoadedContainer->size()>0)
		  if(LoadFileFromInet(LoadedContainer->at(count-i-1).FName)==0)
		  { 
            // если загрузка файла прошла успешно, выводим на экран
			  if((count-i-1>=0)&&(count-i-1<5000)&&(LoadedContainer->size()>0))
        	      Draw(LoadedContainer->at(count-i-1));
			  
			  if((count-i-1>=0)&&(count-i-1<5000)&&(LoadedContainer->size()>0))
			  {
				  if(*LoadedCount==0)
					  break;
			      LoadedContainer->erase(LoadedContainer->begin()+count-i-1);
				  LoadedContainer->resize(5000);
			  }
			  else
			     break;
			 if(*LoadedCount==0)
				 break;
		     (*LoadedCount)--;//!!!
		     count--;
		  }
		 
	  }*/
	}
	return WM_QUIT;
}
void СInetRaster::Create(CBlockRaster *BlockRaster,bool *TerminatedThread,vector<TLoadedFiles>* LoadedContainer,
						 int *LoadedCount,CWnd *pFrame,int UnicWindIndex,CMutex *Mutex)
{
	this->BlockRaster=BlockRaster;
	this->Terminated=TerminatedThread;
	this->LoadedContainer=LoadedContainer;
	this->LoadedCount=LoadedCount;
	this->pFrame=pFrame;
	this->UnicWindIndex=UnicWindIndex;
	this->InetMutex=Mutex;
	LoadedContainer->resize(5000);
}
//==========================================================================================================================
int СInetRaster::LoadFileFromInet(CString FNameWithoutExp)
{
CString m_mes; // переменная в которой будут хранится сообщения
char temp[100]; // промежуточная переменная для перевода 
// данных из Int в char
CString m_path=FolderCache+FNameWithoutExp+".jpg"; // имя файла для записи
char strBody[1024]; // буфер из 1024 байт 
CString m_url=ServerUrl+FNameWithoutExp;


// создаём переменную session и открываем сессию ANDY
CInternetSession *session=new CInternetSession(_T( "ANDY" ), PRE_CONFIG_INTERNET_ACCESS );

// создаём переменную pServer класса CHttpConnection
CHttpConnection* pServer = NULL;

// создаём переменную pFile класса CHttpFile
CHttpFile* pFile = NULL;

try
{
CString strServerName; // имя сервера
CString strObject; // имя объекта 
INTERNET_PORT nPort; // номер порта для связи
DWORD dwServiceType; // тип сервиса

// функция AfxParseURL получает данные с указанного URL ( у нас m_url ) об сервере,
// объекте, типе сервиса и порте 

if ( AfxParseURL( m_url, dwServiceType, strServerName, strObject, nPort ) == 0 )
{
return -1; // выход из функции OnButtonConnect() 
}
// вывод данных о сервере
/*
m_mes = "";
m_mes += "Server Name = "; 
m_mes += (CString)strServerName; m_mes += "rn";
m_mes += "Object Name = ";
m_mes += (CString)strObject; m_mes += "rn";
m_mes += "Port = ";
itoa( nPort, temp, 10 );
m_mes += (CString)&temp[0]; m_mes += "rn";
*/

// Устанавливаем подключение по HTTP протоклолу. 
pServer = session->GetHttpConnection( strServerName, nPort );

// посылаем запрос об объекте ( strObject )
pFile = pServer->OpenRequest(
CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL,
INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT
);

// Добавляем заголовок к HTTP запросу 
pFile->AddRequestHeaders( _T( "Accept: */*rnUser-Agent: ANDYrn" ) );

// посылаем запрос
pFile->SendRequest( );

DWORD dwRet; // переменная для хранения кода состояния 
pFile->QueryInfoStatusCode( dwRet ); // записываем код состояния в dwRet

// вывод данных
m_mes += "The HTTP GET returned a status code of ";
itoa( dwRet, temp, 10 ); 
m_mes += (CString)&temp[0]; m_mes += "rn"; 

CString strHeader; // переменная для хранения полученного заголовока запроса
pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strHeader); // записываем заголовок в strHeader

// вывод данных
m_mes += "Header = ";
m_mes += strHeader; 
//UpdateData( FALSE );

// если код состояния не равен 200, то выходим из функции
if( dwRet != 200 ) { m_mes += "Program terminate!";return -1; }
// ----------------------------------------------------------

// проверка выбора файла для записи 
/*
m_mes += "Starting download the file."; m_mes += "rn";

if( m_path == "" ) 
{ 
m_mes += "Error! No file to save. Choese the file.";
m_mes += "rn"; UpdateData( FALSE ); return 1; }
else 
{
m_mes += "File name to save : ";
m_mes += m_path; m_mes += "rn"; UpdateData( FALSE ); 
}
*/
CFile file2; // объявляем переменную file2 класса CFile

// открываем файл для записи в двоичном формате ( CFile::typeBinary ) !!!
file2.Open((LPCTSTR)m_path,
CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

int allRead = 0; // переменная для хранения общего числи считанных байт
int nRead = pFile->Read( strBody, 1024 ); // считываем первые 1024 байта в буфер.
// переменная nRead хранит количество
// считанных байт 

allRead += nRead; // обновляем общее число считанных байт

// вывод данных
m_mes += "Loading ";
itoa( nRead, temp, 10 );
m_mes += (CString)&temp[0]; m_mes += " bytes"; m_mes += "rn";
//UpdateData( FALSE );

// записываем буфер из nRead байт в файл
file2.Write( strBody, nRead ); 

// цикл считывания, пока nRead не будет равняться нулю
while ( nRead > 0 )
{
nRead = pFile->Read( strBody, 1024 );

if( nRead != 0 ) 
{
m_mes += "Loading ";
itoa( nRead, temp, 10 );
m_mes += (CString)&temp[0]; m_mes += " bytes"; m_mes += "rn";

file2.Write( strBody, nRead );
allRead += nRead;

}

}
// вывод данных
m_mes += "rn";
m_mes += "Total bytes = ";
itoa( allRead, temp, 10 );
m_mes += &temp[0]; m_mes += "rn";
//UpdateData( FALSE );
file2.Close(); // закрываем файл
pFile->Close(); // закрываем Internet файл
pServer->Close(); // закрываем сервер
m_mes += "Download is complete !!!"; m_mes += "rn";
//UpdateData( FALSE );
}
catch ( CInternetException* pEx )
{
// Если произошла ошибка в WinInet

// вывод ошибки
char szErr[1024];
pEx->GetErrorMessage( szErr, 1024 );
m_mes += "Error: ( ";
itoa( int(pEx->m_dwError), temp ,10 );
m_mes += (CString)&temp[0];
m_mes += " ) ";
m_mes += (CString)&szErr[0]; m_mes += "rn";

pEx->Delete( ); // удаление переменной класса CInternetException 
DeleteFile(m_path);//!!!!!!!!!!!!!!
if ( pFile != NULL )
delete pFile; // закрываем Internet файл
if ( pServer != NULL )
delete pServer; // закрываем сервер
session->Close( ); // закрываем сессию
return -1;
}

if ( pFile != NULL )
delete pFile; // закрываем Internet файл
if ( pServer != NULL )
delete pServer; // закрываем сервер
session->Close( ); // закрываем сессию

return 0;
}
//==========================================================================================================================
void СInetRaster::Draw(TLoadedFiles LoadedFile)
{
   
 /*  CPoint LeftTop=BlockRaster->GetLeftTopBlock();
   CPoint DrawRect=BlockRaster->GetColOfBlocks();*/
 //  BlockRaster->AddImage(LoadedFile);
 /*  BlockRaster->PushMatrix();
	if(pFrame->m_hWnd!=NULL)
		FrameContainer->GetFramePtr(UnicWindIndex)->Repaint();*/
}
//==========================================================================================================================