#include "StdAfx.h"
#include "Memory.h"


void* CMemoryMappedFile::GetMemoryMapping(bool bReadOnly)
{
	// функция проецирует файл на память (файл должен быть уже открыт)
// и возвращает стартовый адрес блока памяти
     if(!m_hFile || m_hMapping) return NULL;
     m_hMapping = CreateFileMapping((void*)m_hFile, 0, bReadOnly?PAGE_READONLY:PAGE_READWRITE, 0, 0, 0);
     if(!m_hMapping) return NULL;
     m_bufMapping = MapViewOfFile(m_hMapping, FILE_MAP_READ|(bReadOnly?0:FILE_MAP_WRITE), 0, 0, 0);
     if(!m_bufMapping) CloseHandle(m_hMapping);
     return m_bufMapping;
}



void CMemoryMappedFile::ReleaseMemoryMapping()
{
	// освобождение проекции
     if(!m_hMapping) return;
     UnmapViewOfFile(m_bufMapping);
     CloseHandle(m_hMapping);
     m_hMapping = NULL;
}
void CMemoryMappedFile::FreeMem()
{
    UnmapViewOfFile(m_bufMapping);
 
}
// ===============================================
