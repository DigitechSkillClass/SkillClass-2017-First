#include "DXUT.h"
#include "cActionMove.h"


cActionMove::cActionMove()
	: m_vFrom(0, 0, 0)
	, m_vTo(0, 0, 0)
	, m_fActionTime(0.0f)
	, m_fPassedActionTime(0.0f)
{
}

cActionMove::~cActionMove()
{
}

void cActionMove::Start()
{
	m_fPassedActionTime = 0.0f;
	if (m_pTarget)
	{
		m_pTarget->SetPosition(m_vFrom);
	}
}

void cActionMove::Update()
{
	if (m_pTarget == NULL)
		return;

	m_fPassedActionTime += DXUTGetElapsedTime();
	if (m_fPassedActionTime >= m_fActionTime)
	{
		// ³¡
		m_pTarget->SetPosition(m_vTo);
		if (m_pDelegate)
		{
			m_pDelegate->OnActionFinish(this);
		}
	}
	else
	{
		if (m_fActionTime > 0)
		{
			float t = m_fPassedActionTime / m_fActionTime;
			D3DXVECTOR3 p(0, 0, 0);
			D3DXVec3Lerp(&p, &m_vFrom, &m_vTo, t);
			m_pTarget->SetPosition(p);
		}
	}
}