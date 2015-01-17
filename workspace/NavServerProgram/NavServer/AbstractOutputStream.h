#pragma once
class CAbstractOutputStream
{
public:
	CAbstractOutputStream();
	virtual ~CAbstractOutputStream();

	virtual void println(void);
	virtual void println(CString line) = 0;

};

