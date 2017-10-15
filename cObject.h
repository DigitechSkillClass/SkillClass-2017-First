#pragma once
class cObject
{
private:
	int m_nRefCount;

public:
	void AddRef();
	void Release();

public:
	cObject();
	virtual ~cObject();
};

