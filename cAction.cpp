#include "DXUT.h"
#include "cAction.h"


cAction::cAction()
	: m_pTarget(NULL)
	, m_pDelegate(NULL)
{
}

cAction::~cAction()
{
}

void cAction::SetTarget(cGameObject* pTarget)
{
	m_pTarget = pTarget;
	pTarget->SetAction(this);
}