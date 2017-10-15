#pragma once
class cFrustum
{
	std::vector<D3DXVECTOR3>	m_vecProjVertex;
	std::vector<D3DXPLANE>		m_vecPlane;

public:
	cFrustum();
	~cFrustum();
	void Update();
	bool IsIn(ST_SPHERE* pSphere);
};

