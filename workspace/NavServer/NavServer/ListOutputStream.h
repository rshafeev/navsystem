#pragma once
#include "AbstractOutputStream.h"

class CListOutputStream : public CAbstractOutputStream
{
private:
	CListBox &listBox;
public:
	CListOutputStream(CListBox &list);
	virtual ~CListOutputStream();

	virtual void println(CString line);
};

