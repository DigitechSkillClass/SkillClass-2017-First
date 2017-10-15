#include "DXUT.h"
#include "cGameObject.h"


cGameObject::cGameObject()
	: m_vPosition(0, 0, 0)
	, m_pMesh(NULL)
	, m_pAction(NULL)
{
}

cGameObject::~cGameObject()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pAction);
}

void cGameObject::Update()
{
	if (m_pAction)
	{
		m_pAction->Update();
	}
}

void cGameObject::Render()
{

}