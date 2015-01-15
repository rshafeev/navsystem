#include "Landshaft.h"


CLandshaft::CLandshaft(TLandProperties prop)
{
    InitialError=false;
	LandInfo=prop;
	MassHeight=NULL;
	MassTexture=NULL;
    MassVNormal=NULL;
	LandInfo.DH=LandInfo.height/LandInfo.StepSize;
    LandInfo.DW=LandInfo.width/LandInfo.StepSize;
	GlModelList=3001;
	InitialError=!(InitLandshaft(prop.LandFile,prop.TexFile));
    if(InitialError==true)InitFlag=false;
	else
        InitFlag=true;
	
}
CLandshaft::CLandshaft(void)
{
}
CLandshaft::~CLandshaft(void)
{
  /*if(MassHeight!=NULL)	  
  {
	  for(int i=0; i<LandInfo.height;i++)
	    if(MassHeight[i]!=NULL)
	   	  delete[] MassHeight[i];
		else break;
  }
  if(MassTexture!=NULL)	  
  {
	  for(int i=0; i<LandInfo.height;i++)
	    if(MassTexture[i]!=NULL)
	   	  delete[] MassTexture[i];
	    else break;
  }
  if(MassVNormal!=NULL)	  
  {
	  for(int i=0; i<LandInfo.height;i++)
	    if(MassVNormal[i]!=NULL)
	   	  delete[] MassVNormal[i];
	    else break;
  }

	if(MassHeight!=NULL)
		delete[] MassHeight;
	if(MassTexture!=NULL)
		delete[] MassTexture;
    if(MassVNormal!=NULL)
		delete[] MassVNormal;*/
}
//***********************************************************************************************************
void CLandshaft::Tailing()
{
  int i,j;
  float matrix[3][3];
  float koef=16;
  int size=3,matrix_size=3;
  		  int k,p,k_i,p_i;
          float new_c;
 matrix[0][0]=1/koef; matrix[0][1]=2/koef; matrix[0][2]=1/koef;
 matrix[1][0]=2/koef; matrix[1][1]=4/koef; matrix[1][2]=2/koef;
 matrix[2][0]=1/koef; matrix[2][1]=2/koef; matrix[2][2]=1/koef;
 for(i=2;i<LandInfo.DH-2;i++)
	 for(j=2;j<LandInfo.DW-2;j++)
	  {

//
          new_c=0;
           for(k=0;k<size;k++)
             for(p=0;p<size;p++)
			 {
                k_i=k-(matrix_size-1)/2;
                p_i=p-(matrix_size-1)/2;
                if(i+k_i<0) k_i=k_i*(-1);
                if(j+p_i<0) p_i=p_i*(-1);
				if(i+k_i >= LandInfo.DH-1)  k_i=k_i*(-1);
				if(j+p_i >= LandInfo.DW-1) p_i=p_i*(-1);
                new_c+=MassHeight[i+k_i][j+p_i]*matrix[k][p];
            }
			MassHeight[i][j]=new_c;

	/*		if(MassHeight[i][j]>MassHeight[i-1][j]&&
				MassHeight[i][j]>MassHeight[i+1][j]&&
				MassHeight[i][j]>MassHeight[i][j-1]&&
				MassHeight[i][j]>MassHeight[i][j+1]&&
				MassHeight[i][j]>MassHeight[i-1][j-1]&&
				MassHeight[i][j]>MassHeight[i-1][j+1]&&
				MassHeight[i][j]>MassHeight[i+1][j-1]&&
				MassHeight[i][j]>MassHeight[i-1][j] 	)
			{
                MassHeight[i][j]=MassHeight[i][j]>MassHeight[i-1][j]+MassHeight[i][j]>MassHeight[i+1][j]+
					             MassHeight[i][j]>MassHeight[i][j-1]+MassHeight[i][j]>MassHeight[i][j+1]+
                                 MassHeight[i][j]>MassHeight[i-1][j-1]+MassHeight[i][j]>MassHeight[i-1][j+1];
                MassHeight[i][j]/=6;
				MassHeight[i][j]+=5;
			}*/
	  }


}
//***********************************************************************************************************
void CLandshaft::ReGenModel()
{
	glDeleteLists(GlModelList,1);
	RenderLand();
}
//***********************************************************************************************************
void CLandshaft::GenVNormals()
{
    int i,j;
	int STEP_SIZE=LandInfo.StepSize;
	TPoint Normals[6];//нормали всех 6 плоскостей
	TPoint Versh[3];// вершины плоскости
	TPoint NormalRes;// усредненная нормаль вершины
	bool **FlagNormalMap= new bool*[LandInfo.DH+1];
	TPoint **PloskNormals= new TPoint*[LandInfo.DH+1];
	MassVNormal= new TPoint*[LandInfo.DH+1];
	for(int i=0;i<LandInfo.DH;i++)
	{
	   FlagNormalMap[i]= new bool[LandInfo.DW+1];
	   PloskNormals[i] = new TPoint[LandInfo.DW+1];
       MassVNormal[i]  = new TPoint[LandInfo.DW+1];
	}
	for ( i = 0; i < LandInfo.DH; i ++)
      for ( j = 0; j < LandInfo.DH; j ++)
         FlagNormalMap[i][j]=false;

 	for ( i = 0; i < 6; i++)
	{
	  Normals[i].x=0;
      Normals[i].y=0;
      Normals[i].z=0;
	}
	for ( i = 1; i < LandInfo.DH-1; i ++)
      for ( j = 1; j < LandInfo.DH-1; j ++)
	  {
         if(FlagNormalMap[i][j]==false)
	     {
		     Versh[0].x=i;
			 Versh[0].y=MassHeight[i][j]; 
			 Versh[0].z=j;
			 Versh[1].x=i+STEP_SIZE;
			 Versh[1].y=MassHeight[i+1][j];
			 Versh[1].z=j;
			 Versh[2].x=i+STEP_SIZE;
			 Versh[2].y=MassHeight[i+1][j+1];
			 Versh[2].z=j+STEP_SIZE;
			 Normals[0]=GenTrianglePNormal(Versh[0],Versh[1],Versh[2]);
             PloskNormals[i][j]=Normals[0];
	         FlagNormalMap[i][j]=true;
	      }
	      else
              Normals[0]=PloskNormals[i][j];

		 if(FlagNormalMap[i][j+1]==false)
	     {
		     Versh[0].x=i;
			 Versh[0].y=MassHeight[i][j]; 
			 Versh[0].z=j;
			 Versh[1].x=i+STEP_SIZE;
			 Versh[1].y=MassHeight[i+1][j];
			 Versh[1].z=j;
			 Versh[2].x=i;
			 Versh[2].y=MassHeight[i][j-1];
			 Versh[2].z=j-STEP_SIZE;
			 Normals[1]=GenTrianglePNormal(Versh[0],Versh[1],Versh[2]);
             PloskNormals[i][j+1]=Normals[1];
	         FlagNormalMap[i][j+1]=true;
	      }
	      else
              Normals[1]=PloskNormals[i][j+1];
    /*  if(FlagNormalMap[i+(STEP_SIZE+j)*MAP_SIZE]==false)
	  {
      Versh[0].x=i;Versh[0].y=g_HeightMap[i+j*MAP_SIZE];Versh[0].z=j;
	  Versh[1].x=i+STEP_SIZE;Versh[1].y=g_HeightMap[i+STEP_SIZE+j*MAP_SIZE];Versh[1].z=j;
	  Versh[2].x=i;Versh[2].y=g_HeightMap[i+(j-STEP_SIZE)*MAP_SIZE];Versh[2].z=j-STEP_SIZE;
	  Normals[1]=Normal(Versh[0],Versh[1],Versh[2]);
	  PloskNormals[i+(STEP_SIZE+j)*MAP_SIZE]=Normals[1];
	  FlagNormalMap[i+(STEP_SIZE+j)*MAP_SIZE]=true;
	  }
	  else
       Normals[1]= PloskNormals[i+(STEP_SIZE+j)*MAP_SIZE];
	   */
      	if(FlagNormalMap[i+1][j]==false)
	     {
		     Versh[0].x=i;
			 Versh[0].y=MassHeight[i][j]; 
			 Versh[0].z=j;
			 Versh[1].x=i;
			 Versh[1].y=MassHeight[i][j-1];
			 Versh[1].z=j-STEP_SIZE;
			 Versh[2].x=i-STEP_SIZE;
			 Versh[2].y=MassHeight[i-1][j-1];
			 Versh[2].z=j-STEP_SIZE;
			 Normals[2]=GenTrianglePNormal(Versh[0],Versh[1],Versh[2]);
             PloskNormals[i+1][j]=Normals[2];
	         FlagNormalMap[i+1][j]=true;
	      }
	      else
              Normals[2]=PloskNormals[i+1][j];
		/*
	  if(FlagNormalMap[i+STEP_SIZE+j*MAP_SIZE]==false)
	  {
     Versh[0].x=i;Versh[0].y=g_HeightMap[i+j*MAP_SIZE];Versh[0].z=j;
	  Versh[1].x=i;Versh[1].y=g_HeightMap[i+(j-STEP_SIZE)*MAP_SIZE];Versh[1].z=j-STEP_SIZE;
	  Versh[2].x=i-STEP_SIZE;Versh[2].y=g_HeightMap[i-STEP_SIZE+(j-STEP_SIZE)*MAP_SIZE];Versh[2].z=j-STEP_SIZE;
	  Normals[2]=Normal(Versh[0],Versh[1],Versh[2]);
	  PloskNormals[STEP_SIZE+i+j*MAP_SIZE]=Normals[2];
	  FlagNormalMap[STEP_SIZE+i+j*MAP_SIZE]=true;
	  }
	  else
       Normals[2]=PloskNormals[i+STEP_SIZE+j*MAP_SIZE];
      */
		 if(FlagNormalMap[i-1][j]==false)
	     {
		     Versh[0].x=i;
			 Versh[0].y=MassHeight[i][j]; 
			 Versh[0].z=j;

			 Versh[1].x=i;
			 Versh[1].y=MassHeight[i][j+1];
			 Versh[1].z=j+STEP_SIZE;

			 Versh[2].x=i-STEP_SIZE;
			 Versh[2].y=MassHeight[i-1][j];
			 Versh[2].z=j;
			 Normals[3]=GenTrianglePNormal(Versh[0],Versh[1],Versh[2]);
             PloskNormals[i-1][j]=Normals[3];
	         FlagNormalMap[i-1][j]=true;
	      }
	      else
              Normals[3]=PloskNormals[i-1][j];
		/*
	    if(FlagNormalMap[i-STEP_SIZE+j*MAP_SIZE]==false)
	  {
   
	  Versh[0].x=i;Versh[0].y=g_HeightMap[i+j*MAP_SIZE];Versh[0].z=j;
	  Versh[1].x=i;Versh[1].y=g_HeightMap[i+(j+STEP_SIZE)*MAP_SIZE];Versh[1].z=j+STEP_SIZE;
	  Versh[2].x=i-STEP_SIZE;Versh[2].y=g_HeightMap[i-STEP_SIZE+(j)*MAP_SIZE];Versh[2].z=j;
	  Normals[3]=Normal(Versh[0],Versh[1],Versh[2]);
	  PloskNormals[-STEP_SIZE+i+j*MAP_SIZE]=Normals[3];
	  FlagNormalMap[-STEP_SIZE+i+j*MAP_SIZE]=true;
	  }
	  else
       Normals[3]=PloskNormals[i-STEP_SIZE+j*MAP_SIZE];
      */
		if(FlagNormalMap[i][j-1]==false)
	     {
		     Versh[0].x=i;
			 Versh[0].y=MassHeight[i][j]; 
			 Versh[0].z=j;

			 Versh[1].x=i-STEP_SIZE;
			 Versh[1].y=MassHeight[i-1][j];
			 Versh[1].z=j;

			 Versh[2].x=i-STEP_SIZE;
			 Versh[2].y=MassHeight[i-1][j-1];
			 Versh[2].z=j-STEP_SIZE;
			 Normals[4]=GenTrianglePNormal(Versh[0],Versh[1],Versh[2]);
             PloskNormals[i][j-1]=Normals[4];
	         FlagNormalMap[i][j-1]=true;
	      }
	      else
              Normals[4]=PloskNormals[i][j-1];
		 /*
    if(FlagNormalMap[i+(-STEP_SIZE+j)*MAP_SIZE]==false)
	  {
   
	  Versh[0].x=i;Versh[0].y=g_HeightMap[i+j*MAP_SIZE];Versh[0].z=j;
	  Versh[1].x=i-STEP_SIZE;Versh[1].y=g_HeightMap[i-STEP_SIZE+(j)*MAP_SIZE];Versh[1].z=j;
	  Versh[2].x=i-STEP_SIZE;Versh[2].y=g_HeightMap[i-STEP_SIZE+(j-STEP_SIZE)*MAP_SIZE];Versh[2].z=j-STEP_SIZE;
	  Normals[4]=Normal(Versh[0],Versh[1],Versh[2]);
	  PloskNormals[i+(j-STEP_SIZE)*MAP_SIZE]=Normals[4];
	  FlagNormalMap[i+(j-STEP_SIZE)*MAP_SIZE]=true;
	  }
	  else
       Normals[4]=PloskNormals[i+(j-STEP_SIZE)*MAP_SIZE];
      */
			if(FlagNormalMap[i-1][j-1]==false)
	     {
		     Versh[0].x=i;
			 Versh[0].y=MassHeight[i][j]; 
			 Versh[0].z=j;

			 Versh[1].x=i;
			 Versh[1].y=MassHeight[i][j+1];
			 Versh[1].z=j+STEP_SIZE;

			 Versh[2].x=i+STEP_SIZE;
			 Versh[2].y=MassHeight[i+1][j+1];
			 Versh[2].z=j+STEP_SIZE;
			 Normals[5]=GenTrianglePNormal(Versh[0],Versh[1],Versh[2]);
             PloskNormals[i-1][j-1]=Normals[5];
	         FlagNormalMap[i-1][j-1]=true;
	      }
	      else
              Normals[5]=PloskNormals[i-1][j-1];
		/*
   if(FlagNormalMap[i-STEP_SIZE+(-STEP_SIZE+j)*MAP_SIZE]==false)
	  {
 
	  Versh[0].x=i;Versh[0].y=g_HeightMap[i+j*MAP_SIZE];Versh[0].z=j;
	  Versh[1].x=i;Versh[1].y=g_HeightMap[i+(j+STEP_SIZE)*MAP_SIZE];Versh[1].z=j+STEP_SIZE;
	  Versh[2].x=i+STEP_SIZE;Versh[2].y=g_HeightMap[i+STEP_SIZE+(j+STEP_SIZE)*MAP_SIZE];Versh[2].z=j+STEP_SIZE;
	  Normals[5]=Normal(Versh[0],Versh[1],Versh[2]);
   	  PloskNormals[i-STEP_SIZE+(j-STEP_SIZE)*MAP_SIZE]=Normals[5];
	  FlagNormalMap[i-STEP_SIZE+(j-STEP_SIZE)*MAP_SIZE]=true;
	  }
	  else
       Normals[5]=PloskNormals[i-STEP_SIZE+(j-STEP_SIZE)*MAP_SIZE];
       */
	    NormalRes.x=(Normals[0].x+Normals[1].x+Normals[2].x+Normals[3].x+Normals[4].x+Normals[5].x);
        NormalRes.y=(Normals[0].y+Normals[1].y+Normals[2].y+Normals[3].y+Normals[4].y+Normals[5].y);
        NormalRes.z=(Normals[0].z+Normals[1].z+Normals[2].z+Normals[3].z+Normals[4].z+Normals[5].z);
		float modul;
        modul=sqrt(NormalRes.x*NormalRes.x+NormalRes.y*NormalRes.y+NormalRes.z*NormalRes.z);
        NormalRes.x/=modul;
        NormalRes.y/=modul;
        NormalRes.z/=modul;
		MassVNormal[i][j]=NormalRes;
	}
	delete[] FlagNormalMap;
	delete[] PloskNormals;
}
//***********************************************************************************************************
TPoint CLandshaft::GenTrianglePNormal(TPoint p1,TPoint p2,TPoint p3)
{
   TPoint NVector;
   float modul;
   NVector.x=(p2.y-p1.y)*(p3.z-p1.z)-(p2.z-p1.z)*(p3.y-p1.y);
   NVector.y=(p3.x-p1.x)*(p2.z-p1.z)-(p2.x-p1.x)*(p3.z-p1.z);
   NVector.z=(p2.x-p1.x)*(p3.y-p1.y)-(p3.x-p1.x)*(p2.y-p1.y);
   modul=sqrt(NVector.x*NVector.x+NVector.y*NVector.y+NVector.z*NVector.z);
   NVector.x/=modul;
   NVector.y/=modul;
   NVector.z/=modul;
   return NVector;
}
//***********************************************************************************************************
void CLandshaft::RenderLand()
{
 int i= 0, j = 0;                                 // Создаем пару переменных для перемещения по массиву
 const float Texcord=1.0;
 TPoint p[4];

 glNewList(GlModelList,GL_COMPILE); 
 ////////////////////////////////////////
 	 glColor3f(1.0,0.0,0.0);
////////////////////////////////////////
 for(i=0;i<(LandInfo.height/LandInfo.StepSize)-1;i++)
	  for(j=0;j<(LandInfo.width/LandInfo.StepSize)-1;j++)
     {     
		 p[0].x = i*LandInfo.StepSize;              
         p[0].y = MassHeight[i][j];  
         p[0].z = j*LandInfo.StepSize;  
		 
		 p[1].x = i*LandInfo.StepSize;                    
         p[1].y = MassHeight[i][j+1];  
         p[1].z = j*LandInfo.StepSize + LandInfo.StepSize;              
  	     p[2].x = i*LandInfo.StepSize + LandInfo.StepSize; 
         p[2].y = MassHeight[i+1][j+1];  
         p[2].z = j*LandInfo.StepSize + LandInfo.StepSize;
		 p[3].x = i*LandInfo.StepSize + LandInfo.StepSize; 
         p[3].y = MassHeight[i+1][j]; 
         p[3].z = j*LandInfo.StepSize;

	 // glColor3f(p[0].y/256,p[0].y/256,p[0].y/256);

	glBegin(GL_QUADS);                           //GL_TRIANGLE_FAN  Полигоны GL_LINES GL_QUADS  GL_TRIANGLE_STRIP GL_LINE_STRIP
		/* glMultiTexCoord2f(GL_TEXTURE0,0,0);
		 glMultiTexCoord2f(GL_TEXTURE1,0,0);
		 glMultiTexCoord2f(GL_TEXTURE2,0,0);*/
		 glNormal3f(MassVNormal[i][j].x,
			        MassVNormal[i][j].y,
					MassVNormal[i][j].z);   
	     glVertex3f(p[0].x, p[0].y, p[0].z);      
                                                        
	    /* glMultiTexCoord2f(GL_TEXTURE0,Texcord,0.0);
		   glMultiTexCoord2f(GL_TEXTURE1,Texcord,0.0);
		   glMultiTexCoord2f(GL_TEXTURE2,Texcord,0.0);*/
		 glNormal3f(MassVNormal[i][j+1].x,
			        MassVNormal[i][j+1].y,
					MassVNormal[i][j+1].z);   
         glVertex3f(p[1].x, p[1].y, p[1].z);     
         
        /* glMultiTexCoord2f(GL_TEXTURE0,0.0,Texcord);
		 glMultiTexCoord2f(GL_TEXTURE1,0.0,Texcord);
		 glMultiTexCoord2f(GL_TEXTURE2,0.0,Texcord);*/
		 glNormal3f(MassVNormal[i+1][j+1].x,
			        MassVNormal[i+1][j+1].y,
					MassVNormal[i+1][j+1].z);   
         glVertex3f(p[2].x, p[2].y, p[2].z);     
       
         /*glMultiTexCoord2f(GL_TEXTURE0,Texcord,Texcord);
		 glMultiTexCoord2f(GL_TEXTURE1,Texcord,Texcord);
		 glMultiTexCoord2f(GL_TEXTURE2,Texcord,Texcord);*/
		 glNormal3f(MassVNormal[i+1][j].x,
			        MassVNormal[i+1][j].y,
					MassVNormal[i+1][j].z);   
         glVertex3f( p[3].x, p[3].y, p[3].z);     
	
	glEnd();
	}
glEndList(); 
}
//***********************************************************************************************************
void CLandshaft::Draw()
{
	glCallList(GlModelList);
	
}
//***********************************************************************************************************
bool CLandshaft::InitLandshaft(CString LandFile,CString TexFile)
{
	//функция рендерит ландшафт; возвращает true - если рендеринг прошел успешно
	if(LoadRawFileToMass(&MassHeight,LandInfo.LandFile,LandInfo.ElemSize)==false) return false;
	/*Tailing();
	Tailing();
	Tailing();*/
	GenVNormals();//высчитали вершинные нормали
	RenderLand();//загрузили ландшафт в память(opengl list) 
   
 return true;
}
//***********************************************************************************************************
WORD sh(WORD x)
{
 WORD result;
  _asm
 {
   mov ax,x
   ror ax,8
   mov result,ax
 }
}

bool CLandshaft::LoadRawFileToMass(int ***mass,CString FileName,int type)
{
	
if(type==1)
{
  	BYTE **pTempMap= new BYTE*[LandInfo.height+1];
	int **massH;
    int i,j;
//	int DH,DW;

  FILE *pFile = NULL;
  if((pFile = fopen((LPCSTR) FileName, "rb" ))==NULL)
  {
	  delete[] pTempMap;
	  pTempMap=NULL;
	  return false;
  }
   for(i=0;i<LandInfo.height;i++)
   {
	   if(type==1)
	     pTempMap[i]=new BYTE[LandInfo.width+1];
       	   
	   for(j=0;j<(LandInfo.DW);j++)
           pTempMap[i][j]=0;
       if(fread( pTempMap[i],sizeof(BYTE),LandInfo.width, pFile)==NULL)
       {
	     for(j=0; j<i;j++)
		 {
           delete[] pTempMap[j];
		   pTempMap[j]=NULL;
		 }
		delete[] pTempMap;
		pTempMap=NULL;
		return false;
       }

   }  
  fclose(pFile);

  massH= new int*[LandInfo.DH+1];
  for(i=0;i<LandInfo.DH;i++)
	  massH[i]=new int[LandInfo.DW+1];

  for(i=0;i<(LandInfo.DH);i++)
	  for(j=0;j<(LandInfo.DW);j++)
	  {
		  if(pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]<0)
			  pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]=0;
        massH[i][j]=int(pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]);
	  }
      
	    for(j=0; j<LandInfo.height;j++)
		 {
           delete[] pTempMap[j];
		   pTempMap[j]=NULL;
		 }
		delete[] pTempMap;
		pTempMap=NULL;
  *mass=massH;
}
else
/////////////////////////////////////////////////////////
if(type==2)
{
  	WORD **pTempMap= new WORD*[LandInfo.height+1];
	int **massH;
    int i,j;
//	int DH,DW;

  FILE *pFile = NULL;
  if((pFile = fopen((LPCSTR) FileName, "rb" ))==NULL)
  {
	  delete[] pTempMap;
	  return false;
  }
   for(i=0;i<LandInfo.height;i++)
   {
	   if(type==2)
	      pTempMap[i]=new WORD[LandInfo.width+1];
	   	   
	   for(j=0;j<(LandInfo.DW);j++)
           pTempMap[i][j]=0;
       if(fread( pTempMap[i],type,LandInfo.width, pFile)==NULL)
       {
	     for(j=0; j<i;j++)
		 {
           delete[] pTempMap[j];
		   pTempMap[j]=NULL;
		 }
		delete[] pTempMap;
		pTempMap=NULL;
		return false;
       }

   }  
  fclose(pFile);

  massH= new int*[LandInfo.DH+1];
  for(i=0;i<LandInfo.DH;i++)
	  massH[i]=new int[LandInfo.DW+1];

  for(i=0;i<(LandInfo.DH);i++)
	  for(j=0;j<(LandInfo.DW);j++)
	  {
		  pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]=sh(pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]);
		  if(pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]<0)
			  pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]=0;
		  if(pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]>8000)
			  pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]=0;
        massH[i][j]=int(pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]);
	  }
	    for(j=0; j<LandInfo.height;j++)
		 {
           delete[] pTempMap[j];
		   pTempMap[j]=NULL;
		 }
		delete[] pTempMap;
		pTempMap=NULL;
  *mass=massH;
}
  return true;

 }
//***********************************************************************************************************