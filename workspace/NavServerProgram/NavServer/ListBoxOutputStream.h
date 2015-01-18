#pragma once
#include "AbstractOutputStream.h"
#include "stdafx.h"

class CListBoxOutputStream : public CAbstractOutputStream
{
	CListBox *listBox;
public:
	CListBoxOutputStream(CListBox *_listBox);
	virtual ~CListBoxOutputStream();

	virtual void println(CString line);
};

