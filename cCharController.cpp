#include "DXUT.h"
#include "cCharController.h"
#include "cHeightMap.h"


cCharController::cCharController()
	: m_vPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
	, m_fAngle(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cCharController::~cCharController()
{
}

void cCharController::Update(cHeightMap* pMap)
{
	D3DXMATRIX matR, matT;
	if (GetKeyState('A') & 0x8000)
	{
		m_fAngle -= 0.1f;
	}
	if (GetKeyState('D') & 0x8000)
	{
		m_fAngle += 0.1f;
	}
	
	D3DXMatrixRotationY(&matR, m_fAngle);

	m_vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);

	if (GetKeyState('W') & 0x8000)
	{
		m_vPosition += (m_vDirection * 0.1f);
	}
	if (GetKeyState('S') & 0x8000)
	{
		m_vPosition -= (m_vDirection * 0.1f);
	}

	float y = 0;
	if (pMap && pMap->GetHeight(m_vPosition.x, y, m_vPosition.z))
	{
		m_vPosition.y = y;
	}

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matR * matT;
}
