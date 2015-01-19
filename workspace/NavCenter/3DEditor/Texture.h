#pragma once
#include "afx.h"
#include "3dLib.h"
class CTexture
{
	 AUX_RGBImageRec *texture;                                             // Загрузка картинки
	 CString FileName;
	 GLuint	texPtr;
	 GLvoid LoadGLTextures();
	 bool createdFlag;          //была ли загружена текстура
public:
	void    Create(AUX_RGBImageRec *texture); //создает объект по уже созданному texture 
	void    InitTexture(CString FileName);
	GLuint  GetTexPt();
	bool    IsCreated();
	CString GetTexFileName();
	int     PictWigth();
	int     PictHeight();
	void    Free();
	CTexture(void);
	~CTexture(void);
};
