
#pragma once
#include "afx.h"
/*
 класс для проецирования файла на память + в файле лежат ф-ции работы с файлами 
*/
class CMemoryMappedFile: public CFile
{
protected:
     HANDLE m_hMapping;     // хэндл объекта "проекция файла"
     void *m_bufMapping;      // стартовый адрес проекции

public:   // набор конструкторов
     CMemoryMappedFile()
	 {m_hMapping=NULL;}
     CMemoryMappedFile(char *strFileName, UINT openFlags): CFile(strFileName, openFlags)
	 {m_hMapping=NULL;}
     ~CMemoryMappedFile() {if(m_hMapping) ReleaseMemoryMapping();}


     
public:  // пара функций "спроецировать" / "освободить проекцию"
     void* GetMemoryMapping(bool bReadOnly=false);
     void  ReleaseMemoryMapping();
	 void  FreeMem();
};
//***************************************************************************************************************
//функции обработки пути

static CString GetFileName(CString FullName)
{
	//возвращает имя файла с расширением
		int pos=FullName.ReverseFind('\\');
		FullName.Delete(0,pos+1);
        return FullName;
}
static CString GetPuth(CString FullName)
{ 
	//возвращает путь к файлу
		int pos=FullName.ReverseFind('\\');
		FullName.Delete(pos,FullName.GetLength()-pos);
        return FullName;
}
static CString GetFileNameWithoutExp(CString FullName)  
{
	//возвращает имя файла без расширения
		int pos=FullName.ReverseFind('\\');
		FullName.Delete(0,pos+1);
        pos=FullName.ReverseFind('.');
        FullName.Delete(pos,FullName.GetLength()-pos);
        return FullName;
}

static CString GetExp(CString FullName) 
{
	 //возвращает  расширение
		int pos=FullName.ReverseFind('.');
		FullName.Delete(0,pos+1);
        return FullName;
}
static bool IsFileExists(CString FullName)
{
   //проверяет существование файла на диске
	WIN32_FIND_DATA d;
if(FindFirstFile(FullName,&d)==INVALID_HANDLE_VALUE)
 return false;
return true;
}
static bool IsUsingFile(CString FullName)
{
  //проверяет, используется ли этот файл другой программой или нет
	HANDLE hFile; 
    hFile = CreateFile(FullName,                // name of the write
		               GENERIC_READ,           // open for reading
                       0,                      // do not share
                       NULL,                   // default security
                       OPEN_EXISTING,          // existing
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template
    if (hFile == INVALID_HANDLE_VALUE) 
	{
		CloseHandle(hFile);
       return true;
	}
	CloseHandle(hFile);
  return false;
}
static int GetFileSize(CString FullName)
{
	int size;
	HANDLE hFile; 
    hFile = CreateFile(FullName,                // name of the write
		               GENERIC_READ,           // open for reading
                       0,                      // do not share
                       NULL,                   // default security
                       OPEN_EXISTING,          // existing
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template
	
	size=(int)GetFileSize(hFile,NULL);
	CloseHandle(hFile);
	return size;
}
//функции обработки пути
//***************************************************************************************************************
