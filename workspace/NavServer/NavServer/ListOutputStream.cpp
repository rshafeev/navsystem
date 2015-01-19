#include "stdafx.h"
#include "ListOutputStream.h"

CListOutputStream::CListOutputStream(CListBox &list)
:listBox(list)
{

}

CListOutputStream::~CListOutputStream()
{
}

void CListOutputStream::println(CString line)
{
	this->listBox.AddString(line);
}
