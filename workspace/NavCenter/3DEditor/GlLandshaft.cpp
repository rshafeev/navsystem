#include "StdAfx.h"
#include "GlLandshaft.h"
#include "memory.h"
//====================================================================================================================
CGlLandshaft::CGlLandshaft(void)
{
	InitFlag=false;
	MixingFlag=false;
	MassHeight=NULL;
    MassTexture=NULL;
	MassVNormal=NULL;
    SelectMapInd=-1;
	TextureDetalLevel=3;
}
CGlLandshaft::~CGlLandshaft(void)
{
	Free();

}
void CGlLandshaft::SetTextureDetalization(int DetalizationKoef)
{
  TextureDetalLevel=DetalizationKoef;
}
int   CGlLandshaft::IsolateYFromEauation(CPoint pp,CVector3 *p)
{
  int y=0;
  y=  (p[0].x-pp.x)*(p[1].y-p[0].y)*(p[2].z-p[0].z);
  y+= (p[0].x-p[1].x)*(p[2].y-p[0].y)*(pp.y-p[0].z);
  y+= (pp.y-p[0].z)*(p[1].y-p[0].y)*(p[2].x-p[0].x);
  y+= (p[1].z-p[0].z)*(p[2].y-p[0].y)*(pp.x-p[0].x);
  y/= ( (p[1].z-p[0].z)*(p[2].y-p[0].y)-(p[1].x-p[0].x)*(p[2].z-p[0].z) );
  y+=p[0].y;
  return 0;
}
bool  CGlLandshaft::PointBilongTriangle(CPoint Point,CVector3 *Triangle)
{
  int RebroLong[3];    // длина сторон треугольника(в квадрате)
  int LongToVersh[3];  //расстояние от точки до вершины(в квадрате, всеравно нужно только для сравнения)
  unsigned int i,j;
  RebroLong[0]=(Triangle[0].x-Triangle[1].x)*(Triangle[0].x-Triangle[1].x)+
	           (Triangle[0].y-Triangle[1].y)*(Triangle[0].y-Triangle[1].y);
  RebroLong[1]=(Triangle[0].x-Triangle[2].x)*(Triangle[0].x-Triangle[2].x)+
	           (Triangle[0].y-Triangle[2].y)*(Triangle[0].y-Triangle[2].y);
  RebroLong[2]=(Triangle[1].x-Triangle[2].x)*(Triangle[1].x-Triangle[2].x)+
	           (Triangle[1].y-Triangle[2].y)*(Triangle[1].y-Triangle[2].y);
  
  for(i=0;i<3;i++)
  LongToVersh[i]=(Point.x-Triangle[i].x)*(Point.x-Triangle[i].x)+
	             (Point.y-Triangle[i].y)*(Point.y-Triangle[i].y);
  for(i=0;i<3;i++)
    for(j=0;j<3;j++)
     if(LongToVersh[j]> RebroLong[i])
       return false;
   return true;
}
//====================================================================================================================
int  CGlLandshaft::GetHeightForPoint(CPoint p)
{
  int Height=0;
  CPoint NormP;
  CPoint BlockP;
  CVector3 RecangleP[4]; //соседние вершины
  NormP.x=p.x/LandInfo.ScaleX;
  NormP.y=p.y/LandInfo.ScaleY;
  RecangleP[0].x=NormP.x/LandInfo.StepSize;
  RecangleP[0].y=NormP.y/LandInfo.StepSize;

  if(NormP.x/LandInfo.ScaleX/LandInfo.StepSize>=LandInfo.DW||NormP.y/LandInfo.ScaleY/LandInfo.StepSize>=LandInfo.DH||
	 NormP.x<0||NormP.y<0)
	  return 0;
  if(NormP.x==0||NormP.y==0)
      return MassHeight[NormP.x][NormP.y];
//находим точки квадрата, в который попала точка p
  RecangleP[0].z=MassHeight[(int)RecangleP[0].x][(int)RecangleP[0].y];
  
  RecangleP[1].x=RecangleP[0].x+LandInfo.StepSize;
  RecangleP[1].y=RecangleP[0].y;
  RecangleP[1].z=MassHeight[(int)RecangleP[1].x][(int)RecangleP[1].y];
  
  RecangleP[2].x=RecangleP[0].x+LandInfo.StepSize;
  RecangleP[2].y=RecangleP[0].y+LandInfo.StepSize;
  RecangleP[2].z=MassHeight[(int)RecangleP[2].x][(int)RecangleP[2].y];
  
  RecangleP[3].x=RecangleP[0].x;
  RecangleP[3].y=RecangleP[0].y+LandInfo.StepSize;
  RecangleP[3].z=MassHeight[(int)RecangleP[3].x][(int)RecangleP[3].y];

//находим треугольник, куда попола точка p
  if(PointBilongTriangle(p,&RecangleP[0])==true)
  {
    //подставляем нашу точку в уравнение плоскости, костроенную на точках RecangleP[0],RecangleP[1],RecangleP[2]
	  IsolateYFromEauation(NormP,&RecangleP[0]);
  }
  else
  {

  }


  return Height;
//  if()
}
//====================================================================================================================
CVector2 CGlLandshaft::GetHgtGeoCord(CPoint p)
{
	//result: x-широта y- долгота
    CVector2 result;
	if(LandInfo.ElemSize!=2)
	{
	  result.x=190.0;
	  result.y=190.0;
 	  return result;
	}
	result.x=HgtGeoInfo.GeoCord.x;
	result.y=HgtGeoInfo.GeoCord.y;

	CVector2 NormalizeCord;
    HgtGeoInfo.GlobMapSize.x=1200*180;
    HgtGeoInfo.GlobMapSize.y=1200*360;

	NormalizeCord.y=p.y/LandInfo.ScaleY+HgtGeoInfo.Block.y*1200;
	NormalizeCord.x=p.x/LandInfo.ScaleX+HgtGeoInfo.Block.x*1200;

    result.y = (NormalizeCord.y)/HgtGeoInfo.GlobMapSize.y *360.0-180;
	result.x = (NormalizeCord.x)/HgtGeoInfo.GlobMapSize.x *180.0-90;
	
	double z=(NormalizeCord.x)/(HgtGeoInfo.GlobMapSize.x)*M_PI;

	//result.x=(2 * atan(exp(z)) - M_PI/2.0) * 180.0/M_PI;
	//result.x=sinh(z)*180/M_PI-90;
/* double z=(-block.y + BitmapOrigo(level)) / 2*M_PI;
	result.x=(block.x - BitmapOrigo(level)) / PixelsPerLonDegree(level);
	result.y=(2 * atan(exp(z)) - M_PI/2.0) * 180.0/M_PI;
*/
 /*  double z=(block.y - BitmapOrigo(level)) / (-PixelsPerLonRadian(level));
   *Lon = (block.x - BitmapOrigo(level)) / PixelsPerLonDegree(level);
   *Lat = (2 * atan(exp(z)) - M_PI/2.0) * 180.0/M_PI;*/

/*     return STEPENTWO[level-1]* PICTSIZE/360.0;
PixelsPerLonRadian(int level)
	return STEPENTWO[level-1]* PICTSIZE/(2*M_PI);*/
/*
  Lon = (X - BitmapOrigo[Level]) / PixelsPerLonDegree[Level];
  Lat = (2 * arctan(exp(z)) - PI/2) * 180/PI,
  где z = (Y - BitmapOrigo[Level]) / -PixelsPerLonRadian[Level];
*/
    return result;
}
//====================================================================================================================
int CGlLandshaft::GetLandGridType()
{
	return LandInfo.ElemSize;
}
//====================================================================================================================
template<class T> void  CGlLandshaft:: FilterSmooth(vector<T> &m_heights,int w,int h)
{
    std::vector<T> source(m_heights);
	int m_size=w;
    float value = 0.0f;
    float cellAverage = 0.0f;
    int i = 0;
    int bounds = m_size * m_size;

    for (int y = 0; y < w; ++y)
    {
        for (int x = 0; x < h; ++x)
        {
            value = 0.0f;
            cellAverage = 0.0f;

            i = (y - 1) * m_size + (x - 1);
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            i = (y - 1) * m_size + x;
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            i = (y - 1) * m_size + (x + 1);
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            i = y * m_size + (x - 1);
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            i = y * m_size + x;
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            i = y * m_size + (x + 1);
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            i = (y + 1) * m_size + (x - 1);
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            i = (y + 1) * m_size + x;
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            i = (y + 1) * m_size + (x + 1);
            if (i >= 0 && i < bounds)
            {
                value += source[i];
                cellAverage += 1.0f;
            }

            m_heights[y * m_size + x] = value / cellAverage;
        }
    }

}
//====================================================================================================================
bool CGlLandshaft::SaveLand(CString PathName,vector<CString> *ProtocolStr,bool rewrite)
{
	unsigned int i;
	if(IsInit()==false) return false;
	PathName=PathName+"\\";
//копируем все необходимые для открытия 

	//копируем карту высот
	if((PathName+GetFileName(LandInfo.LandFile))!=LandInfo.LandFile&&LandInfo.LandFile!="")
	{
		CString NewName=PathName+GetFileName(LandInfo.LandFile);
		CString LastName=LandInfo.LandFile;
		if(CopyFileA(LastName,NewName,false)==false) return false;
	}
	//копируем mixing текстуры
	CString Tex1Str,Tex2Str,Tex3Str,TexVecStr,TexWaterStr;
	GetMixingInfo(Tex1Str,Tex2Str,Tex3Str,TexVecStr,TexWaterStr);
	if((PathName+GetFileName(Tex1Str))!=Tex1Str&&Tex1Str!="")
	{
		CString NewName=PathName+GetFileName(Tex1Str);
		CString LastName=Tex1Str;
		if(CopyFileA(LastName,NewName,false)==false) return false;
	}
	if((PathName+GetFileName(Tex2Str))!=Tex2Str&&Tex2Str!="")
	{
		CString NewName=PathName+GetFileName(Tex2Str);
		CString LastName=Tex2Str;
		if(CopyFileA(LastName,NewName,false)==false) return false;
	}
	if((PathName+GetFileName(Tex3Str))!=Tex3Str&&Tex3Str!="")
	{
		CString NewName=PathName+GetFileName(Tex3Str);
		CString LastName=Tex3Str;
		if(CopyFileA(LastName,NewName,false)==false) return false;
	}
	if((PathName+GetFileName(TexVecStr))!=TexVecStr&&Tex3Str!="")
	{
		CString NewName=PathName+GetFileName(TexVecStr);
		CString LastName=TexVecStr;
		if(CopyFileA(LastName,NewName,false)==false) return false;
	}	
	//копируем обычные текстуры ландшафта
   for(i=0;i<MapTextures.size();i++)
   {
	   	if((PathName+GetFileName(MapTextures[i].GetTexFileName()))!=MapTextures[i].GetTexFileName()&&
			MapTextures[i].GetTexFileName()!="")
	    {
		   CString NewName=PathName+GetFileName(MapTextures[i].GetTexFileName());
		   CString LastName=MapTextures[i].GetTexFileName();
		   if(CopyFileA(LastName,NewName,false)==false) return false;
	    }	
   }

	//записываем информацию о ландшафте
	ProtocolStr->push_back("Landshaft:\n");
	CString buf;

	ProtocolStr->push_back(GetFileName(LandInfo.LandFile)+"\n");

	buf.Format("%d %d \n",LandInfo.width,LandInfo.height);//ширина вдлина ландшафта
	ProtocolStr->push_back(buf);

	buf.Format("%f %f \n",90/LandInfo.ScaleX,90/LandInfo.ScaleY);//ширина вдлина ландшафта
	ProtocolStr->push_back(buf);

	buf.Format("%d %d \n",LandInfo.MinHeight,LandInfo.MaxHeight);//мин-макс высота ландшафта
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.ElemSize);//размер одного эл-та 1: raw(BYTE); 2: hgt(WORD)
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.StepSize);//шаг построения сетки
	ProtocolStr->push_back(buf);

	buf.Format("%d \n",LandInfo.FilterTaling);//активизация фильтра(сглаживание)
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.FilterFractal);//активизация фильтра(фрактальное построение гор)
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.FilterAverage);//активизация фильтра(усреднение матрицей 8*8)
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.FilterSmooth);//активизация фильтра(сглаживание)
	ProtocolStr->push_back(buf);


	//Текстуры:
	ProtocolStr->push_back("   MixingTextures:\n");
	//записываем информацию о текстурах смешивания
	if(MixingFlag==true)
	{
		/*Tex1Str+="\n";
        Tex2Str+="\n";
		Tex3Str+="\n";
		TexVecStr+="\n";*/
	    ProtocolStr->push_back(GetFileName(Tex1Str)+"\n");
	    ProtocolStr->push_back(GetFileName(Tex2Str)+"\n");
	    ProtocolStr->push_back(GetFileName(Tex3Str)+"\n");
	    ProtocolStr->push_back(GetFileName(TexVecStr)+"\n");
	}
	else
	{
		ProtocolStr->push_back("-#- \n");
		ProtocolStr->push_back("-#- \n");
	    ProtocolStr->push_back("-#- \n");
	    ProtocolStr->push_back("-#- \n");
 	}
    //записываем информацию об обычных текстурах
   ProtocolStr->push_back("   Textures:\n");
   for(i=0;i<MapTextures.size();i++)
   {
	   ProtocolStr->push_back(GetFileName(MapTextures[i].GetTexFileName())+ "\n");
   }
  return true;
}
//====================================================================================================================
static unsigned int heightIndexAt(int x, int z,int w,int h) 
{
    // Given a 2D height map coordinate, this method returns the index
    // into the height map. This method wraps around for coordinates larger
    // than the height map size.

    return (((x + w) % w) + ((z + h) % h) * w);
}
static float random(float min, float max)
{
        // Returns a random number in range [min,max].

        return min + (max - min)
            * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
}
//======================================
template<class T> void CGlLandshaft::FilterDiamondSquareFractal(T** &heights,float roughness,int width,int height)
{
	
	
int m_size=min(width,height);
	vector<double> m_heights;
	m_heights.resize(width*height);
	for (int i = 0; i <width; i++)
		for (int j = 0; j <height; j++)
		{
			m_heights[i*width+j]=  heights[i][j];
		}

if(LandInfo.FilterFractal==true)
{
   //  srand(static_cast<unsigned int>(time(0)));
   	roughness=26.0f;


	int p1, p2, p3, p4, mid;
    float dH = m_size * 0.5f;
    float dHFactor = powf(2.0f, -roughness);
    float minH = 0.0f, maxH = 0.0f;
	
    for (int w = m_size-1; w > 0; dH *= dHFactor, w /= 2)
    {
        // Кристал
        for (int z = 0; z < width; z += w)
        {
            for (int x = 0; x < height; x += w)
            {
                p1 = heightIndexAt(x, z,width,height);
                p2 = heightIndexAt(x + w, z,width,height);
                p3 = heightIndexAt(x + w, z + w,width,height);
                p4 = heightIndexAt(x, z + w,width,height);
                mid = heightIndexAt(x + w / 2, z + w / 2,width,height);
	
				
                m_heights[mid] = /*random(-dH, dH) + */(m_heights[p1] + m_heights[p2] + m_heights[p3] + m_heights[p4]) * 0.25f;

                minH = min(minH, (double)m_heights[mid]);
                maxH = max(maxH, (double)m_heights[mid]);
            }
        }
       // continue;
        // Сфера
        for (int z = 0; z < m_size; z += w)
        {
            for (int x = 0; x < m_size; x += w)
            {
                p1 = heightIndexAt(x, z,width,height);
                p2 = heightIndexAt(x + w, z,width,height);
                p3 = heightIndexAt(x + w / 2, z - w / 2,width,height);
                p4 = heightIndexAt(x + w / 2, z + w / 2,width,height);
                mid = heightIndexAt(x + w / 2, z,width,height);
                m_heights[mid] =/* random(-dH, dH) +*/ (m_heights[p1] + m_heights[p2] + m_heights[p3] + m_heights[p4]) * 0.25f;
                minH = min(minH, m_heights[mid]);
                maxH = max(maxH, m_heights[mid]);

                p1 = heightIndexAt(x, z,width,height);
                p2 = heightIndexAt(x, z + w,width,height);
                p3 = heightIndexAt(x + w / 2, z + w / 2,width,height);
                p3 = heightIndexAt(x - w / 2, z + w / 2,width,height);
                mid = heightIndexAt(x, z + w / 2,width,height);

                m_heights[mid] =/* random(-dH, dH) +*/ (m_heights[p1] + m_heights[p2] + m_heights[p3] + m_heights[p4]) * 0.25f;

                minH = min(minH, m_heights[mid]);
                maxH = max(maxH, m_heights[mid]);
            }
        }
		
    }

//////////////////

}
if(LandInfo.FilterSmooth==true)
	FilterSmooth(m_heights,width,height);
		for (int i = 0; i <width; ++i)
		for (int j = 0; j <height; ++j)
            heights[i][j]= (T)m_heights[i*m_size+j];
		m_heights.clear();
}
//====================================================================================================================
void CGlLandshaft::AddMap(CString FileName)
{
	CTexture texAdd;
	texAdd.InitTexture(FileName);
	MapTextures.push_back(texAdd);
	SelectMapInd=MapTextures.size();
}
//====================================================================================================================
void CGlLandshaft::Free()
{
	if(MixingShader.IsCreated()==true)
		MixingShader.DestroyProgram();

  	  if(MassHeight!=NULL)	  
  {
	  for(int i=0; i<LandInfo.DH;i++)
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
	  for(int i=0; i<LandInfo.DW;i++)
	    if(MassVNormal[i]!=NULL)
	   	  delete[] MassVNormal[i];
	    else break;
  }

	if(MassHeight!=NULL)
		delete[] MassHeight;
	if(MassTexture!=NULL)
		delete[] MassTexture;
   if(MassVNormal!=NULL)
		delete[] MassVNormal;
   glDeleteLists(GlMemoryPt,1);
	InitFlag=false;
	MixingFlag=false;

	for(int i=0;i<MapTextures.size();i++)
		MapTextures[i].Free();
	MapTextures.clear();
	SelectMapInd=-1;
    

}
//====================================================================================================================
vector<CString> CGlLandshaft::PrintTexFiles()
{
   vector<CString> list;
   for(int i=0;i<MapTextures.size();i++)
	   list.push_back(MapTextures[i].GetTexFileName());
   return list;
}
//====================================================================================================================
void CGlLandshaft::AddMapMixing(CString Tex1File,CString Tex2File,CString Tex3File,CString TexWaterFile, CString TexVec)
{
    for(int i=0;i<4;i++)
       TexMixing[i].Free();
	TexMixing[0].InitTexture(Tex1File);
	TexMixing[1].InitTexture(Tex2File);
	TexMixing[2].InitTexture(Tex3File);
	TexMixing[3].InitTexture(TexVec);
	if(TexMixing[4].IsCreated()!=true)
      TexMixing[4].InitTexture(TexWaterFile);
	SelectMapInd=0;
	MixingFlag=true;
}
//====================================================================================================================
void CGlLandshaft::NextMap()
{
	if(SelectMapInd==-1&&MixingFlag==true)
       SelectMapInd++;
	else
	if(SelectMapInd>=0&&SelectMapInd<MapTextures.size())
	   SelectMapInd++;
	else
	if(MixingFlag==false&&SelectMapInd==-1&&MapTextures.size()!=0)
      SelectMapInd=1;
   else
       SelectMapInd=-1;

}
void CGlLandshaft::BackMap()
{
	if(SelectMapInd>2)    
	   SelectMapInd--;
   else
	if(SelectMapInd==1&&MixingFlag==true)
       SelectMapInd--;
   else
	if(MapTextures.size()>0&&SelectMapInd==-1)
       SelectMapInd = MapTextures.size();
}
//====================================================================================================================
bool CGlLandshaft::IsLandMixing()
{
	return MixingFlag;
}
//====================================================================================================================
void CGlLandshaft::GetMixingInfo(CString &Tex1File,CString &Tex2File,CString &Tex3File,CString &TexVec,CString &TexWaterFile)
{
   if(MixingFlag==true)
   {
	   Tex1File     = TexMixing[0].GetTexFileName();
	   Tex2File     = TexMixing[1].GetTexFileName();
	   Tex3File     = TexMixing[2].GetTexFileName();
	   TexVec       = TexMixing[3].GetTexFileName();
	   TexWaterFile = TexMixing[4].GetTexFileName();
   }

}
//====================================================================================================================
int CGlLandshaft::Create(TLandProperties Properties,CPoint MapSize)
{
	Free();
  
	LandInfo=Properties;
  	MassHeight=NULL;
	MassTexture=NULL;
    MassVNormal=NULL;
	LandInfo.DH=LandInfo.height/LandInfo.StepSize-1;
    LandInfo.DW=LandInfo.width/LandInfo.StepSize-1;
	GlMemoryPt=3001;

	MixingShader.LoadShader(RES_PATH+"Shaders\\MixingLand.vert",RES_PATH+"Shaders\\MixingLand.frag");  //загрузили шейдер смешивания
	MapShader.LoadShader(RES_PATH+"Shaders\\MapLand.vert",RES_PATH+"Shaders\\MapLand.frag"); //шейдер для карт
	WaterShader.LoadShader(RES_PATH+"Shaders\\Water.vert",RES_PATH+"Shaders\\Water.frag");
  if(LoadRawFileToMass(&MassHeight,LandInfo.LandFile,LandInfo.ElemSize)==false)
	  return -1;
  
  if(LandInfo.FilterAverage==true)
      FilterAverage(MassHeight,LandInfo.DW,LandInfo.DH);
  if(LandInfo.FilterTaling==true)
     FilterTailing(MassHeight,LandInfo.DW,LandInfo.DH);
  if(LandInfo.FilterFractal==true||LandInfo.FilterSmooth==true)  
     FilterDiamondSquareFractal(MassHeight,1.0f,LandInfo.DW,LandInfo.DH);

  //после всех изменений с высотой, выравниваем её в зависимости с известными LandInfo.Max/MinHeight
	double MaxNow=0,MinNow=0;
	for (int i = 0; i <LandInfo.DW; i++)
		for (int j = 0; j <LandInfo.DH; j++)
	    {
			if(MinNow>MassHeight[i][j])MinNow=MassHeight[i][j];
			if(MaxNow<MassHeight[i][j])MaxNow=MassHeight[i][j];
		}
  	for (int i = 0; i <LandInfo.DW; i++)
		for (int j = 0; j <LandInfo.DH; j++)
		MassHeight[i][j] =(int)(LandInfo.MaxHeight/MaxNow*(MassHeight[i][j] - MinNow)); //делаем растяжение/сжатие по высоте
	

   if(LandInfo.ElemSize==2)
   {
	   ScaleTexMixing.x=90.0/LandInfo.ScaleX;
	   ScaleTexMixing.y=90.0/LandInfo.ScaleY;
	   LandInfo.ScaleX=90.0/LandInfo.ScaleX;
	   LandInfo.ScaleY=90.0/LandInfo.ScaleY;
   }
   else
   {
	   ScaleTexMixing.x=LandInfo.ScaleX;
	   ScaleTexMixing.y=LandInfo.ScaleY;

   }

//Расчитываем географические данные:
 if(LandInfo.ElemSize==2)
 {

	CString LonStr,DesStr,Str;
	CString buf[2];
	Str=GetFileNameWithoutExp(LandInfo.LandFile);
  
	buf[0]=Str[0];
	Str.Delete(0,1);
	LonStr=Str.SpanIncluding("0123456789");
	Str.Delete(0,LonStr.GetLength());
	buf[1]=Str[0];
	Str.Delete(0,1);
	DesStr=Str;
	
	if(buf[1]=='W')
       LonStr="-"+LonStr;
	if(buf[0]=='S')
		DesStr="-"+DesStr;
	
	HgtGeoInfo.GeoCord.x=atof(DesStr);//тут надо переименовать название строк, а то непонятно
	HgtGeoInfo.GeoCord.y=atof(LonStr);
	HgtGeoInfo.Block.x=HgtGeoInfo.GeoCord.x+90.0;
	HgtGeoInfo.Block.y=HgtGeoInfo.GeoCord.y+180.0;

 } 
 else
  {
	HgtGeoInfo.GeoCord.x=190.0;
	HgtGeoInfo.GeoCord.x=190.0;
  }
 HgtGeoInfo.GlobMapSize.x=1200*360;
 HgtGeoInfo.GlobMapSize.y=1200*180;

  GenVNormals();          //высчитали вершинные нормали
  LoadLandToMemory();     //загружаем в память opengl
  InitFlag=true;          //отмечаем, что загрузка ландшафта прошла успешно
  this->MapSize=MapSize;  //размер карты
  return 1;
}
//====================================================================================================================
void CGlLandshaft::Render()
{ 

	if(SelectMapInd==0)
 {



    MixingShader.Use();
	MixingShader.SetUniformVec2("PictSize",TexMixing[3].PictWigth(),TexMixing[3].PictHeight());
    MixingShader.SetUniformVec2("Tex1Size",TexMixing[0].PictWigth()/TextureDetalLevel/ScaleTexMixing.x,
		                                   TexMixing[0].PictHeight()/TextureDetalLevel/ScaleTexMixing.y);
    MixingShader.SetUniformVec2("Tex2Size",TexMixing[1].PictWigth()/TextureDetalLevel/ScaleTexMixing.x,
		                                   TexMixing[1].PictHeight()/TextureDetalLevel/ScaleTexMixing.y);
	MixingShader.SetUniformVec2("Tex3Size",TexMixing[2].PictWigth()/TextureDetalLevel/ScaleTexMixing.x,
		                                   TexMixing[2].PictHeight()/TextureDetalLevel/ScaleTexMixing.y);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TexMixing[3].GetTexPt());


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, TexMixing[0].GetTexPt());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, TexMixing[1].GetTexPt());
 
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, TexMixing[2].GetTexPt());

  /*  glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, TexMixing[4].GetTexPt());
*/
	MixingShader.SetUniformInt("texVec",1);
	MixingShader.SetUniformInt("tex1",2);
	MixingShader.SetUniformInt("tex2",3);
	MixingShader.SetUniformInt("tex3",4);
	//MixingShader.SetUniformInt("texWater",5);
    glCallList(GlMemoryPt);
	MixingShader.Close();



	WaterShader.Use();
	  glActiveTexture(GL_TEXTURE1);
	  glBindTexture(GL_TEXTURE_2D, TexMixing[4].GetTexPt());
	  WaterShader.SetUniformInt("texWater",1);
	  glBegin(GL_QUADS);
	    glMultiTexCoord2f(GL_TEXTURE1,0,0);
		glVertex3f(0.0,0.0,0.0);
		glMultiTexCoord2f(GL_TEXTURE1,1,0);
		glVertex3f(0.0,0.0,LandInfo.height*LandInfo.ScaleY);
		glMultiTexCoord2f(GL_TEXTURE1,1,1);
		glVertex3f(LandInfo.width*LandInfo.ScaleX,0.0,LandInfo.height*LandInfo.ScaleY);
		glMultiTexCoord2f(GL_TEXTURE1,0,1);
		glVertex3f(LandInfo.width*LandInfo.ScaleX,0.0,0.0);
	  glEnd();
	WaterShader.Close();


	glActiveTexture(GL_TEXTURE0);
  }
  else
  if(SelectMapInd==-1)
  {
	 glColor3f(0.9,0.9,0.9);
   glCallList(GlMemoryPt);
  }
  else
  {
	  MapShader.Use();
	  pictCord.x=float(MapTextures[SelectMapInd-1].PictWigth());
      pictCord.y=float(MapTextures[SelectMapInd-1].PictHeight());
      MapShader.SetUniformVec2("PictSize",pictCord.x,pictCord.y);
 	  glActiveTexture(GL_TEXTURE1);
	  glBindTexture(GL_TEXTURE_2D,MapTextures[SelectMapInd-1].GetTexPt());
	  MapShader.SetUniformInt("texture",1);
	  glCallList(GlMemoryPt);
	  MapShader.Close();
	  glActiveTexture(GL_TEXTURE0);
  }

  /*	glBegin(GL_QUADS);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,LandInfo.height);
		glVertex3f(LandInfo.width,0.0,LandInfo.height);
		glVertex3f(LandInfo.width,0.0,0.0);
	glEnd();*/

}
//====================================================================================================================
void CGlLandshaft::LoadLandToMemory()
{
int i= 0, j = 0;                                 // Создаем пару переменных для перемещения по массиву
 const float Texcord=1.0;
 CVector3 p[4];
 GlMemoryPt=3001;

 glNewList(GlMemoryPt,GL_COMPILE); 

 ////////////////////////////////////////
 	// glColor3f(1.0,0.0,0.0);
 glPushMatrix();

	 glScalef(LandInfo.ScaleX,1.0,LandInfo.ScaleY);
	////////////////////////////////////////
 for(i=0;i<(LandInfo.height/LandInfo.StepSize)-2;i++)
	  for(j=0;j<(LandInfo.width/LandInfo.StepSize)-2;j++)
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
		 glMultiTexCoord2f(GL_TEXTURE0,0,0);
		 glMultiTexCoord2f(GL_TEXTURE1,0,0);
		 glMultiTexCoord2f(GL_TEXTURE2,0,0);
		 glNormal3f(MassVNormal[i][j].x,
			        MassVNormal[i][j].y,
					MassVNormal[i][j].z);   
	     glVertex3f(p[0].x, p[0].y, p[0].z);      
                                                        
	     glMultiTexCoord2f(GL_TEXTURE0,Texcord,0.0);
		   glMultiTexCoord2f(GL_TEXTURE1,Texcord,0.0);
		   glMultiTexCoord2f(GL_TEXTURE2,Texcord,0.0);
		 glNormal3f(MassVNormal[i][j+1].x,
			        MassVNormal[i][j+1].y,
					MassVNormal[i][j+1].z);   
         glVertex3f(p[1].x, p[1].y, p[1].z);     
         
         glMultiTexCoord2f(GL_TEXTURE0,0.0,Texcord);
		 glMultiTexCoord2f(GL_TEXTURE1,0.0,Texcord);
		 glMultiTexCoord2f(GL_TEXTURE2,0.0,Texcord);
		 glNormal3f(MassVNormal[i+1][j+1].x,
			        MassVNormal[i+1][j+1].y,
					MassVNormal[i+1][j+1].z);   
         glVertex3f(p[2].x, p[2].y, p[2].z);     
       
         glMultiTexCoord2f(GL_TEXTURE0,Texcord,Texcord);
		 glMultiTexCoord2f(GL_TEXTURE1,Texcord,Texcord);
		 glMultiTexCoord2f(GL_TEXTURE2,Texcord,Texcord);
		 glNormal3f(MassVNormal[i+1][j].x,
			        MassVNormal[i+1][j].y,
					MassVNormal[i+1][j].z);   
         glVertex3f( p[3].x, p[3].y, p[3].z);     
	
	glEnd();



	}
/*	glBegin(GL_QUADS);
		glVertex3f(0.0,0.0,0.0);
		glVertex3f(0.0,0.0,LandInfo.height);
		glVertex3f(LandInfo.width,0.0,LandInfo.height);
		glVertex3f(LandInfo.width,0.0,0.0);
	glEnd();*/
 glPopMatrix();

	glEndList(); 
}
//====================================================================================================================
void CGlLandshaft::GenVNormals()
{
    int i,j;
	int STEP_SIZE=LandInfo.StepSize;
	CVector3 Normals[6];//нормали всех 6 плоскостей
	CVector3 Versh[3];// вершины плоскости
	CVector3 NormalRes;// усредненная нормаль вершины
	bool **FlagNormalMap= new bool*[LandInfo.DH+1];
	CVector3 **PloskNormals= new CVector3*[LandInfo.DH+1];
	MassVNormal= new CVector3*[LandInfo.DH+1];
	for(int i=0;i<LandInfo.DH;i++)
	{
	   FlagNormalMap[i]= new bool[LandInfo.DW+1];
	   PloskNormals[i] = new CVector3[LandInfo.DW+1];
       MassVNormal[i]  = new CVector3[LandInfo.DW+1];
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
//====================================================================================================================
CVector3 CGlLandshaft::GenTrianglePNormal(CVector3 p1,CVector3 p2,CVector3 p3)
{
   CVector3 NVector;
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
//====================================================================================================================
bool CGlLandshaft::IsInit()
{
  return InitFlag;
}
//====================================================================================================================
WORD CGlLandshaft::sh(WORD x)
{
 WORD result;
  _asm
 {
   mov ax,x
   ror ax,8
   mov result,ax
 }
}

bool CGlLandshaft::LoadRawFileToMass(int  ***mass,CString FileName,int type)
{

if(type==1)
{
  	BYTE **pTempMap= new BYTE*[LandInfo.height+2];
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
  if(LandInfo.AutoCalcMaxMinHeight==true)
   {
	   LandInfo.MinHeight=0;
       LandInfo.MaxHeight=0;
       for(i=0;i<(LandInfo.height-5);i++)
	       for(j=0;j<(LandInfo.width-5);j++)
	   {
		   if(LandInfo.MinHeight>pTempMap[i][j]&&pTempMap[i][j]>-1000)LandInfo.MinHeight=pTempMap[i][j];
		   if(LandInfo.MaxHeight<pTempMap[i][j]&&pTempMap[i][j]<8800)LandInfo.MaxHeight=pTempMap[i][j];
	   }
   }

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
//*****************************************
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
	   //////
	   //////

   }
   if(LandInfo.AutoCalcMaxMinHeight==true)
   {
	   LandInfo.MinHeight=0;
       LandInfo.MaxHeight=0;
       for(i=0;i<(LandInfo.height-5);i++)
	       for(j=0;j<(LandInfo.width-5);j++)
	   {
		   pTempMap[i][j]=sh(pTempMap[i][j]);
		   if(LandInfo.MinHeight>pTempMap[i][j]&&pTempMap[i][j]>-1000)LandInfo.MinHeight=pTempMap[i][j];
		   if(LandInfo.MaxHeight<pTempMap[i][j]&&pTempMap[i][j]<8800)LandInfo.MaxHeight=pTempMap[i][j];
	   }
   }
   else
   {
      for(i=0;i<(LandInfo.height-5);i++)
	       for(j=0;j<(LandInfo.width-5);j++)
		   pTempMap[i][j]=sh(pTempMap[i][j]);
   }
  fclose(pFile);
  massH= new int*[LandInfo.DH+1];
  for(i=0;i<LandInfo.DH;i++)
	  massH[i]=new int[LandInfo.DW+1];
  float max=0,min=0;
  for(i=0;i<(LandInfo.DH);i++)
	  for(j=0;j<(LandInfo.DW);j++)
	  {
		massH[i][j]=int(pTempMap[i*LandInfo.StepSize][j*LandInfo.StepSize]);
		if(max<massH[i][j])max=massH[i][j];
		if(min>massH[i][j])min=massH[i][j];

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
//====================================================================================================================
template<class T> void CGlLandshaft::FilterTailing(T** &heights,int w, int h)
{

}
 //====================================================================================================================
template<class T> void CGlLandshaft::FilterAverage(T** &mass,int w, int h)
{
	
  int i,j;
  double matrix[8][8];
  double koef=34;
  int size=6,matrix_size=6;
  		  int k,p,k_i,p_i;
  double new_c;
 matrix[0][0]=1/koef; matrix[0][1]=1/koef;        matrix[0][2]=1/koef;  matrix[0][3]=1/koef; matrix[0][4]=1/koef; matrix[0][5]=1/koef;
 matrix[1][0]=1/koef; matrix[1][1]=1/koef;        matrix[1][2]=2/koef;  matrix[1][3]=2/koef; matrix[1][4]=1/koef;       matrix[1][5]=1/koef;
 matrix[2][0]=1/koef; matrix[2][1]=1/koef;        matrix[2][2]=0;       matrix[2][3]=0;      matrix[2][4]=1/koef;      matrix[2][5]=1/koef;
 matrix[3][0]=1/koef; matrix[3][1]=1/koef;        matrix[3][2]=0;       matrix[3][3]=0;      matrix[3][4]=1/koef;   matrix[3][5]=1/koef;
 matrix[4][0]=1/koef; matrix[4][1]=1/koef;        matrix[4][2]=2/koef;  matrix[4][3]=2/koef; matrix[4][4]=1/koef;   matrix[4][5]=1/koef;
 matrix[5][0]=1/koef; matrix[5][1]=1/koef;        matrix[5][2]=1/koef;  matrix[5][3]=1/koef; matrix[5][4]=1/koef;  matrix[5][5]=1/koef;
 for(i=0;i<w;i++)
	 for(j=0;j<h;j++)
	  {
           
       if(i<=7||j<=7||(i>=h-7)||(j>=w-7)) 
	   {
		   if(mass[i][j]>40)
          mass[i][j]=1;
		  continue;
	   }
	   if(mass[i][j]<1&&mass[i][j]>-3000)
		   continue;
          new_c=0;
           for(k=0;k<size;k++)
             for(p=0;p<size;p++)
			 {
                k_i=k-(matrix_size-1)/2;
                p_i=p-(matrix_size-1)/2;
                if(i+k_i<0) k_i=k_i*(-1);
                if(j+p_i<0) p_i=p_i*(-1);
				if(i+k_i >= h-1)  k_i=k_i*(-1);
				if(j+p_i >= w-1)  p_i=p_i*(-1);
				if(mass[i+k_i][j+p_i]>8000)mass[i+k_i][j+p_i]=1;
                new_c+=mass[i+k_i][j+p_i]*fabs(matrix[k][p]);
            }
			mass[i][j]=T(new_c);
	  }
	  
}
//====================================================================================================================
