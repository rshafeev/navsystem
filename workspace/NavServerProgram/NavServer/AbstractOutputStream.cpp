#include "stdafx.h"
#include "AbstractOutputStream.h"
#include "ListBoxOutputStream.h"

CAbstractOutputStream::CAbstractOutputStream()
{
}


void CAbstractOutputStream::println()
{
	this->println(CString(""));
}

void CAbstractOutputStream::println(const char* line)
{
	this->println(CString(line));
}

CAbstractOutputStream::~CAbstractOutputStream()
{
}
