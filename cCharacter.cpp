#include "DXUT.h"
#include "cCharacter.h"


cCharacter::cCharacter()
	: m_vPrevPosition(0, 0, 0)
	, m_vDirection(0, 0, 1)
{
}


cCharacter::~cCharacter()
{
}

void cCharacter::Update()
{
	cGameObject::Update();
}

void cCharacter::Render()
{
	D3DXVECTOR3 vDirection = m_vPosition - m_vPrevPosition;
	if (D3DXVec3LengthSq(&vDirection) > 0)
	{
		m_vDirection = vDirection;
		D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	}

	D3DXMATRIXA16 matT, matR, matWorld;
	D3DXMatrixLookAtLH(&matR,
		&D3DXVECTOR3(0, 0, 0),
		&m_vDirection,
		&D3DXVECTOR3(0, 1, 0));
	D3DXMatrixTranspose(&matR, &matR);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	matWorld = matR * matT;
	
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pMesh->DrawSubset(0);

	m_vPrevPosition = m_vPosition;
}