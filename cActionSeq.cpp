#include "DXUT.h"
#include "cActionSeq.h"


cActionSeq::cActionSeq()
	: m_nCurrAction(0)
{
}


cActionSeq::~cActionSeq()
{
	for each(auto p in m_vecAction)
	{
		SAFE_RELEASE(p);
	}
}

void cActionSeq::AddAction(cAction* pAction)
{
	SAFE_ADDREF(pAction);
	if (pAction)
	{
		m_vecAction.push_back(pAction);
	}
}

void cActionSeq::Start()
{
	m_nCurrAction = 0;
	if (!m_vecAction.empty())
	{
		m_vecAction.front()->Start();
	}
}

void cActionSeq::Update()
{
	if (m_nCurrAction < m_vecAction.size())
	{
		m_vecAction[m_nCurrAction]->Update();
	}
}

void cActionSeq::OnActionFinish(cAction* pSender)
{
	m_nCurrAction++;
	if (m_nCurrAction < m_vecAction.size())
	{
		m_vecAction[m_nCurrAction]->Start();
	}
	else
	{
		if (m_pDelegate)
		{
			m_pDelegate->OnActionFinish(this);
		}
	}
}