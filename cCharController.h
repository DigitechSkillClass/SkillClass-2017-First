#pragma once

class cHeightMap;

class cCharController
{
private:
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vDirection;
	D3DXMATRIX	m_matWorld;
	float		m_fAngle;

public:
	cCharController();
	virtual ~cCharController();

	void Update(cHeightMap* pMap);
	D3DXMATRIX* GetWorldTM()
	{
		return &m_matWorld;
	}
	D3DXVECTOR3* GetPosition()
	{
		return &m_vPosition;
	}
};

