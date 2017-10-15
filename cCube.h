#pragma once
class cCube
{
private:
	std::vector<ST_PN_VERTEX>	m_vecVertex;
	D3DMATERIAL9				m_stMtl;

public:
	cCube();
	virtual ~cCube();

	void Setup();
	void Render(D3DXMATRIX* pWorldTM);
};

