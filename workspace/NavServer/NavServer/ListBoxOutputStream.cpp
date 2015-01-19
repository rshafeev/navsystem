#include "stdafx.h"
#include "ListBoxOutputStream.h"


CListBoxOutputStream::CListBoxOutputStream(CListBox *_listBox)
{
	this->listBox = _listBox;
}


CListBoxOutputStream::~CListBoxOutputStream()
{
}

void CListBoxOutputStream::println(CString line)
{
	this->listBox->AddString(line);
}