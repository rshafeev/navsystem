//adoclasses.h
#ifndef __ADO__CLASSES__H__
#define __ADO__CLASSES__H__

//----CADOException--------------------------------------------------
class CADOException
{
public:
    virtual ~CADOException() {
    }
    virtual const char* What() const {
        return "";
    }
    virtual int GetCode() const {
        return 0;
    }
};
//-------------------------------------------------------------------


//----CADOConnectionException----------------------------------------
class CADOConnectionException : public CADOException
{
public:
    enum _errConnection {
        ceUnknown, ceCantCreateConnection, ceConnectionNotExists, ceConnectionNotOpen,
        ceErrCloseConnection, ceConnectionAlreadyOpen, ceErrOpenConnection, ceErrExecuteCmd
    };
    explicit CADOConnectionException(_errConnection ec)
        : m_errCode(ec), m_strMsg(""), m_bHasDescription(false) {
    }
    explicit CADOConnectionException(const char *pStrMsg)
        : m_errCode(ceUnknown), m_strMsg(pStrMsg), m_bHasDescription(false) {
    }
    CADOConnectionException(_errConnection ec, const char *pStrMsg, bool bHasDescription = false)
        : m_errCode(ec), m_strMsg(pStrMsg), m_bHasDescription(bHasDescription) {
    }
    virtual int GetCode() const {
        return static_cast<int>(m_errCode);
    }
    const char* GetDescription() const {
        return m_strMsg.c_str();
    }
    bool HasDescription() const {
        return m_bHasDescription;
    }
	
    virtual const char* What() const {
        if (!m_bHasDescription && m_strMsg.length())
            return m_strMsg.c_str();
        switch (m_errCode)
        {
        case ceUnknown                  : return "Ошибка не имеет описания";
        case ceCantCreateConnection     : return "Ошибка при создании объекта \"Соединение\"";
        case ceConnectionNotExists      : return "Объект \"Соединение\" не создан";
        case ceConnectionNotOpen        : return "Соединение не открыто";
        case ceErrCloseConnection       : return "Ошибка при закрытии соединения";
        case ceConnectionAlreadyOpen    : return "Соединение уже открыто";
        case ceErrOpenConnection        : return "Ошибка при открытии соединения";
        case ceErrExecuteCmd            : return "Ошибка при выполнении запроса";
        default : break;
        }
        return "Неизвестная ошибка";
    }
private:
    _errConnection m_errCode;
    string m_strMsg;
    bool m_bHasDescription;
};
//-------------------------------------------------------------------


//----CADORecordsetException-----------------------------------------
class CADORecordsetException : public CADOException
{
public:
    enum _errRecordset {
        reUnknown, reRecordsetNotCreated, reRecordsetNotClosed, reCannotCreateCmd,
        reCannotCreateRecordset, reRecordsetAlreadyExists, reErrOpenRecordset,
        reRecordsetNotOpen, reErrMoveFirst, reErrMoveLast, reErrMoveNext, reErrMovePrev,
        reErrGetEoF, reErrGetBoF, reErrGetField, reErrCloseRecordset, reErrAddNewRecord,
        reErrUpdateRecordset, reErrSetField, reErrCancel, reErrGetFieldsCount,
        reErrSafeBookmark, reErrSafeGoToBookmark
    };
    explicit CADORecordsetException(_errRecordset re)
        : m_errCode(re), m_strMsg(""), m_bHasDescription(false) {
    }
    explicit CADORecordsetException(const char *pStrMsg)
        : m_errCode(reUnknown), m_strMsg(pStrMsg), m_bHasDescription(false) {
    }
    CADORecordsetException(_errRecordset re, const char *pStrMsg, bool bHasDescription = false)
        : m_errCode(re), m_strMsg(pStrMsg), m_bHasDescription(bHasDescription) {
    }
    virtual int GetCode() const {
        return static_cast<int>(m_errCode);
    }
    const char* GetDescription() const {
        return m_strMsg.c_str();
    }
    bool HasDescription() const {
        return m_bHasDescription;
    }
    virtual const char* What() const {
        if (!m_bHasDescription && m_strMsg.length())
            return m_strMsg.c_str();
        switch (m_errCode)
        {
        case reUnknown                  : return "Ошибка не имеет описания";
        case reRecordsetNotCreated      : return "Объект \"Набор записей\" не создан";
        case reRecordsetNotClosed       : return "\"Набор записей\" не закрыт";
        case reCannotCreateCmd          : return "Ошибка при создании объекта \"Комманда\"";
        case reCannotCreateRecordset    : return "Ошибка при создании объекта \"Набор записей\"";
        case reRecordsetAlreadyExists   : return "Набор записей уже существует";
        case reErrOpenRecordset         : return "Ошибка при открытии набора данных";
        case reRecordsetNotOpen         : return "Набор записей не открыт";
        case reErrMoveFirst             : return "Ошибка при переходе на первую запись";
        case reErrMoveLast              : return "Ошибка при переходе на последнюю запись";
        case reErrMoveNext              : return "Ошибка при переходе на следующую запись";
        case reErrMovePrev              : return "Ошибка при переходе на предыдущую запись";
        case reErrGetEoF                : return "Ошибка при определении начала набора записей";
        case reErrGetBoF                : return "Ошибка при определении конца набора записей";
        case reErrGetField              : return "Ошибка при получении значения поля";
        case reErrCloseRecordset        : return "Ошибка при закрытии набора данных";
        case reErrAddNewRecord          : return "Ошибка при добавлении новой записи";
        case reErrUpdateRecordset       : return "Ошибка при обновлении набора записей";
        case reErrSetField              : return "Ошибка при установке значения поля";
        case reErrCancel                : return "Ошибка при отмене";
        case reErrGetFieldsCount        : return "Ошибка при получении количества полей набора записей";
        case reErrSafeBookmark          : return "Ошибка при сохранении закладки";
        case reErrSafeGoToBookmark      : return "Ошибка при переходе на закладку";
        default : break;
        }
        return "Неизвестная ошибка";
    }
private:
    _errRecordset m_errCode;
    string m_strMsg;
    bool m_bHasDescription;
};
//-------------------------------------------------------------------


//----CADOConnection-------------------------------------------------
class CADOConnection;
//-------------------------------------------------------------------


//----CADORecordset--------------------------------------------------
class CADORecordset
{
public:
    CADORecordset();
    virtual ~CADORecordset();
    bool IsCreated() const;//создан ли объект?
    bool IsOpen() const;   //открыти ли БД?
    void SetConnection(CADOConnection *pConnection); //привязать к объекту pConnection
    void SetSQL(const char *pStrSQL);//выполняет SQL- запрос
	void AddParam(DataTypeEnum prmType, const _variant_t &vtPrm);
    void SetOpenMode(bool bDynamicOpen);
    void Open();//открыть БД
    void Close();//закрыть БД
    void First();//перевести курсор на первую запись
    void Last(); //перевести курсор на последнюю запись
    void Next(); //перевести курсор на следующую запись
    void Prev();
    bool IsEoF();//добрались до концы БД?
    bool IsBoF();//добрались до начала БД?
    bool IsEmpty();
	 //получить запись:
    _variant_t GetField(short nNumField);      
    _variant_t GetField(const char *pFieldName);
	//добавить запись/записи:
    void SetField(short nNumField, const _variant_t &vtValue);
    void SetField(const char *pFieldName, const _variant_t &vtValue);
    void SetArrayField(const char *pFieldName, const void *pBuf, int nBytes);
    void SetArrayField(short nNumField, const void *pBuf, int nBytes);
	void AddNew();//добавление
    void Update();//сохранение изменений в базе данных
    void Cancel();//отмена несохраненных изменений

    int GetFieldsCount() const; //получить количество записей
    void SafeBookmark();
    void GoToBookmark();
    long GetCountRecs();
    bool IsNewRecord() const;
    bool IsEditing() const;
private:
    CADOConnection *m_pConnection;//указатель на объект подкл. БД
	//объявления объектов ADO из директивы msado15.dll
    _CommandPtr m_pCommand;
    _RecordsetPtr m_pRecordset;
    _bstr_t m_strSQL;
    bool m_bDynamicOpen;//тип курсора
    _ConnectionPtr GetAdoConnection();
    struct SQL_PARAMS {
        DataTypeEnum prmType;
        _variant_t vtParam;
    };
    list<SQL_PARAMS> m_lstParam;
    _variant_t m_vtBookmark;
    bool m_bNewRecond;
    bool m_bEditing;
	void CheckOpen();
};
//-------------------------------------------------------------------


//----CADOConnection-------------------------------------------------
class CADOConnection
{
public:
    CADOConnection();
	//конструкторы с параметрами подключения к БД
    CADOConnection(const char *pStrConnection, const char *pStrUserID = "", const char *pStrPassword = "", bool bOpen = false);
    CADOConnection(int nJetVer, int nJetVerEx, const char *pStrFileName,
                    const char *pStrUserID = "", const char *pStrPassword = "", bool bOpen = false);
    //было ли создано подключение?
    bool IsCreated() const;
	//была ли открыта БД?
    bool IsOpen() const;
	//задать след. параметры подключения: строка подкл., имя пользователя, пароль
    void SetConnectionParam(const char *pStrConnection, const char *pStrUserID = "", const char *pStrPassword = "");
    //задать след. параметры подключения: тип драйвера,версия драйвера, имя БД, имя пользователя,пароль
	void SetMSAccessConnectionParam(int nJetVer, int nJetVerEx, const char *pStrFileName,
                                    const char *pStrUserID = "", const char *pStrPassword = "");

    void Open();         	         //открыть БД
    void Close();                    //закрыть БД
    void Destroy();                  //уничтожить объект
    void Execute(const char *pStrCmd);
	virtual ~CADOConnection();
private:
	
    CADOConnection(const CADOConnection &) {}
    void operator = (const CADOConnection &) {}
    //объявления объектов ADO из директивы msado15.dll
	_ConnectionPtr m_pConnection;
    _bstr_t m_strConnection;
    _bstr_t m_strUserID;
    _bstr_t m_strPassword;
    friend class CADORecordset;
};
//-------------------------------------------------------------------


#endif

