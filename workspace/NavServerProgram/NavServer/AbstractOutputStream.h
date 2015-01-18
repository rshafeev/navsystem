#pragma once
class CAbstractOutputStream
{
protected:
	CAbstractOutputStream();
public:
	virtual ~CAbstractOutputStream();

	void println(void);
	virtual void println(const char *line);
	virtual void println(CString line) = 0;

};

