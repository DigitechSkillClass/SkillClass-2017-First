#pragma once
class cCube;
class cGrid;
class cCharController;
class cCamera;
class cCharacter;
class cFrame;
class cHeightMap;
class cMtlTex;
class cGroup;
class cFrustum;
class cPyramid;

class cMainGame
{
private:
	cCube*				m_pCube;
	cGrid*				m_pGrid;
	cCharController*	m_pCharController;
	cCamera*			m_pCamera;
	D3DMATERIAL9		m_stMtl;
	cFrame*				m_pRootFrame;
	cHeightMap*			m_pHeightMap;
	LPD3DXMESH			m_pMesh;
	cFrustum*			m_pFrustum;
	cPyramid*			m_pPyramid;
	D3DXMATRIX			m_matWorld;

	std::vector<ST_PC_VERTEX>	m_vecRect1;
	std::vector<ST_PC_VERTEX>	m_vecRect2;
	std::vector<cCharacter*>	m_vecCharacter;
	std::vector<cMtlTex*>		m_vecMtlTex;
	std::vector<cGroup*>		m_vecGroup;
	std::vector<ST_SPHERE>		m_vecSphere;
	std::vector<ST_PC_VERTEX>	m_vecPlane;
public:
	cMainGame();
	~cMainGame();

	void Setup();
	void Update();
	void Render();
	void MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetLight();
};

