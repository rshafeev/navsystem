#include "StdAfx.h"
#include "Shader.h"

#include <iostream>
#include <string.h>
#include <windows.h>


#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")
//**************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CShader::CShader(void)
{
	createdFlag=false;
}

CShader::~CShader(void)
{
  if(createdFlag==true)
	  DestroyProgram();
}
//=========================================================================================================================
GLuint  CShader::GetShadPt()
{
   return ShadPtr;
}
//=========================================================================================================================
void CShader::Use()
{
	glUseProgram(ShadPtr);
}
void CShader::Close()
{
   glUseProgram(0);
}
//=========================================================================================================================
char* CShader::textFileRead(char *fn)
{
	FILE *fp;
	char *content = NULL;
 
	int count=0;
 
	if (fn != NULL) {
		fp = fopen(fn,"rt");
 
		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
	  content=new char[count+3];
      rewind(fp);
      fclose(fp);
	  fp = fopen(fn,"rt");
			if (count > 0) {
				count = fread(content,sizeof(char),count-1,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}
//=========================================================================================================================
int CShader::textFileWrite(char *fn, char *s) 
{
	FILE *fp;
	int status = 0;
 
	if (fn != NULL) {
		fp = fopen(fn,"w");
 
		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}
//=========================================================================================================================
void CShader::setShaders(char * fileV,char *fileF)
{
	char *vs = NULL,*fs = NULL,*fs2 = NULL;
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	
	vs = textFileRead(fileV);
	fs = textFileRead(fileF);
	// Копируем код шейдеров
	const char * vv = vs;
	const char * ff = fs;

	// Передаём шейдерам их исходный код
	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);
 
	// Освобождаем память под ненужные теперь массивы
	free(vs);free(fs);
 
	// Компилируем шейдеры
	glCompileShader(v);
	glCompileShader(f);
 
	// Создаём программу
	ShadPtr = glCreateProgram();
	// Присоединяем к программе шейдеры
	glAttachShader(ShadPtr,v);
	glAttachShader(ShadPtr,f);
 
	// Линкуем программу
	glLinkProgram(ShadPtr);
	createdFlag=true;
 
}
//=========================================================================================================================
void CShader::LoadShader(CString VertFName, CString FragFName)
{
	glewInit();
	setShaders(VertFName.GetBuffer(),FragFName.GetBuffer());
	
}
//=========================================================================================================================
void CShader::DestroyProgram()
{
	glDeleteObjectARB(f);
	glDeleteObjectARB(v);
	glDeleteObjectARB(ShadPtr);
	createdFlag=false;
}
//=========================================================================================================================
bool CShader::IsCreated()
{
   return createdFlag;
}
//=========================================================================================================================
bool CShader::SetUniformFloat(const char* name, float value)
{
	int loc = glGetUniformLocation(ShadPtr, name);
  if (loc < 0)
     return false;
  glUniform1f(loc, value);
  
  return true;
}
//=========================================================================================================================
bool CShader::SetUniformInt(const char* name, int value)
{ 
  int loc = glGetUniformLocation(ShadPtr, name);
  if (loc < 0)
    return false;
  glUniform1i(loc, value);
return true;
}
//=========================================================================================================================
bool CShader::SetUniformVec2(const char *name, float value1, float value2)
{
  int loc = glGetUniformLocation(ShadPtr, name);
  if (loc < 0)
    return false;
  glUniform2f(loc, value1,value2);
return true;

}
//=========================================================================================================================