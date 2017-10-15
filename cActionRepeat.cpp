#include "DXUT.h"
#include "cActionRepeat.h"


cActionRepeat::cActionRepeat()
	: m_pAction(NULL)
{
}


cActionRepeat::~cActionRepeat()
{
	SAFE_RELEASE(m_pAction);
}


void cActionRepeat::Start()
{
	if (m_pAction)
	{
		m_pAction->Start();
	}
}

void cActionRepeat::Update()
{
	if (m_pAction)
	{
		m_pAction->Update();
	}
}

void cActionRepeat::OnActionFinish(cAction* pSender)
{
	if (m_pAction)
	{
		m_pAction->Start();
	}
}