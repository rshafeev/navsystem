#pragma once
#include "AbstractClient.h"
class ClientTraker : public CAbstractClient
{
private:
	CString IMEI;
	vector<int> users; //список юзеров, которым надо разослать пол. данные от IMEI
public:
	ClientTraker();
	~ClientTraker();
};

