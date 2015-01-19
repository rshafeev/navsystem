#include "stdafx.h"
#include "3DLib.h"
#include "3ds.h"
#include <math.h>
int gBuffer[90000] = {0};	// Используется для чтения нежелательных данных
 
// В этом файле находится весь код, необходимый для загрузки файлов .3ds.
// Как оно работает: вы загружаете chunk, затем проверяете его ID.
// В зависимости от его ID, загружаете информацию, хранящуюся в chunk-е.
// Если вы не хотите читать эту информацию, читаете дальше неё.
// Вы знаете, как много байт нужно пропустить, так как каждый chunk хранит
// свою длинну в байтах.
 


///////////////////////////////// ROUND FLOAT """"""""""\\*
/////
/////	Новая функция: округляет float до нуля, если он меньше 0.001 или больше -0.001
/////
///////////////////////////////// ROUND FLOAT """"""""""\\*
 
float RoundFloat(float number)
{
 
	// По какой-то странной причине когда читаются очень маленькие числа,
	// они сохраняются с какой-то погрешностью, которая некорректно обрабатывается
	// OpenGL. Так что мы просто округлим их до 0. Разницы вы не заметите, а
	// работать будет лучше.
 
	// Если передано очень маленькое число, устанавливаем его в 0
	if(number > 0 && number <  0.001f) number = 0;
	if(number < 0 && number > -0.001f) number = 0;
 
	// Возвращаем изменённый или неизменённый float
	return number;
}


 
///////////////////////////////// CLOAD3DS """"""""""\\*
/////
/////	Конструктор инициализирует данные tChunk
/////
///////////////////////////////// CLOAD3DS """"""""""\\*

CLoad3DS::CLoad3DS()
{
	m_CurrentChunk = new tChunk;
	m_TempChunk = new tChunk;
	m_CurrentObject=NULL;
}
CLoad3DS::~CLoad3DS(void)
{
    CleanUp();
}
///////////////////////////////// IMPORT 3DS """"""""""\\*
/////
/////	Вызывается клиентом для открытия, чтения и затем очистки .3ds
/////
///////////////////////////////// IMPORT 3DS """"""""""\\*
 
bool CLoad3DS::Import3DS(t3DModel *pModel,const char *strFileName)
{
	char strMessage[2550] = {0};
    pModel->numOfMaterials = 0;
	pModel->numOfObjects   = 0;
	pModel->numberOfFrames = 0;
	// Открываем .3ds файл
	m_FilePointer = fopen(strFileName, "rb");
 
	// Убедимся, что указатель на файл верен (мы открыли файл)
	if(!m_FilePointer)
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
 
	// Открыв файл, нужно прочитать хидер файла, чтобы убедится, что это 3DS.
	// Если это верный файл, то первым ID chunk-а будет PRIMARY
 
	// Читаем первый chunk файла, чтобы убедится, что это 3DS
	ReadChunk(m_CurrentChunk);
 
	// Убедимся, что это 3DS
	if (m_CurrentChunk->ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
 	// Теперь начинаем чтение данных. ProcessNextChunk() - рекурсивная функция
 	// Начинаем загрузку обьектов вызовом рекурсивной функции
	ProcessNextChunk(pModel, m_CurrentChunk);
 	// После прочтения всего файла нам нужно рассчитать нормали вершин
	ComputeNormals(pModel);
 	fclose(m_FilePointer);
 	return true;
}
 
///////////////////////////////// CLEAN UP """"""""""\\*
/////
/////	Функция чистит всю занятую память и закрывает файл
/////
///////////////////////////////// CLEAN UP """"""""""\\*
 
void CLoad3DS::CleanUp()
{

	
	delete m_CurrentChunk;
	delete m_TempChunk;
}
 
 
///////////////////////////////// PROCESS NEXT CHUNK""""""""""\\*
/////
/////	Функция читает главную секцию файла, затем рекурсивно идёт глубже
/////
///////////////////////////////// PROCESS NEXT CHUNK""""""""""\\*

void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	t3DObject newObject = {0};		// Используется для добавления обьекта в список
	tMaterialInfo newTexture = {0};		// Используется для добавления материала
	int version = 0;                    // Версия файла
 
	m_CurrentChunk = new tChunk;		// Текущий chunk для загрузки
 
	// Продолжаем читать подсекции, пока не дойдем до общей длинны файла.
	// После чтения ЧЕГО УГОДНО, увеличиваем прочитанные байты и сравниваем
	// их с общей длинной.
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Читаем следующий chunk
		ReadChunk(m_CurrentChunk);
 
		// Получаем chunk ID
		switch (m_CurrentChunk->ID)
		{
		case VERSION:			// Версия файла
 
			// Читаем версию файла и добавляем прочитанные байты в переменную bytesRead
			m_CurrentChunk->bytesRead += fread(&version, 1, m_CurrentChunk->length - 
				m_CurrentChunk->bytesRead, m_FilePointer);
 
			// Если версия файла больше 3, выведем предупреждение, что могут
			// возникнуть проблемы.
			if (version > 0x03)
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly", "Warning", MB_OK);
			break;

		case KEYFRAME:
 
			// Здесь мы начинаем читать информацию о ключевых кадрах.
			// Она читается в КОНЦЕ файла. 
 
			// Рекурсивно считываем chunk
			ProcessNextKeyFrameChunk(pModel, m_CurrentChunk);
 
			// Увеличиваем счетчик считанных байт
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, m_CurrentChunk->length - 
						m_CurrentChunk->bytesRead, m_FilePointer);
//			MessageBox(NULL,"test","e",MB_OK);
			break;
 
		case OBJECTINFO:		// Содержит версию меша
			// Этот chunk содержит версию меша. Также это заголовок для chunk-ов MATERIAL
			// и OBJECT. Отсюда мы начинаем читать информацию материалов и обьектов.
 
			// Читаем следующий chunk
			ReadChunk(m_TempChunk);
 
			// Получаем версию меша
			m_TempChunk->bytesRead += fread(&version, 1, m_TempChunk->length - 
				m_TempChunk->bytesRead, m_FilePointer);
 
			// Увеличиваем bytesRead на число прочитанных байт
			m_CurrentChunk->bytesRead += m_TempChunk->bytesRead;
 
			// Переходим к следующему chunk-у, это будет MATERIAL, затем OBJECT
			ProcessNextChunk(pModel, m_CurrentChunk);
			break;
		case MATERIAL:			// Содержит информацию о материале
 
			// Этот chunk - хидер для информации о материале
 
			// Увеличиваем число материалов
			pModel->numOfMaterials++;
 
 
			// Добавляем пустую структуру текстуры в наш массив текстур.
			pModel->pMaterials.push_back(newTexture);
 
			// Вызываем функцию, обрабатывающую материал
			ProcessNextMaterialChunk(pModel, m_CurrentChunk);
			break;
 
		case OBJECT:			// Хранит имя читаемого обьекта
 
			// Этот chunk - хидер для chunk-ов, хранящих информацию обьекта.
			// Также он хранит имя обьекта.
 
			// Увеличиваем счетчик обьектов
			pModel->numOfObjects++;
 
			// Добавляем новый элемент tObject к списку обьектов
			pModel->pObject.push_back(newObject);
 
			// Инициализируем обьект и все его данные
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));
 
			// Получаем и сохраняем имя обьекта, затем увеличиваем счетчик прочитанных байт
			m_CurrentChunk->bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
 
			// Переходим к чтению оставшейся информации обьекта
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_CurrentChunk);
			break;
 
		default:
 
			// Остальные chunk-и, которые нам не нужны, будут обработаны здесь. Нам
			// всё ещё нужно прочитать в "мусорную" переменную неизвестные или игнорируемые
			// chunk-и и увеличить счетчик прочитанных байт.
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, m_CurrentChunk->length - 
				m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
 
		// Прибавим прочитанные байты последнего chunk-а к счетчику
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}
 
///////////////////////////////// PROCESS NEXT KEYFRAME CHUNK """"""""""\\*
/////
/////	Эта функция и сохраняет данные анимации.
/////	Мы получаем имя обьекта, для которого читается анимация, число анимированных кадров,
/////	точку опоры, размерность, вращение и перемещение для ключевых кадров. Затем мы
/////	интерполируем эти данные между ключевыми кадрами.
/////
///////////////////////////////// PROCESS NEXT KEYFRAME CHUNK """"""""""\\*
 
void CLoad3DS::ProcessNextKeyFrameChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
//	MessageBox(NULL,"test","e",MB_OK);
	char strKeyFrameObject[500] = {0};	// Хранит имя текущего обьекта
	float temp = 0.0f;			// Для смены координат y и z точки опоры
 
	// Инициализируем новый chunk для работы с ним
	m_CurrentChunk = new tChunk;
 
	// Продолжаем читать секции, пока не достигаем конца подсекции
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{

//	MessageBox(NULL,"yes","e",MB_OK);

		// Читаем следующий chunk
		ReadChunk(m_CurrentChunk);
 
		// Проверяем, что за секцию мы читаем
		switch (m_CurrentChunk->ID)
		{
		case KEYFRAME_MESH_INFO:	// Говорит нам, что описывается информация для нового обьекта

			// Это сообщает нам, что читается анимация для следующего обьекта, так что
			// нужно рекурсивно заново читать chunk
			ProcessNextKeyFrameChunk(pModel, m_CurrentChunk);
			break;
 
		case KEYFRAME_OBJECT_NAME:	// Хранит имя текущего обьекта
 
			// Получаем имя обьекта, для которого читаются данные анимации
			m_CurrentChunk->bytesRead += GetString(strKeyFrameObject);
 
			// Теперь, узнав, для какого обьекта читаются данные, устанавливаем m_CurrentObject.
			// Таким образом у мы получаем указатель на обьект модели, содержащий данные
			// анимации.
			SetCurrentObject(pModel, strKeyFrameObject);
 
			// Читаем 2 переданных флага и иерархический номер (3 short-а - не используется
			// в этом уроке).
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, 
					m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
 
		case KEYFRAME_START_AND_END:		// Этот chunk хранит начальный и конечный кадры.
 
			// Читаем начальный и конечный кадры. Просто переписываем начальный кадр, т.к.
			// подразумевается, что мы всегда начинаем с 0 кадра
			m_CurrentChunk->bytesRead += fread(&(pModel->numberOfFrames), 1, 4, m_FilePointer);
			m_CurrentChunk->bytesRead += fread(&(pModel->numberOfFrames), 1, 4, m_FilePointer);
			break;
 
		case PIVOT:			// Хранит точку опоры обьекта
 
			// Здесь мы читаем 3 float-а (X, Y, Z) для точки опоры обьекта.
			// Точка опоры - локальная ось, вокруг которой вращается обьект. По умолчанию
			// её координаты (0, 0, 0), но они могут быть изменены.
			m_CurrentObject->numOfFaces;
			if(m_CurrentObject==NULL) break;  //ROMA
			m_CurrentChunk->bytesRead +=
			fread(&(m_CurrentObject->vPivot), 1, sizeof(CVector3), m_FilePointer);
 
			// Так как в 3DS Max оси Z и Y перевернуты, меняем местами координаты
			// z и y точки опоры, затем делаем отрицательным Z.
			
			temp = m_CurrentObject->vPivot.y;
			m_CurrentObject->vPivot.y = m_CurrentObject->vPivot.z;
			m_CurrentObject->vPivot.z = -temp;
			break;
 
		case POSITION_TRACK_TAG:	// Хранит перемещение для каждого кадра
 
			// Теперь нам нужно прочитать позицию для каждого кадра анимации
			ReadKeyFramePositions(pModel, m_CurrentChunk);
			break;
 
		case ROTATION_TRACK_TAG:	// Хранит вращение для каждого КЛЮЧЕВОГО кадра
 
			// Теперь нам нужно прочитать вращение для каждого КЛЮЧЕВОГО кадра анимации.
			// Данные вращения для каждого простого кадра не хранятся как, например,
			// перемещения, так что нам нужно интерполировать их вручную.
			ReadKeyFrameRotations(pModel, m_CurrentChunk);
			break;
 
		case SCALE_TRACK_TAG:		// Хранит scale-значения для каждого кадра
 
			// Теперь читаем значения размерности для каждого кадра анимации
			ReadKeyFrameScales(pModel, m_CurrentChunk);
			break;
 
		default:
 
			// Читаем в "мусор" ненужные/неизвестные данные
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, 
					m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
 
		// Увеличиваем счетчик прочитанных байт
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
 
	// Освобождаем текущий chunk и возвращаемся к предыдущему
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}

///////////////////////////////// READ KEYFRAME POSITIONS """"""""""\\*
/////
/////	Функция читает позицию текущего обьекта для каждого кадра
/////
///////////////////////////////// READ KEYFRAME POSITIONS """"""""""\\*
 
void CLoad3DS::ReadKeyFramePositions(t3DModel *pModel, tChunk *pPreviousChunk)
{
	short frameNumber= 0, flags= 0, ignored= 0;
	long  lunknown= 0;
	int i;
 
 
	// Эта функция будет читать положение обьекта для каждого кадра. Запомните, это
	// положение изменяется для точки опоры, а от неё уже рассчитываем положение обьекта.
	// Первые 5 значений в этом уроке игнорируются.
 
	// Считываем в "мусор" игнорируемые данные
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
 
	// Читаем число кадров с изменением позиции, т.е. сколько раз обьект перемещался
	pPreviousChunk->bytesRead += fread(&(m_CurrentObject->positionFrames), 1, sizeof(short), m_FilePointer);
 
	// Читаем в "мусор" игнорируемые данные
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
 
	// Теперь нам нужно пройти через все кадры анимации и установить позицию
	// обьекта для каждого кадра. Имеем мы одну или 50 позиций, изменённых за 100 кадров,
	// нам нужно установить позиции в оставшиеся кадры.
	for(i = 0; i <= (pModel->numberOfFrames + 1); i++)
	{
		// Здесь мы добавляем новый CVector3 в наш массив позиций. Он будет сохранять
		// текущую позицию для текущего кадра анимации 'i'. Используя STL vector, мы
		// просто передаём конструктор CVector3.
		m_CurrentObject->vPosition.push_back(CVector3());
 
		// Если текущий кадр анимации входит в кадры изменения позиции, читаем следующую
		// позицию для текущего кадра.
		if(i < m_CurrentObject->positionFrames)
		{
			// Читаем текущий фреймбуфер
			pPreviousChunk->bytesRead += fread(&frameNumber, 1, sizeof(short), m_FilePointer);
 
			// Читаем в "мусор" неизвестный long
			pPreviousChunk->bytesRead += fread(&lunknown, 1, sizeof(long), m_FilePointer);
 
			// Читаем 3 float, содержащих (x,y,z) позиции. Запомните, CVector3 - это
			// 3 флоата, так что передаём как размер sizeof(float)*3
			pPreviousChunk->bytesRead +=
			fread(&(m_CurrentObject->vPosition[i]), 1, sizeof(CVector3), m_FilePointer);
 
			// Так как 3DS Max меняет местами Y<->Z, конвертируем наши координаты
			float temp = m_CurrentObject->vPosition[i].y;
			m_CurrentObject->vPosition[i].y = m_CurrentObject->vPosition[i].z;
			m_CurrentObject->vPosition[i].z = -temp;
		}
		// Иначе просто устанавливаем текущую позицию равной прошлой
		else
		{
			m_CurrentObject->vPosition[i] = m_CurrentObject->vPosition[m_CurrentObject->positionFrames - 1];
		}
	}
 
	// Теперь нам нужно вычесть точку опоры из каждой вершины. 3DS файлы сохраняют
	// вершины обьекта как ВЕРШИНА плюс точка опоры (плохо). Вы заметите, что мы
	// также вычитаем позицию текущего кадра из каждой точка. Это потому, что
	// 3DS файлы хранят позицию точки опоры для каждого кадра. Мы хотим, чтобы 
	// точка опоры начиналась с 0, иак чир будем вызывать glTranslate(), что будет
	// перемещать первую позицию.
	for(i = 0; i < m_CurrentObject->numOfVerts; i++)
	{
		// Вычитаем текущую позицию кадра и точку опоры из каждой вершины
		m_CurrentObject->pVerts[i].x -= m_CurrentObject->vPosition[0].x + m_CurrentObject->vPivot.x;
		m_CurrentObject->pVerts[i].y -= m_CurrentObject->vPosition[0].y + m_CurrentObject->vPivot.y;
		m_CurrentObject->pVerts[i].z -= m_CurrentObject->vPosition[0].z + m_CurrentObject->vPivot.z;
	}
}
 
 
///////////////////////////////// READ KEYFRAME ROTATIONS """"""""""\\*
/////
/////	Новая функция: читает вращение текущего обьекта на каждый кейфрейм
/////
///////////////////////////////// READ KEYFRAME ROTATIONS """"""""""\\*
 
void CLoad3DS::ReadKeyFrameRotations(t3DModel *pModel, tChunk *pPreviousChunk)
{
	short frameNumber = 0, flags = 0, rotkeys = 0, ignored = 0;
	long lunknown = 0;
	float rotationDegree = 0;
	vector<int> vFrameNumber;
	vector<float> vRotDegree;
	vector<CVector3> vRotation;
	int i;
 
	// Эта функция будет читать ось и угол вращения для каждого ключевого кадра.
	// Помните, вращение происходит вокруг опорной точки обьекта. Первые 5 
	// short-значений игнорируются в этом уроке.
 
	// Читаем в "мусор" игнорируемые данные
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1,  sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1,  sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignored, 1,  sizeof(short), m_FilePointer);
 
	// Читаем число кадров вращения для анимации. Помните, эти данные сохраняются
	// не для каждого кадра, а только для ключевых. Поэтому нам надо будет
	// вручную интерполировать данные между каждым кейфреймом.
	pPreviousChunk->bytesRead += fread(&(m_CurrentObject->rotationFrames), 1, sizeof(short), m_FilePointer);
 
	// Читаем игнорируемые данные
	pPreviousChunk->bytesRead += fread(&ignored, 1,  sizeof(short), m_FilePointer);
 
	// Теперь нужно пройти через ВСЕ кадры анимации и установить вращение обьекта
	// для каждого кадра. Нам нужно интерполировать вращение между ключевыми
	// кадрами, если их более 1.
	for(i = 0; i < m_CurrentObject->rotationFrames; i++)
	{
		// Добавляем новый CVector3 в массив вращений. Он будет хранить текущий угол
		// вращения для кадра 'i'. Используя STL vector, просто передадим конструктор
		// CVector3 через копирующий конструктор.
		vRotation.push_back(CVector3());
 
		// Далее читаем номер кадра
		pPreviousChunk->bytesRead += fread(&frameNumber, 1, sizeof(short), m_FilePointer);
 
		// Добавляем номер кадра в список номеров кадров для дальнейшей интерполяции
		vFrameNumber.push_back(frameNumber);
 
		// Читаем неизвестные данные в "мусор"
		pPreviousChunk->bytesRead += fread(&lunknown, 1, sizeof(long), m_FilePointer);
 
		// Читаем текущий угол вращения для данного ключевого кадра.
		pPreviousChunk->bytesRead += fread(&rotationDegree, 1, sizeof(float), m_FilePointer);
 
		// Так как 3DS Max хранит угол в радианах, а не градусах, преобразуем значение.
		// Конвертируем radian->degree (Radians * (180 / PI) = degrees)
		rotationDegree = rotationDegree * (180.0f / 3.14159f);
 
		// Добавляем угол вращения в список
		vRotDegree.push_back(rotationDegree);
 
		// Читаем ось, вокруг которой вращается обьект.
		pPreviousChunk->bytesRead += fread(&(vRotation[i]), 1, sizeof(CVector3), m_FilePointer);
 
		// Округляем углы во избежание ненужных проблем
		vRotation[i].x = RoundFloat(vRotation[i].x);
		vRotation[i].y = RoundFloat(vRotation[i].y);
		vRotation[i].z = RoundFloat(vRotation[i].z);
 
		// Преобразуем оси y<->z
		float temp = vRotation[i].y;
		vRotation[i].y = -vRotation[i].z;
		vRotation[i].z = temp;
 
		// Делаем значение X отрицательным
		vRotation[i].x *= -1;
	}
 
	// Теперь у нас есть все данные о вращении. Теперь, поскольку данные о вращении
	// хранятся для ключевых кадров, нужно их интерполировать.
 
	// Добавляем в список ось вращения
	m_CurrentObject->vRotation.push_back(vRotation[0]);
 
	// Добавляем угол вращения первого кадра в массив. Если вращения нет, угол == 0
	m_CurrentObject->vRotDegree.push_back(vRotDegree[0]);
 
	// Счетчик для текущего кадра вращения
	int currentKey = 1;
 
	// Нужно пройти через все кадры анимации ещё раз, чтобы интерполировать данные
	// вращения. Если кадр вращения только один, просто устанавливаем остальные углы
	// вращения в ноль.
 
	// Проходим через все кадры анимации
	for(i = 1; i <= (pModel->numberOfFrames + 1); i++)
	{
		// Проверяем, не равен ли текущий кадр числу кадров вращения
		if(currentKey < m_CurrentObject->rotationFrames)
		{
			// Получаем текущий и предыдущий номера кадров и углы вращения.
			int currentFrame = vFrameNumber[currentKey];
			int previousFrame = vFrameNumber[currentKey - 1];
			float degree = vRotDegree[currentKey];
 
			// Интерполируем углы вращения между текущим и предыдущим ключевыми
			// кадрами. Алгоритм прост, просто проверяем как много кадров прошло от
			// прошлого до текущего ключевого. Затем делим угол вращения на эту 
			// разницу. Например, скажем, есть кадр 0 и кадр 50. 50-0 = 50, то есть
			// нужно задать вращение для пятидесяти кадров. 
			float rotDegree = degree / (currentFrame - previousFrame);
 
			// Добавляем угол и вектор вращения для этого кадра
			m_CurrentObject->vRotation.push_back(vRotation[currentKey]);
			m_CurrentObject->vRotDegree.push_back(rotDegree);
 
			// Проверим, нужно ли переходить к след. кейфрейму
			if(vFrameNumber[currentKey] <= i)
				currentKey++;
		}
		// Иначе мы закончили с ключевыми кадрами
		else
		{
			// Устанавливаем остальное вращение в 0, т.к. нам больше не нужно вращение
			m_CurrentObject->vRotation.push_back(vRotation[currentKey - 1]);
			m_CurrentObject->vRotDegree.push_back(0.0f);
		}
	}
}
 
 
///////////////////////////////// READ KEYFRAME SCALE """"""""""\\*
/////
/////	Новая функция: читает значение scale для текущего обьекта и кадра
/////
///////////////////////////////// READ KEYFRAME SCALES """"""""""\\*
 
void CLoad3DS::ReadKeyFrameScales(t3DModel *pModel, tChunk *pPreviousChunk)
{
	short frameNumber = 0, ignore = 0, flags = 0;
	long lunknown = 0;
	int i = 0;
 
	// Как и данные перемещения, изменение размеров сохраняется для каждого
	// кадра анимации. Это значит, нам не нужно интерполировать эти данные 
	// вручную. Первые 5 параметров игнорируются. 
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
 
	// Читаем число кадров-scale. Если есть 100 кадров анимации и только 50
	// кадров изменения размера, просто устанавливаем остальные 50 кадров в
	// значение последнего кадра.
	pPreviousChunk->bytesRead += fread(&(m_CurrentObject->scaleFrames), 1, sizeof(short), m_FilePointer);
 
	// Читаем игнорируемые данные
	pPreviousChunk->bytesRead += fread(&ignore, 1, sizeof(short), m_FilePointer);
 
	// Теперь нам нужно пройти через ВСЕ кадры анимации и установить значение scale.
	for(i = 0; i <= (pModel->numberOfFrames + 1); i++)
	{
		// Добавляем довый CVector3 в список.
		m_CurrentObject->vScale.push_back(CVector3());
 
		// Если текущий кадр меньше их общего количества, читаем его данные
		if(i < m_CurrentObject->scaleFrames)
		{
			// Читаем текущий номер кадра
			pPreviousChunk->bytesRead += fread(&frameNumber, 1, sizeof(short), m_FilePointer);
 
			// Читаем неизвестный long
			pPreviousChunk->bytesRead += fread(&lunknown, 1, sizeof(long), m_FilePointer);
 
			// Читаем (X, Y, Z) scale-значения для текущего кадра. Мы будем передавать
			// их в glScalef().
			pPreviousChunk->bytesRead +=
			fread(&(m_CurrentObject->vScale[i]), 1, sizeof(CVector3), m_FilePointer);
 
			// Меняем местами Z и Y оси
			float temp = m_CurrentObject->vScale[i].y;
			m_CurrentObject->vScale[i].y = m_CurrentObject->vScale[i].z;
			m_CurrentObject->vScale[i].z = temp;
		}
		// Иначе мы закончили с данными scale
		else
		{
			// Устанавливаем текущее scale-значение в значение последнего кадра
			m_CurrentObject->vScale[i] = m_CurrentObject->vScale[m_CurrentObject->scaleFrames - 1];
		}
	}
}
 
 
///////////////////////////////// SET CURRENT OBJECT """"""""""\\*
/////
/////	Новая функция: устанавливает текущую модель, для которой будет читатся анимация
/////
///////////////////////////////// SET CURRENT OBJECT """"""""""\\*
 
void CLoad3DS::SetCurrentObject(t3DModel *pModel, char *strObjectName)
{
	// Функция принимает модель и имя обьекта внутри этой модели.
	// Затем в модели ищется обьект с соответствующим именем.
 
	// Убедимся, что передано правильное имя обьекта
	if(!strObjectName)
	{
		// Устанавливаем текущий обьект в NULL и выходим
		m_CurrentObject = NULL;
		return;
	}
 
	// Проходим через все обьекты в модели и сравниваем их имя с переданным
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		// Проверяем совпадение имён
     

		if(strcmp(pModel->pObject[i].strName, strObjectName) == 0)
		{
			// Получаем указатель на обьект с переданным именем
			// Это будет обьект, для которого дальше будут считыватся данные анимации.
			m_CurrentObject =&(pModel->pObject[i]);
			return;
		}
	}

	// Выводим сообщение об ошибке
	//MessageBox(NULL, "ERROR: No object in model with given name! (SetCurrentObject)", "Error!", MB_OK);
 
	// Устанавливаем текущий обьект в NULL, т.к. не нашли его
	m_CurrentObject = NULL;
}
 

 
///////////////////////////////// PROCESS NEXT OBJECT CHUNK """"""""""\\*
/////
/////	Функция сохраняет всю информацию об обьекте
/////
///////////////////////////////// PROCESS NEXT OBJECT CHUNK """"""""""\\*
 
void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	// Текущий chunk, с которым работаем
	m_CurrentChunk = new tChunk;
 
	// Продолжаем читать эти chunk-и, пока не дошли до конца этой секции
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Читаем следующую секцию
		ReadChunk(m_CurrentChunk);
 
		// Проверяем, что это за секция
		switch (m_CurrentChunk->ID)
		{
		case OBJECT_MESH:		// Даёт нам знать, что мы читаем новый обьект
 
			// Нашли новый обьект, прочитаем его информацию рекурсией
			ProcessNextObjectChunk(pModel, pObject, m_CurrentChunk);
			break;
 
		case OBJECT_VERTICES:		// Вершины нашего обьекта
			ReadVertices(pObject, m_CurrentChunk);
			break;
 
		case OBJECT_FACES:		// Полигоны обьекта
			ReadVertexIndices(pObject, m_CurrentChunk);
			break;
 
		case OBJECT_MATERIAL:		// Имя материала обьекта
 
			// Эта секция хранит имя материала, связанного с текущим обьектом. Это может быть
			// цвет или текстурная карта. Эта секция также содержит полигоны, к которым
			// привязана текстура (Если например на обьекте несколько текстур, или просто
			// текстура наложено только на часть обьекта). Сейчас у нас будет только одна
			// текстура на весь обьект, так что получим только ID материала.
 
			// Теперь мы читаем имя материала, привязанного к обьекту
			ReadObjectMaterial(pModel, pObject, m_CurrentChunk);
			break;
 
		case OBJECT_UV:		// Хранит текстурные координаты обьекта
 
			// Эта секция содержит все UV-координаты обьекта. Прочитаем их.
			ReadUVCoordinates(pObject, m_CurrentChunk);
			break;
 
		default:
 
			// Read past the ignored or unknown chunks
			// Читаем игнорируемые/неизвестные данные в "мусорный" массив
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, m_CurrentChunk->length 
						- m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}
 
		// Прибавляем прочитанные данные к счетчику
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}

	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}
 
 
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK """"""""""\\*
/////
/////	Эта функция хранит всю информацию о материале (текстуре)
/////
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK """"""""""\\*
 
void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	// Текущий chunk для работы
	m_CurrentChunk = new tChunk;
 
	// Продолжаем читать эти chunk-и, пока не дошли до конца подсекции
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// Читаем следующую секцию
		ReadChunk(m_CurrentChunk);
 
		// Проверяем, что именно мы прочитали
		switch (m_CurrentChunk->ID)
		{
		case MATNAME:		// Эта секция хранит имя материала
 
			// читаем имя материала
			m_CurrentChunk->bytesRead += 
						fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName,
						1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
 
		case MATDIFFUSE:		// Хранит RGB-цвет обьекта
			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), m_CurrentChunk);
			break;
 
		case MATMAP:			// Это хидер информации о текстуре
 
			// Читаем информацию информацию о материале
			ProcessNextMaterialChunk(pModel, m_CurrentChunk);
			break;
 
		case MATMAPFILE:		// Хранит имя файла материала
 
			// Читаем имя файла материала
			m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile,
						1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
 
		default:
 
			// Читаем остальные данные в "мусор"
			m_CurrentChunk->bytesRead += fread(gBuffer, 1, 
						m_CurrentChunk->length - m_CurrentChunk->bytesRead,
						m_FilePointer);
			break;
		}
 
		// Прибавляем прочитанные данные к счетчику
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}
 
///////////////////////////////// READ CHUNK """"""""""\\*
/////
/////	Функция читает ID chunk-а и его длинну в байтах
/////
///////////////////////////////// READ CHUNK """"""""""\\*
 
void CLoad3DS::ReadChunk(tChunk *pChunk)
{
	// Функция читает ID секции (2 байта).
	// ID chunk-а - это, например, OBJECT/MATERIAL. Это говорит нам,
	// какие данные могут быть прочитаны в этой секции.
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);
 
	// Затем читаем длинну секции (4 байта). Теперь мы знаем,
	// сколько данных нам нужно будет прочитать.
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}
 
///////////////////////////////// GET STRING """"""""""\\*
/////
/////	Читает строку в массив char-ов
/////
///////////////////////////////// GET STRING """"""""""\\*
 
int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;
 
	// Читаем 1 байт данных, первую букву строки
	fread(pBuffer, 1, 1, m_FilePointer);
 
	// Цикл, пока не получаем NULL
	while (*(pBuffer + index++) != 0) {
 
		// Читаем символы всё время, пока не получим NULL
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}
 
	// Вернём длинну строки, т.е. сколько байтов мы прочитали (включая NULL)
	return strlen(pBuffer) + 1;
}
 
 
///////////////////////////////// READ COLOR """"""""""\\*
/////
/////	Читает данные RGB-цвета
/////
///////////////////////////////// READ COLOR """"""""""\\*
 
void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{ 
	// Читаем информацию о цвете
	ReadChunk(m_TempChunk);
 
	// Читаем RGB-цвет (3 байта - от 0 до 255)
	m_TempChunk->bytesRead += fread(pMaterial->color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);
 
	// Увеличиваем счетчик
	pChunk->bytesRead += m_TempChunk->bytesRead;
}
 
 
///////////////////////////////// READ VERTEX INDECES """"""""""\\*
/////
/////	Функция читает индексы для массива вершин
/////
///////////////////////////////// READ VERTEX INDECES """"""""""\\*
 
void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;		// Используется для чтения индекса текущего полигона
 
	// Чтобы прочитать индексы вершин для обьекта, нужно сначала прочитать их
	// число, затем уже их самих. Запомните, нам нужно прочитать только 3 из
	// 4 значений для каждого полигона. Это четвертое значение - флаг видимости
	// для 3DS Max, которое ничего для нас не значит.
 
	// Читаем число полигонов этого обьекта
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);
 
	// Выделяем достаточно памяти для полигонов и инициализируем структуру
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);
 
	// Проходим через все полигоны этого обьекта
	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		// Далее читаем A-B-C индексы для полигона, но игнорируем 4-е значение.
		for(int j = 0; j < 4; j++)
		{
			// Читаем первый индекс вершины для текущего полигона
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);
 
			if(j < 3)
			{
				// Сохраняем индекс в структуру полигонов
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}
 
 
///////////////////////////////// READ UV COORDINATES """"""""""\\*
/////
/////	Функция читает UV-координаты обьекта
/////
///////////////////////////////// READ UV COORDINATES """"""""""\\*
 
void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// Чтобы прочитать индексы UV-координат для обьекта, сначала нужно
	// прочитать их полное количество, потом уже их самих.
 
	// Читаем число UV-координат
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);
 
	// Выделяем память для хранения UV-координат
	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];
 
	// Читаем текстурные координаты (массив из 2х float)
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1,
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}
 
 
///////////////////////////////// READ VERTICES """"""""""\\*
/////
/////	Функция читает вершины обьекта
/////
///////////////////////////////// READ VERTICES """"""""""\\*
 
void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// Как и в большинстве chunk-ов, прежде чем читать сами вершины,
	// нужно найти их количество.
 
	// Читаем число вершин
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);
 
	// Выделяем память для вершин и инициализируем структуру
	pObject->pVerts = new CVector3 [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3) * pObject->numOfVerts);
 
	// Читаем в массив вершин (массив из 3 float)
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, 
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
 
	// Теперь все вершины прочитаны. Так как в моделях 3DS Max всегда перевёрнуты
	// оси, нужно поменять Y-значения и Z-значения наших вершин.
 
	// Проходим через все вершины и меняем y<->z
	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		// Сохраняем старое знач-е Y
		float fTempY = pObject->pVerts[i].y;
 
		// Устанавливаем значение Y в Z
		pObject->pVerts[i].y = pObject->pVerts[i].z;
 
		// Устанавливаем значение Z в Y
		// И делаем его отрицательным, т.к. в 3ds max Z-ось перевернута
		pObject->pVerts[i].z = -fTempY;
	}
}
 
 
///////////////////////////////// READ OBJECT MATERIAL """"""""""\\*
/////
/////	Функция читает имя материала, наложенного на обьект, и устанавливает materialID
/////
///////////////////////////////// READ OBJECT MATERIAL """"""""""\\*
 
void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = {0};		// Хранит имя материала
 
	// *Что такое материал?* - Материал - это цвет + текстурная карта обьекта.
	// Также он можетхранить другую информацию типа яркости, "блестящести" и т.д.
	// Сейчас нам нужно только цвет или имя текстурной карты.
 
	// Читаем имя материала, привязанного к текущему обьекту.
	// strMaterial теперь должен содержать строку с именем материала, типа "Material #2" и т.д...
	pPreviousChunk->bytesRead += GetString(strMaterial);
 
	// Теперь, имея имя материала, нужно пройти через все материалы и проверять их
	// имена на совпадение с нашим. Когда найдем материал с только что прочитанным 
	// именем, привязываем materialID обьекта к индексу этого материала. 
 
	// Проходим через все материалы
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		// Если только что прочитанный материал совпадает с именем данного
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			// Проверяем, есть ли текстурная карта. Если strFile содержит
			// строку с длинной >=1, текстура есть.
			if(strlen(pModel->pMaterials[i].strFile) > 0)
			{
				// Устанавливаем ID материала в текущий индекс 'i' и заканчиваем проверку
				pObject->materialID = i;
 
				// Устанавливаем флаг текстурирования в true
				pObject->bHasTexture = true;
			}
			break;
		}
		else
		{
			// Проверяем флаг, чтобы увидеть, есть ли уже текстура на этом обьекте
			if(pObject->bHasTexture != true)
			{
				// Устанавливаем ID материала в -1, чтобы указать, что материала для обьекта нет
				pObject->materialID = -1;
			}
		}
	}
 
	// Остальное читаем в "мусор"
	pPreviousChunk->bytesRead += fread(gBuffer, 1, 
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}
 
// *Note*
//
// Ниже идут несколько математических функций, вычисляющих нормали вершин. Они нам
// нужны, чтобы эффект освещения рассчитывался верно. В прошлых уроках мы уже писали 
// эти функции, так что при желании можно просто подключить файлы 3dmath.h/.cpp
 
//////////////////////////////	Math Functions  ////////////////////////////////*
 
// Рассчитывает величину нормали (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))
 
// Рассчитывает векторы между 2 точками и возвращает результат
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
	CVector3 vVector;			// Хранит результирующий вектор
 
	vVector.x = vPoint1.x - vPoint2.x;
	vVector.y = vPoint1.y - vPoint2.y;
	vVector.z = vPoint1.z - vPoint2.z;
 
	return vVector;				// Вернём результирующий вектор
}
 
// This adds 2 vectors together and returns the result
// Складывает 2 вектора и возвращает результат
CVector3 AddVector(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vResult;				// Хранит результирующий вектор
 
	vResult.x = vVector2.x + vVector1.x;
	vResult.y = vVector2.y + vVector1.y;
	vResult.z = vVector2.z + vVector1.z;
 
	return vResult;					// Вернём результат
}
 
// Делит вектор на переданный номер и возвращает результат
CVector3 DivideVectorByScaler(CVector3 vVector1, float Scaler)
{
	CVector3 vResult;
 
	vResult.x = vVector1.x / Scaler;
	vResult.y = vVector1.y / Scaler;
	vResult.z = vVector1.z / Scaler;
 
	return vResult;
}
 
// Возвращает скалярное произведение (dot product) двух векторов
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vCross;
 
	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
 
	return vCross;
}
 
// Возвращает нормаль вектора
CVector3 Normalize(CVector3 vNormal)
{
	double Magnitude;
 
	Magnitude = Mag(vNormal);
 
	vNormal.x /= (float)Magnitude;
	vNormal.y /= (float)Magnitude;
	vNormal.z /= (float)Magnitude;
 
	return vNormal;
}
 
///////////////////////////////// COMPUTER NORMALS """"""""""\\*
/////
/////	Функция рассчитывает нормали для обьекта и его вершин
/////
///////////////////////////////// COMPUTER NORMALS """"""""""\\*
 
void CLoad3DS::ComputeNormals(t3DModel *pModel)
{
	CVector3 vVector1, vVector2, vNormal, vPoly[3];
 
	// Если обьектов нет, пропускаем этот шаг
	if(pModel->numOfObjects <= 0)
		return;
 
	// Что такое нормали вершин? Чем они отличаются от остальных нормалей? Если вы
	// нашли нормаль треугольника, это "нормаль полигона". Если вы передали OpenGL
	// нормаль полигона для освещения, ваш обьект будет выглядеть плоским и резким.
	// Если же вы найдете нормали для каждой вершины, освещенный обьект будет
	// выглядеть сглаженным, т.е. более реалистичным.
 
	// Проходим через все обьекты для вычисления их вершин
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		// Получим текущий обьект
		t3DObject *pObject = &(pModel->pObject[index]);
 
		// Выделяем память под нужные переменные
		CVector3 *pNormals		= new CVector3 [pObject->numOfFaces];
		CVector3 *pTempNormals	= new CVector3 [pObject->numOfFaces];
		pObject->pNormals		= new CVector3 [pObject->numOfVerts];
 
		// Проходим через все полигоны обьекта
		for(int i=0; i < pObject->numOfFaces; i++)
		{
			// Сохраняем 3 точки этого полигона, чтобы избежать большого кода
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];
 
			// Теперь вычислим нормали полигонов
 
			vVector1 = Vector(vPoly[0], vPoly[2]);	// вектор полигона (из 2х его сторон)
			vVector2 = Vector(vPoly[2], vPoly[1]);	// Второй вектор полигона
 
			vNormal  = Cross(vVector1, vVector2);	// получаем cross product векторов
			pTempNormals[i] = vNormal;		// временно сохраняем не-нормализированную нормаль
								// для вершин
			vNormal  = Normalize(vNormal);		// нормализируем cross product для нормалей полигона
 
			pNormals[i] = vNormal;			// Сохраняем нормаль в массив
		}
 
		//////////////// Теперь получаем вершинные нормали /////////////////
 
		CVector3 vSum = {0.0, 0.0, 0.0};
		CVector3 vZero = vSum;
		int shared=0;
 
		for (int i = 0; i < pObject->numOfVerts; i++)	// Проходим через все вершины
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	// Проходим через все треугольники
			{				// Проверяем, используется ли вершина другим полигоном
				if (pObject->pFaces[j].vertIndex[0] == i ||
					pObject->pFaces[j].vertIndex[1] == i ||
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);	// Прибавляем не-
								// нормализированную нормаль другого полигона
					shared++;		// Увеличиваем число полигонов с общими вершиными
				}
			}
 
			// Получаем нормаль делением на сумму общих полигонов. Делаем её отрицат.
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));
 
			// Нормализуем нормаль для вершины
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);
 
			vSum = vZero;			// Сбрасываем сумму
			shared = 0;			// И общие полигоны
		}
 
		// Освобождаем память временных переменных
		delete [] pTempNormals;
		delete [] pNormals;
	}
}
