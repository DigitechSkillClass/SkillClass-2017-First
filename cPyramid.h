#pragma once
class cPyramid
{
private:
	std::vector<ST_PC_VERTEX>	m_vecVertex;

public:
	cPyramid();
	virtual ~cPyramid();

	void Setup(D3DCOLOR c, D3DXMATRIX* pmat);
	void Render(D3DXMATRIX* pMat = nullptr);
};

