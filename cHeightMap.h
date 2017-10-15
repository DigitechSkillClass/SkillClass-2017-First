#pragma once

class cCamera;

class cHeightMap
{
private:
	LPD3DXMESH					m_pMesh;
	D3DMATERIAL9				m_stMtl;
	std::string					m_sTexture;
	std::vector<D3DXVECTOR3>	m_vecVertex;
	int							m_nCol;
	LPD3DXEFFECT				m_pEffect;
	LPD3DXEFFECT				m_pBlurEffect;
	LPDIRECT3DTEXTURE9			m_pRenderTarget;
	LPDIRECT3DSURFACE9			m_pDepthStencil;
	std::vector<ST_PT_VERTEX>	m_vecScreen;

public:
	cHeightMap();
	~cHeightMap();

	void Load(char* szRawFile, char* szTexFile, int nBytesPerPixel = 1);
	void Render(cCamera* pCamera);
	bool GetHeight(IN float x, OUT float& y, IN float z);
};

