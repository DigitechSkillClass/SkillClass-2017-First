#include "DXUT.h"
#include "cObject.h"


cObject::cObject()
	: m_nRefCount(1)
{
	g_setObject.insert(this);
}

cObject::~cObject()
{
	g_setObject.erase(this);
}

void cObject::AddRef()
{
	++m_nRefCount;
	assert(m_nRefCount < INT_MAX);
}

void cObject::Release()
{
	--m_nRefCount;
	if (m_nRefCount <= 0)
		delete this;
}
