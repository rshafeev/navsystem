#pragma once
#include "afx.h"
#include "3dLib.h"

class CShader
{
 char* textFileRead(char *fn);                                //читает файл  fn и возвращает прочитанные данные
 int   textFileWrite(char *fn, char *s);                      //записывает в файл fn строку s
 void  setShaders(char * fileV,char *fileF);                  //создает, компилит шейдерную программу 
 GLuint v,f,ShadPtr;
 bool  createdFlag;                                           //была ли создана программа?
public:
	void    LoadShader(CString VertFName,CString FragFName);  //загрузка шейдера
	GLuint  GetShadPt();                                      //получить дескриптор объекта шейдера
	void    Use();                                            //использовать шейдер
	void    Close();                                          //вернуть стандартный шейдер
    void    DestroyProgram();                                 //удалить программу
	bool    IsCreated();                                      //возвращает createdFlag
	bool    SetUniformFloat(const char* name, float value);   //послать  переменной шейдера name значение  value
	bool    SetUniformInt  (const char* name, int value);     //послать  переменной шейдера name значение  value
	bool    SetUniformVec2 (const char* name,float value1,float value2);
	CShader(void);
	~CShader(void);
};
//=========================================================================================================================

