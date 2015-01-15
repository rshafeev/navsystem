#include "StdAfx.h" 
#include "GLFrame.h"
#include "3DEditor.h"
#include "memory.h"
#include "geography.h"
const double PI = 3.1415;
IMPLEMENT_DYNAMIC(CGLFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CGLFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_WM_MOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
//	ON_WM_NCACTIVATE()
ON_WM_KEYDOWN()
ON_WM_SHOWWINDOW()
ON_WM_SETCURSOR()
//ON_WM_ACTIVATE()
ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()

CGLFrame::CGLFrame(CString FName)
{
    WindowType=2;//указываем, что окно типа 3d -карта

	GridFlag=false;
    screenBitSize=32;
	pDC=NULL;
	hDC=NULL;
	hRC=NULL;
	IsCreated=false;
	Camera.CameraType=0;
	Camera.CursorCamera.Angle.x=-332.0;
	Camera.CursorCamera.Angle.x=0;
	Camera.CursorCamera.Angle.y=-220.0;
	Camera.CursorCamera.Angle.z=0.0;
	Camera.CursorCamera.Pos.x=0.0;
	Camera.CursorCamera.Pos.y=5170.0;
	Camera.CursorCamera.Pos.z=0.0;
	Camera.CursorCamera.Center.x=0;
	Camera.CursorCamera.Center.y=0;
	Camera.CursorCamera.Center.z=-30;
	Camera.PressingLB=false;
	Camera.CursorCamera.R=2000;
	Camera.CursorCamera.Center.x=600;
	Camera.CursorCamera.Center.z=600;
   
	PosLight[0]=-15.0;
	PosLight[1]=10.0;
	PosLight[2]=-20.0;
	PosLight[3]=0.0;

    MapWidth=2000;
    MapHeight=2000;
	CursorPosition.x=MapWidth/2;
	CursorPosition.y=20;
	CursorPosition.z=MapHeight/2;
	MoveObj.move=false;
	Landshaft=NULL;
	ProjName=FName;
}
CGLFrame::~CGLFrame(void)
{
	/*for(unsigned int i=0;i<ModelsList.size();i++) 
	{
		delete ModelsList[i];

	}
	ModelsList.clear();
	*/
	IsCreated=false;

	FrameContainer->DelFromContainer(UnicIndex);

	pDC=NULL;
	hDC=NULL;
	hRC=NULL;

	IsCreated=false;
	if(Landshaft!=NULL)
		delete Landshaft;


}
//===================================================================================================================
CString CGLFrame::GetFNameFromDB(CString caption)
{
	CString FName="";
	CString sqlStr;
	sqlStr="select [Caption],[FName] from DATA WHERE Caption='"+caption;
	sqlStr+="'";
   TRY_ADO()
      CADORecordset rs;
  
      rs.SetConnection (&theApp.m_GlConnection);

	  rs.SetSQL (sqlStr);
      rs.SetOpenMode (true);
      rs.Open ();
 if(rs.IsEmpty()==true) return "";
 FName=(const char*)((_bstr_t)rs.GetField("FName"));
	  rs.Close();
     CATCH_ADO()

 return FName;
}
//===================================================================================================================
CVector2 CGLFrame::GetGeoCord(CPoint p)
{
  CVector2 result;
  result.x=190.0;
  result.y=190.0;

  if(Landshaft!=NULL)
  {
	  result=Landshaft->GetHgtGeoCord(p);
    //если тип сетки - hgt

  }

  return result;
}
//===================================================================================================================
void CGLFrame::Save(CString FName)
{
 CFileDialog FileDlg(false,"rsmap",NULL,OFN_NOLONGNAMES   ,"Список устройства|*.rsmap||");
if(FileDlg.DoModal()==IDOK)
{
	unsigned int i;
	vector<CString> FileData; //массив строк, запис. в файл
	FileData.push_back("2\n");
   //1)добавляем информацию о ландшафте
	if(Landshaft!=NULL&&Landshaft->IsInit()==true)
	{
		Landshaft->SaveLand(GetPuth(FileDlg.GetPathName()),&FileData,false);
	}
    //2) добавляем инфу о моделях
	GlContainer.GetSaveProtocol(&FileData,false);
	FileData.push_back(".end.");
 //запись в файл
	CStdioFile f;
	f.Open(FileDlg.GetPathName(),CFile::modeCreate|CFile::modeWrite);
	for(i=0;i<FileData.size();i++)
		f.WriteString(FileData[i]);
}
}
//===================================================================================================================
void CGLFrame::Load(CString FName)
{
	CString buf="",str1;
	CStdioFile F;
	F.Open(FName,CFile::modeRead);
	 F.ReadString(buf);
	 F.ReadString(buf);
	 bool f=false;
     if(buf=="Landshaft:")
	 {
		 TLandProperties Land;
		 if(Landshaft==NULL)
            Landshaft=new CGlLandshaft;
           F.ReadString(buf);//считали имя файла
		   Land.LandFile=GetPuth(FName)+"\\"+buf;
		
		   //ширина длина
           F.ReadString(buf);
           str1=buf.SpanExcluding(" ");
		   Land.width=atoi(str1);
		   buf.Delete(0,str1.GetLength()+1);
		   Land.height=atoi(buf);
           //масштаб
		   F.ReadString(buf);
           str1=buf.SpanExcluding(" ");
		   Land.ScaleX=atof(str1);
		   buf.Delete(0,str1.GetLength()+1);
		   Land.ScaleY=atof(buf);

		   F.ReadString(buf);
		   str1=buf.SpanExcluding(" ");
		   Land.MinHeight=atoi(str1);
		   buf.Delete(0,str1.GetLength()+1);
		   Land.MaxHeight=atoi(buf);

		   F.ReadString(buf);
			Land.ElemSize=atoi(buf);

            F.ReadString(buf);
			Land.StepSize=atoi(buf);

            F.ReadString(buf);
			Land.FilterTaling=atoi(buf);
            F.ReadString(buf);
			Land.FilterFractal=atoi(buf);
            F.ReadString(buf);
			Land.FilterAverage=atoi(buf);
            F.ReadString(buf);
			Land.FilterSmooth=atoi(buf);
            InitLand(Land);
			if(Landshaft->IsInit()==false)return;
			//111mixing
			F.ReadString(buf);
			CString str1,str2,str3,str4;
			F.ReadString(str1);
			F.ReadString(str2);
			F.ReadString(str3);
			F.ReadString(str4);
			str1=GetPuth(FName)+"\\"+str1;
			str2=GetPuth(FName)+"\\"+str2;
			str3=GetPuth(FName)+"\\"+str3;
			str4=GetPuth(FName)+"\\"+str4;
			if(str1!="-#- ")
			{
				
				Landshaft->AddMapMixing(str1,str2,str3,RES_PATH+"Land\\textureWater1.bmp",str4);
			}
			//textures
			F.ReadString(buf);
			while(buf!=".end."&&buf!="Models:")
			{
				F.ReadString(buf);
				if(IsFileExists(GetPuth(FName)+"\\"+buf))
			    	Landshaft->AddMap(GetPuth(FName)+"\\"+buf);

			}
			f=true;
	 }
	//читаем модели
	 //!!
	 if(f==false)
        F.ReadString(buf);//строка Models:
	 GlToolFrame->ListObj->DeleteAllItems();
     while(buf!=".end."&&buf!="")
	 {
		 CVector3 pos;
           F.ReadString(buf);
		   if(buf=="")break;
		   pos.x=0;pos.y=0;pos.z=0;
           CString FName=GetFNameFromDB(buf);
		   CString capt=buf;
            if(FName=="")break;
		   
		   
		   F.ReadString(buf);
		   str1=buf.SpanExcluding(" ");
		   pos.x=atof(str1);
		   buf.Delete(0,str1.GetLength()+1);
		   pos.y=atof(buf);
           str1=buf.SpanExcluding(" ");
		   buf.Delete(0,str1.GetLength()+1);
		   pos.z=atof(buf);
		   CPoint ind=GlContainer.LoadModel(RES_PATH+"\\Models\\"+GetFileNameWithoutExp(FName)+"\\"+FName,capt,pos);
            GlToolFrame->AddToObjList(capt);
           F.ReadString(buf);//pAlphaValue -не исп.

		   CVector3 pRotate;
		   F.ReadString(buf);
		   str1=buf.SpanExcluding(" ");
		   pRotate.x=atof(str1);
		   buf.Delete(0,str1.GetLength()+1);
		   pRotate.y=atof(buf);
		   buf.Delete(0,str1.GetLength()+1);
		   pRotate.z=atof(buf);
 
		   CVector3 ScaleKoef;
		   F.ReadString(buf);
		   str1=buf.SpanExcluding(" ");
		   ScaleKoef.x=atof(str1);
		   buf.Delete(0,str1.GetLength()+1);
		   ScaleKoef.y=atof(buf);
		   str1=buf.SpanExcluding(" ");
		   buf.Delete(0,str1.GetLength()+1);
		   ScaleKoef.z=atof(buf);
		   GlContainer.ChangeScale(ind,ScaleKoef);
           F.ReadString(buf);
		  
		//   break;
		/*	ProtocolStr->push_back(ModelsList[i].caption+"\n");+
			+buf.Format("%f %f %f \n",ModelsList[i].MassCoord[j].pPos.x,ModelsList[i].MassCoord[j].pPos.y,ModelsList[i].MassCoord[j].pPos.z);
			ProtocolStr->push_back(buf);
			+buf.Format("%f \n",ModelsList[i].MassCoord[j].pAlphaValue);
			+ProtocolStr->push_back(buf);
			=buf.Format("%f %f %f \n",ModelsList[i].MassCoord[j].pRotate.x,ModelsList[i].MassCoord[j].pRotate.y,ModelsList[i].MassCoord[j].pRotate.z);
			ProtocolStr->push_back(buf);
			-buf.Format("%f %f %f \n",ModelsList[i].MassCoord[j].ScaleKoef.x,ModelsList[i].MassCoord[j].ScaleKoef.y,ModelsList[i].MassCoord[j].ScaleKoef.z);
			ProtocolStr->push_back(buf);
		    ProtocolStr->push_back("#==#\n");*/

	 }
	 //!!
	 
	 /*

	 	//записываем информацию о ландшафте
	ProtocolStr->push_back("Landshaft:\n");+
	CString buf;
	ProtocolStr->push_back(GetFileName(LandInfo.LandFile)+"\n");+
	buf.Format("%d %d \n",LandInfo.MinHeight,LandInfo.MaxHeight);//мин-макс высота ландшафта+
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.ElemSize);//размер одного эл-та 1: raw(BYTE); 2: hgt(WORD)+
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.StepSize);//шаг построения сетки+
	ProtocolStr->push_back(buf);

	buf.Format("%d \n",LandInfo.FilterTaling);//активизация фильтра(сглаживание)
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.FilterFractal);//активизация фильтра(фрактальное построение гор)
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.FilterAverage);//активизация фильтра(усреднение матрицей 8*8)
	ProtocolStr->push_back(buf);
	buf.Format("%d \n",LandInfo.FilterSmooth);//активизация фильтра(сглаживание)
	ProtocolStr->push_back(buf);*/
	 F.Close();
}
//===================================================================================================================
void CGLFrame::InitLand(TLandProperties LandProperties)
{
	if(LandProperties.StepSize==0||LandProperties.height==0||LandProperties.width==0)
		return;
  	TLandProperties prop=LandProperties;
	CursorPosition.x=LandProperties.width/2;
	CursorPosition.z=LandProperties.height/2;
	GlContainer.SetPosition(CursorPosition); //задаем позицию курсора	
	
/*	prop.LandFile=FileName;
	prop.ElemSize=ElemSize;
	prop.height=width;
	prop.width=height;
	prop.StepSize=StepSize;
	prop.FilterTaling=FilterTaling;
	prop.FilterFractal=FilterFractal;
	prop.FilterAverage=FilterAverage;
	prop.FilterSmooth=FilterSmooth;*/
	if(Landshaft!=NULL)
		delete Landshaft;
	Landshaft = new CGlLandshaft();
	Landshaft->Create(prop,CPoint(1000,1000));
    
}
//===================================================================================================================
void CGLFrame::OnMove(int x, int y)
{
  CMDIChildWnd::OnMove(x, y);
//  DrawScene();

}
//**********************************************************************************************************
void CGLFrame::DestroyGL(GLvoid)
{
	wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
	ReleaseDC(pDC);
	
	pDC=NULL;
	hDC=NULL;
	hRC=NULL;
}
//**********************************************************************************************************
void CGLFrame::DrawScene()
{
/*	for(int i=0;i<ModelsList.size();i++)
      ModelsList[i]->Draw();
	*/
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(.25f, .25f, .75f, .25f);
	glLoadIdentity();                                                 // Сброс просмотра
	glRotatef(Camera.CursorCamera.Angle.x, 1.0f, 0.0, 0.0);
	glRotatef(Camera.CursorCamera.Angle.y-90, 0.0, 1.0f, 0.0);
	glRotatef(Camera.CursorCamera.Angle.z, 0.0, 0.0, 1.0f);
	glTranslatef(-Camera.CursorCamera.Pos.x,-Camera.CursorCamera.Pos.y,-Camera.CursorCamera.Pos.z);
	
	glLightfv(GL_LIGHT0, GL_POSITION, PosLight);
	
	if(Landshaft!=NULL&&Landshaft->IsInit()==true)
	  Landshaft->Render();                     //рисуем ландшафт

	GlContainer.RenderObjects();             //рисуем все объекты контейнера
    GlContainer.SetPosition(CursorPosition); //задаем позицию курсора	
	SwapBuffers(wglGetCurrentDC());

static long xCurPosOld, yCurPosOld;
	POINT point;
	GetCursorPos(&point);
	if(Camera.PressingLB==true)
	{
	CRect rectCur,rectWind;
    GetWindowRect(rectCur);
		if((point.x>=rectCur.right-3)||(point.x<=rectCur.left+3)||
		(point.y>=rectCur.bottom-3)||(point.y<=rectCur.top+3))
		        Camera.PressingLB=false;
		Camera.CursorCamera.Angle.x += (yCurPosOld-point.y)*0.17f;
		Camera.CursorCamera.Angle.y += (xCurPosOld-point.x)*0.17f;   
		
		Camera.CursorCamera.Pos.x =Camera.CursorCamera.R*cos((Camera.CursorCamera.Angle.y)/180.0*PI)+Camera.CursorCamera.Center.x;
        Camera.CursorCamera.Pos.z =Camera.CursorCamera.R*sin((Camera.CursorCamera.Angle.y)/180.0*PI)+Camera.CursorCamera.Center.z;
	}
	else
	if(Camera.PressingRB==true)
	{
	CRect rectCur,rectWind;
    GetWindowRect(rectCur);
		if((point.x>=rectCur.right-3)||(point.x<=rectCur.left+3)||
		(point.y>=rectCur.bottom-3)||(point.y<=rectCur.top+3))
		        Camera.PressingLB=false;
		
		//Camera.Angle.y += (xCurPosOld-point.x)*0.07f;   
	}
   	xCurPosOld = point.x;
 	yCurPosOld = point.y;	
    


}
//=======================================================================================================================
int CGLFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    
	InitGL();
	//InitLand();
   
    vector<CString> objStr;
	GlContainer.GetObjString(&objStr);
 	GlToolFrame->ReloadObjList(objStr);

	if(ProjName!="")
	Load(ProjName);

	FrameContainer->AddToContainer(this);
	return 0;
}
//=======================================================================================================================
bool CGLFrame::InitGL( GLvoid )// Начальные установки касаемо OpenGL
{	
	CRect rect;
	int pixelformat;
	pDC = GetDC();
	hDC = pDC->GetSafeHdc();
  static  PIXELFORMATDESCRIPTOR pfd=                              // pfd сообщает Windows каким будет вывод на экран каждого пикселя
  {
    sizeof(PIXELFORMATDESCRIPTOR),                              // Размер дескриптора данного формата пикселей
    1,                                                          // Номер версии
    PFD_DRAW_TO_WINDOW |                                        // Формат для Окна
    PFD_SUPPORT_OPENGL |                                        // Формат для OpenGL
    PFD_DOUBLEBUFFER,                                           // Формат для двойного буфера
    PFD_TYPE_RGBA,                                              // Требуется RGBA формат
    screenBitSize,                                              // Выбирается бит глубины цвета
    0, 0, 0, 0, 0, 0,                                           // Игнорирование цветовых битов
    0,                                                          // Нет буфера прозрачности
    0,                                                          // Сдвиговый бит игнорируется
    0,                                                          // Нет буфера накопления
    0, 0, 0, 0,                                                 // Биты накопления игнорируются
    32,                                                         // 32 битный Z-буфер (буфер глубины)
    0,                                                          // Нет буфера трафарета
    0,                                                          // Нет вспомогательных буферов
    PFD_MAIN_PLANE,                                             // Главный слой рисования
    0,                                                          // Зарезервировано
    0, 0, 0                                                     // Маски слоя игнорируются
  };

	if ((pixelformat = ChoosePixelFormat(pDC->GetSafeHdc(), &pfd)) == 0)
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}
	
	hRC = wglCreateContext(hDC);
	//ASSERT(hRC != NULL);
	wglMakeCurrent(hDC, hRC);

	GetClientRect(&rect);

 glShadeModel( GL_SMOOTH );                                   // Разрешить плавное цветовое сглаживание
 glClearDepth( 1.0f );                                        // Разрешить очистку буфера глубины
 glEnable( GL_DEPTH_TEST );                                   // Разрешить тест глубины
 glDepthFunc( GL_LEQUAL );									  // Тип теста глубины
 glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );		  // Улучшение в вычислении перспективы

 	CRect t;
	GetWindowRect(t);
////////////////////////////////////////
//освещение
	 glViewport( 0, 0, t.Width(), t.Height());                                   // Сброс текущей области вывода 
	 //GLfloat LightAmbient[] = {0.9, 0.9,0.9};
	 //GLfloat LightDiff[] = {0.9, 0.9, 0.9, 0.0};
	    GLfloat MaterialAmbient[] = {1.0, 1.0, 1.0, 0.0};
     /*GLfloat MaterialDiffuse[] = {0.9, 0.9, 0.9, 0.0 };//
     GLfloat MaterialSpecular[] = {0.9, 0.9, 0.9, 0.0 };//
     GLfloat MaterialShininess[] = {100.0};
     GLfloat MaterialEmis[] = {0.01, 0.01, 0.01, 0.0};*/
////////////////////////////////////////
	glMaterialfv(GL_FRONT, GL_AMBIENT, MaterialAmbient);
/*	glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, MaterialShininess);*/
 //	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiff);
/*	glMaterialfv(GL_FRONT, GL_EMISSION, MaterialEmis);
 	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);*/

//освещение
////////////////////////////////////////
 glEnable(GL_LIGHT0);
 glEnable(GL_LIGHTING);   
 glMatrixMode( GL_PROJECTION );                                       // Выбор матрицы проекций
 glLoadIdentity();                                                    // Сброс матрицы проекции
 gluPerspective( 45.0f, (GLfloat)t.Width()/(GLfloat)t.Height(), 1.1f,70500.0f ); // Вычисление соотношения геометрических размеров для окна
 glMatrixMode( GL_MODELVIEW );                                        // Выбор матрицы вида модели
 glLoadIdentity();                                                    // Сброс матрицы вида модели
 glEnable(GL_NORMALIZE);
 return true;												          // Инициализация прошла успешно
}
//**********************************************************************************************************
void CGLFrame::OnPaint()
{

	CPaintDC dc(this);
	dc.BitBlt(0,0,900,900,pDC,0,0,SRCCOPY);
		dc.Rectangle(0,0,60,10);
    DrawScene();
}

void CGLFrame::OnDestroy()
{

	CMDIChildWnd::OnDestroy();
    
	DestroyGL();
}

void CGLFrame::OnClose()
{

	CMDIChildWnd::OnClose();
}

void CGLFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	Camera.PressingLB=true;
	Camera.CursorCamera.Center.x=CursorPosition.x;
	Camera.CursorCamera.Center.z=CursorPosition.z;
    DrawScene();
	CMDIChildWnd::OnLButtonDown(nFlags, point);
}

void CGLFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	Camera.PressingLB=false;
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}

void CGLFrame::OnRButtonUp(UINT nFlags, CPoint point)
{
	Camera.PressingRB=false;

	CMDIChildWnd::OnRButtonUp(nFlags, point);
}

void CGLFrame::OnRButtonDown(UINT nFlags, CPoint point)
{
	Camera.PressingRB=true;

	CMDIChildWnd::OnRButtonDown(nFlags, point);
}

	
void CGLFrame::OnMouseMove(UINT nFlags, CPoint point)
{


	CMDIChildWnd::OnMouseMove(nFlags, point);
}

BOOL CGLFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	Camera.CursorCamera.R-=zDelta;
	Camera.CursorCamera.Pos.x =Camera.CursorCamera.R*cos((Camera.CursorCamera.Angle.y)/180.0*PI)+Camera.CursorCamera.Center.x;
    Camera.CursorCamera.Pos.z =Camera.CursorCamera.R*sin((Camera.CursorCamera.Angle.y)/180.0*PI)+Camera.CursorCamera.Center.z;

	DrawScene();
	return CMDIChildWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CGLFrame::OnSize(UINT nType, int cx, int cy)
{
	glViewport(0,0,cx,cy);
	CMDIChildWnd::OnSize(nType, cx, cy);
	DrawScene();
}


BOOL CGLFrame::OnNcActivate(BOOL bActive)
{

	return CMDIChildWnd::OnNcActivate(bActive);
}

//BOOL CGLFrame::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
//{
//
//	return CMDIChildWnd::OnWndMsg(message, wParam, lParam, pResult);
//}

void CGLFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
	if(Landshaft!=NULL&&Landshaft->IsInit()==true)
 		Landshaft->SetTextureDetalization(1);
    if(nChar==99)
	{
		Camera.CursorCamera.Pos.y-=40.0;
		DrawScene();           
	}
	else
    if(nChar==105)
	{
		Camera.CursorCamera.Pos.y+=40.0;
		DrawScene();           
	}else
	if(nChar==122)
	{
		if(Landshaft!=NULL&&Landshaft->IsInit()==true)
		Landshaft->NextMap();
		DrawScene();
	}
	else
	if(nChar==123)
	{
		/*if(Landshaft!=NULL&&Landshaft->IsInit()==true)
		Landshaft->BackMap();*/

	}
	if(nChar==37)
	{
		CursorPosition.x-=8;
		DrawScene();
	}
	else
	if(nChar==38)
	{
        CursorPosition.z-=8;
		DrawScene();
	}
	else
	if(nChar==39)
	{
		CursorPosition.x+=8;
		DrawScene();
	}
	else
	if(nChar==40)
	{
        CursorPosition.z+=8;
		DrawScene();
	}
	else
	if(nChar==36)
	{
       CursorPosition.y+=5;
	   DrawScene();
	}
	else
    if(nChar==35)
	{
       CursorPosition.y-=5;
	   DrawScene();

	}
	Camera.CursorCamera.Center.x=CursorPosition.x;
	Camera.CursorCamera.Center.z=CursorPosition.z;
/*	if(Landshaft!=NULL&&Landshaft->IsInit())
		CursorPosition.y=Landshaft->GetHeightForPoint(CPoint(CursorPosition.x,CursorPosition.z))+5;
*/	if(MoveObj.move==true)
	{
		GlContainer.ChangeObjPos(MoveObj.ind,CursorPosition);
		DrawScene();
	}

    CString CursorStr;
	CursorStr.Format("x:%f y: %f z: %f",CursorPosition.x,CursorPosition.y,CursorPosition.z);
	MainForm->m_StatusBar.SetPaneText(3,(LPCSTR)CursorStr);

	CVector2 GeoCord=GetGeoCord(CPoint(CursorPosition.x,CursorPosition.z));   
	CursorStr.Format("Широта:%f Долгота: %f",GeoCord.x,GeoCord.y);
    MainForm->m_StatusBar.SetPaneText(2,(LPCSTR)CursorStr);
	CString De=ConvertGeoDoubleToStr(GeoCord.x);
	CString Lo=ConvertGeoDoubleToStr(GeoCord.y);
	CursorStr.Format("Широта:%s Долгота: %s",De,Lo);
	MainForm->m_StatusBar.SetPaneText(1,(LPCSTR)CursorStr);

	CFrameContainer::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGLFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameContainer::OnShowWindow(bShow, nStatus);

	DrawScene();
}

BOOL CGLFrame::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	DrawScene();
    if(MainForm->Menu.DocumentView==true)
		ViewDocFrame->SetListBoxPos(UnicIndex);
	FrameContainer->SetActive(UnicIndex);

	return CFrameContainer::OnSetCursor(pWnd, nHitTest, message);
}

void CGLFrame::OnCaptureChanged(CWnd *pWnd)
{
  SetFocus();
	CFrameContainer::OnCaptureChanged(pWnd);
}
