#include "StdAfx.h"
#include "GlContainer.h"
#include "Texture.h"
#include "memory.h"
const int MAX_OBJ=3000;

CGlContainer::CGlContainer(void)
{
	int i;
    FreeUnicObjInd.resize(MAX_OBJ);
    for(i=0;i<MAX_OBJ;i++)
        FreeUnicObjInd[i]=i+1;
	GridModel.ModelMemory=-1;
	selected_ind.x=-1;
	selected_ind.y=-1;
}

CGlContainer::~CGlContainer(void)
{

}
//============================================================================================================================
bool CGlContainer::GetSaveProtocol(vector<CString> *ProtocolStr,bool rewrite)
{
	unsigned int i,j;
	CString buf;
	//проходим по всем объектам и записываем инфу о них в  ProtocolStr
	ProtocolStr->push_back("Models:\n");
	for(i=0;i<ModelsList.size();i++)
	{
		for(j=0;j<ModelsList[i].MassCoord.size();j++)
		{
			ProtocolStr->push_back(ModelsList[i].caption+"\n");
			buf.Format("%f %f %f \n",ModelsList[i].MassCoord[j].pPos.x,ModelsList[i].MassCoord[j].pPos.y,ModelsList[i].MassCoord[j].pPos.z);
			ProtocolStr->push_back(buf);
			buf.Format("%f \n",ModelsList[i].MassCoord[j].pAlphaValue);
			ProtocolStr->push_back(buf);
			buf.Format("%f %f %f \n",ModelsList[i].MassCoord[j].pRotate.x,ModelsList[i].MassCoord[j].pRotate.y,ModelsList[i].MassCoord[j].pRotate.z);
			ProtocolStr->push_back(buf);
			buf.Format("%f %f %f \n",ModelsList[i].MassCoord[j].ScaleKoef.x,ModelsList[i].MassCoord[j].ScaleKoef.y,ModelsList[i].MassCoord[j].ScaleKoef.z);
			ProtocolStr->push_back(buf);
		    ProtocolStr->push_back("#==#\n");
	    }
	}

  return true;
}
//============================================================================================================================
void CGlContainer::SelectObj(CPoint ind)
{
   selected_ind=ind;
}
//============================================================================================================================
CVector3 CGlContainer::GetScale(CPoint index)
{
  
  if(index.x>0&&index.y<(int)ModelsList.size()&&index.y>0&&index.y< ModelsList[index.x].MassCoord.size())
	  return ModelsList[index.x].MassCoord[index.y].ScaleKoef;
   else
   {
	   CVector3 result;

	   result.x=0.0;
	   result.y=0.0;
	   result.z=0.0;
     return result;
   }
}
//==================================================
CVector3 CGlContainer::GetPosition(CPoint index)
{
	if(index.x>0&&index.y<(int)ModelsList.size()&&index.y>0&&index.y< ModelsList[index.x].MassCoord.size())
		return ModelsList[index.x].MassCoord[index.y].pPos;
   CVector3 p;	
   p.x=0.0;p.y=0.0;p.z=0.0;
   return p;
}
//============================================================================================================================
void CGlContainer::Destroy()
{
	FreeUnicObjInd.clear();
	for(unsigned int i=0;i<ModelsList.size();i++)
	{
		 
		if(ModelsList[i].ModelMemory!=-1)
			glDeleteLists(ModelsList[i].ModelMemory,1);
	}
	ModelsList.clear();
	delete this;
}
//============================================================================================================================
void CGlContainer::ChangeScale(CPoint ind, CVector3 scale)
{
	//if(ind.x>=0&&ind.y<(int)ModelsList.size()&&ind.y>=0&&ind.y< ModelsList[ind.x].MassCoord.size())
		ModelsList[ind.x].MassCoord[ind.y].ScaleKoef=scale;
 
}
//============================================================================================================================
void CGlContainer::ChangeObjPos(CPoint ind,CVector3 position)
{
	if((ind.x>=0)&&(ind.x<ModelsList.size())&&(ind.y>=0)&&(ind.y< ModelsList[ind.x].MassCoord.size()) )
		ModelsList[ind.x].MassCoord[ind.y].pPos=position;
}
//============================================================================================================================
void CGlContainer::ShowGrid(bool show,int Width,int Height,int Step)
{
	//ищем в списке объектов объект "Сетка: 1"
	
	
	//glColor3ub(0, 255, 0);
	if(show==true&&GridModel.ModelMemory==-1) //просят показать сетку, в памяти её нет, поэтому добавляем
	{
		
		GridModel.caption="Сетка";
		GridModel.type=1;
		GridModel.ScaleKoef.x=1.0;
		GridModel.ScaleKoef.y=1.0;
		GridModel.ScaleKoef.z=1.0;
		GridModel.Pos.x=0.0;
		GridModel.Pos.y=0.0;
		GridModel.Pos.z=0.0;
	
		GridModel.ModelMemory=FreeUnicObjInd[0];
		FreeUnicObjInd.erase(FreeUnicObjInd.begin());

		glNewList(GridModel.ModelMemory,GL_COMPILE);
		glDisable(GL_LIGHTING);
		glEnable(GL_LINE_SMOOTH_HINT); 
    	   glColor3ub(30, 200, 120);
           // Сетка 10х10 вдоль осей X и Z
	       for(int x = 0; x <= Width; x += Step)
	       {
		     glBegin(GL_LINES);
 			  // Горизонтальные линии (вдоль X)
			   glVertex3f(0, 0, (GLfloat)x);
			   glVertex3f(Height, 0, (GLfloat)x);
             glEnd();
 	       }
		   for(int x = 0; x <= Height; x += Step)
	       {
		     glBegin(GL_LINES);
 			   // Вертикальные линии (вдоль Z)
			   glVertex3f((GLfloat)x, 0,0);
			   glVertex3f((GLfloat)x, 0, Width);
 		     glEnd();
 		     
	       }

		 glEnable(GL_LIGHTING);
		 glEndList();
	}
	else
	if(show==false&&GridModel.ModelMemory!=-1)
	{
		FreeUnicObjInd.push_back(GridModel.ModelMemory);
		glDeleteLists(GridModel.ModelMemory,1);
		GridModel.ModelMemory=-1;
	}
		//glColor3ub(255, 255, 255);

}
//============================================================================================================================
int CGlContainer::GetGridInd()
{
   unsigned int i,n=ModelsList.size();
for(i=0;i<n;i++)
	   if(ModelsList[i].type==1)
          return i;  
  return -1;
}
//============================================================================================================================
void CGlContainer::RenderObjects()
{
     unsigned int i,j,n=ModelsList.size();
const float LightDiffS[4]={0.1, 0.2, 0.7, 0.0 };
const float LightDiff [4]={0.9, 0.9, 0.9, 0.0 };
glEnable(GL_TEXTURE_2D);
TModelCord *ModelCord;
for(i=0;i<n;i++)
   {   
	 /*  if(selected_ind==i) 
	   {
           continue;
	   }	*/
	   for(j=0;j<ModelsList[i].MassCoord.size();j++)
	   {
		   if(selected_ind.x==i&&selected_ind.y==j) continue;
		   ModelCord=&ModelsList[i].MassCoord[j];
		   glPushMatrix();
		   glTranslated(ModelCord->pPos.x,ModelCord->pPos.y,ModelCord->pPos.z);
		   glScaled(ModelCord->ScaleKoef.x,ModelCord->ScaleKoef.y,ModelCord->ScaleKoef.z);
	       glCallList(ModelsList[i].ModelMemory);
	       glPopMatrix();
	    }
   }

if(selected_ind.x!=-1&&selected_ind.y!=-1)
  {
	       i=selected_ind.x;
		   j=selected_ind.y;
           glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffS);
		   ModelCord=&ModelsList[i].MassCoord[j];
	       glPushMatrix();
		   glTranslated(ModelCord->pPos.x,ModelCord->pPos.y,ModelCord->pPos.z);
		   glScaled(ModelCord->ScaleKoef.x,ModelCord->ScaleKoef.y,ModelCord->ScaleKoef.z);
	       glCallList(ModelsList[i].ModelMemory);
	       glPopMatrix();
  }	 
   if(GridModel.ModelMemory!=-1)
	   glCallList(GridModel.ModelMemory);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiff);

}
//============================================================================================================================
void CGlContainer::SetPosition(CVector3 position)
{
	//glDisable(GL_COLOR_MATERIAL);
				// Иначе выключим текстуры
		//	glDisable(GL_TEXTURE_2D);
			// И сбросим цвет на нормальный
		//	glColor3ub(255, 255, 255);  
	glBindTexture(GL_TEXTURE_2D,0);
	glDisable(GL_LIGHTING);
   	       glColor3ub(255, 255, 0);
     	   glLineWidth(2.5);
 	 	   glBegin(GL_LINES);
		      glVertex3f(position.x-300,position.y, position.z);
			  glVertex3f(position.x+300,position.y, position.z);
 			  glVertex3f(position.x,position.y, position.z);
			  glVertex3f(position.x,position.y+ 400,position.z);
 			  glVertex3f(position.x, position.y, position.z-300);
			  glVertex3f(position.x, position.y, position.z+300);
 	        glEnd();
 	        glColor3ub(255, 255, 255);
 	        glLineWidth(1);
	glEnable(GL_LIGHTING);
   //  glEnable(GL_COLOR_MATERIAL);

}
//============================================================================================================================
CPoint CGlContainer::LoadModel(CString FileName,CString Caption,CVector3 pos)
{
	t3DModel g_3DModel;
	 GLfloat ColorMaterial;
	 TModelCord MassCoord;
	 int i,j;
	 int koef=22;
	 int whichVertex;
	 int   g_ViewMode	= GL_TRIANGLES;		// По умолчанию режим рендера - GL_TRIANGLES
	 CTexture textures[2000];
	 int ind=-1;
     
	 MassCoord.pPos=pos;
	 MassCoord.ScaleKoef.x=1.0;
	 MassCoord.ScaleKoef.y=1.0;
	 MassCoord.ScaleKoef.z=1.0;
	 
	 for(i=0;i<ModelsList.size();i++)
		 if(ModelsList[i].caption==Caption)
		 {ind=i; break;}
if(ind!=-1)
{
	ModelsList[ind].MassCoord.push_back(MassCoord);
}
else
{
	 TStaticModel *ModelAdd=new TStaticModel;
	 ModelAdd->caption= "стул";
	 ModelAdd->type   = 2;
	 ModelAdd->caption=Caption;
	 ModelAdd->ModelMemory=FreeUnicObjInd[0];
	 ModelAdd->MassCoord.push_back(MassCoord);
	 FreeUnicObjInd.erase(FreeUnicObjInd.begin());
	if(g_Load3ds.Import3DS(&g_3DModel,(LPCSTR)FileName)==true)
	{
	    for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	    {
		 if(strlen(g_3DModel.pMaterials[i].strFile) > 0)// Проверяем, есть ли в загруженном материале имя файла
		 {
			textures[i].InitTexture(GetPuth(FileName)+"\\"+g_3DModel.pMaterials[i].strFile);// Используем имя файла для загрузки битмапа с текстурным ID (i).
		 }
		  g_3DModel.pMaterials[i].texureId = i; // Устанавливаем ID текстуры для этого материала
	    }
	ModelsList.push_back(*ModelAdd);
	glNewList(ModelAdd->ModelMemory,GL_COMPILE);
    glEnable(GL_COLOR_MATERIAL);		
	for(i = 0; i < g_3DModel.numOfObjects; i++) // Так как мы знаем число обьектов в нашей модели, проходим через каждый из них
	{
		glPushMatrix();
		if(g_3DModel.pObject.size() <= 0) break;		// Убедимся, что передан верный обьект
		t3DObject *pObject = &(g_3DModel.pObject[i]);// Получим текущий обьект
		g_3DModel.currentFrame =0;
		AnimateModel(&g_3DModel, pObject);// Анимируем модель
		if(pObject->bHasTexture)                                  // Проверим, имеет ли обьект тексурную карту, если да - биндим на него текстуру
		{
			glEnable(GL_TEXTURE_2D);                              // Включаем текстуры
            glColor3f(0.9,0.9,0.9);
  //          GLfloat MaterialAmbient[] = {0.9, 0.9, 0.9, 0.0};
//	        glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
	        glBindTexture(GL_TEXTURE_2D, textures[pObject->materialID].GetTexPt());  // Биндим текстурную карту на обьект по его materialID
		} 
		else
		{
			
			   glDisable(GL_TEXTURE_2D);// Иначе выключим текстуры
			   glColor3f(0.87,0.87,0.87);
		}
		// Начинаем отрисовку в выбранном режиме
		glBegin(g_ViewMode);	// Рисуем обьекты (треугольники или линии)
			for(j = 0; j < pObject->numOfFaces; j++)// Проходим через все полигоны обьекта и рисуем их
			{
				for(whichVertex = 0; whichVertex < 3; whichVertex++)// Проходим через каждый угол треугольника и рисуем его
				{
					int index = pObject->pFaces[j].vertIndex[whichVertex];// Получаем индекс для каждой точки полигона
					glNormal3f(pObject->pNormals[ index ].x, 
					pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);// Передаём OpenGL нормаль этой вершины
					if(pObject->bHasTexture)// Если обьект имеет текстуру, передаем текст. координаты
					{
 						if(pObject->pTexVerts)
							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
					else
					{
 						// Убедимся, что у нас есть верный материал/цвет, привязанный
						// к обьекту. Вообще практически всегда к обьекту привязан как 
						// минимум цвет, но просто на всякий случай проверим это.
						// Если размер материала минимум 1, и materialID != -1, 
						// материал верен.
						if((g_3DModel.pMaterials.size()>0)&&(pObject->materialID >= 0))
						{
							// Получаем и устанавливаем цвет обьекта, если он
							// не имеет текстуры
							BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;
							// Применяем цвет к модели
							glColor3ub(pColor[0], pColor[1], pColor[2]);
						}
					}
                    
					// Передаём текущую вершину обьекта
					if(index<0||pObject->numOfVerts<index) continue;
					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y,
								pObject->pVerts[ index ].z);
				}
			}
 		    glEnd();
			glPopMatrix();
	  }
      glDisable(GL_COLOR_MATERIAL);
      GLfloat MaterialAmbient[] = {0.87, 0.87, 0.87, 0.0};
	  glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
      glEndList();
    }
 }
if(ind!=-1)
  return CPoint(ind,ModelsList[ind].MassCoord.size()-1);
else
{
  ind=ModelsList.size()-1;
  return CPoint(ind,ModelsList[ind].MassCoord.size()-1);
}

}
//============================================================================================================================
void CGlContainer::AnimateModel(t3DModel *pModel, t3DObject *pObject)
{
	// Это самое главное изменение в этом файле. Эта функция контролирует
	// изменения обьекта для каждого кадра анимации. В зависимости от кадра
	// мы перемещаем, затем масштабируем, затем вращаем обьект на необходимые
	// значения. Для каждого обьекта есть список значений для каждого кадра.
 
	// Получаем текущую позицию для этого кадра
	if(pModel->currentFrame>=(int)pObject->vPosition.size()||pModel->currentFrame<0) return;

	CVector3 vPosition = pObject->vPosition[pModel->currentFrame];
 
	// Перемещаем обьект
	glTranslatef(vPosition.x, vPosition.y, vPosition.z);
 
	// Получаем значение scale для этого кадра
	CVector3 vScale = pObject->vScale[pModel->currentFrame];
 
	// Масштабируем обьект на значение для этого кадра
	//glScalef(vScale.x, vScale.y, vScale.z);
 
	// Здесь всё немного по другому. Создаём цикл через все
	// предыдущие кадры, пока не достигнем первого кадра, и
	// вызываем для каждого кадра glRotatef(). Это будет вращать 
	// обьект на маленькие углы, пока не получится значение текущего
	// кадра. Если бы мы использовали матрицы (но я решил не перегружать
	// этот урок), можно было бы просто умножить матрицу вращения
	// на текущую матрицу. 
	// Устанавливаем "i" в 1, т.к. мы игнорируем нулевой кадр анимации.
	for(int i = 1; i <=1/* pModel->currentFrame*/; i++)
	{
		// Получаем текущий угол и ось вращения для текущего кадра
		CVector3 vRotation = pObject->vRotation[i];
		float rotDegree = pObject->vRotDegree[i];
 
		// Проверяем, не равен ли угол вращения нулю
		if(rotDegree)
		{
			// Вращаем обьект на угол для текущего кадра
			glRotatef(rotDegree, vRotation.x, vRotation.y, vRotation.z);
		}
	}
}
//============================================================================================================================
void CGlContainer::GetObjString(vector<CString> *StrList)
{
	
	for(unsigned int i=0;i<ModelsList.size();i++)
	{
		StrList->push_back(ModelsList[i].caption);
	}
}
//============================================================================================================================