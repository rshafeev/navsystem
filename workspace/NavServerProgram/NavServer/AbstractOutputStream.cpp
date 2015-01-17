#include "stdafx.h"
#include "AbstractOutputStream.h"


CAbstractOutputStream::CAbstractOutputStream()
{
}


void CAbstractOutputStream::println()
{
	this->println("");
}

CAbstractOutputStream::~CAbstractOutputStream()
{
}
