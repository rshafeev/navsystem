#include "StdAfx.h"
#include "Texture.h"
#include "memory.h"
CTexture::CTexture(void)
{
	createdFlag=false;
}

CTexture::~CTexture(void)
{
}
//====================================================================================================
CString CTexture::GetTexFileName()
{
   return FileName;
}
//====================================================================================================
void CTexture::Create(AUX_RGBImageRec *texture)
{
  if(texture!=NULL)
	  this->texture=texture;
}
//====================================================================================================
GLvoid CTexture::LoadGLTextures()
{
   texture=auxDIBImageLoadA((LPCSTR)FileName);
 
   glGenTextures(1,&texPtr);                                  // Создание текстуры
  texture->sizeX -= texture->sizeX%2;
  texture->sizeY -= texture->sizeY%2;
     glBindTexture(GL_TEXTURE_2D, texPtr);	
	 gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture->sizeX, texture->sizeY,
                       GL_RGB, GL_UNSIGNED_BYTE, texture->data); 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
     
	 createdFlag=true;
}
//=======================================================================================================
void CTexture::InitTexture(CString FileName)
{
	this->FileName=FileName;
	if(IsFileExists(FileName)==true)
	   LoadGLTextures();
}
//=======================================================================================================
GLuint CTexture::GetTexPt()
{
	return texPtr;
}
//=======================================================================================================
bool CTexture::IsCreated()
{
  return createdFlag;
}
//=======================================================================================================
int CTexture::PictWigth()
{
	if(createdFlag==false)   return 0;
	return (int)texture->sizeX;
}
//=======================================================================================================
int CTexture::PictHeight()
{
	if(createdFlag==false)   return 0;
	return (int)texture->sizeY;
}
//=======================================================================================================
void CTexture::Free()
{
 if(createdFlag==true)
 {
   createdFlag=false;
   FileName="";
   glDeleteTextures(1,&texPtr);
   free(texture);
   texture=NULL;
 }
}
//=======================================================================================================