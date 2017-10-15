#include "DXUT.h"
#include "cCube.h"

cCube::cCube()
{
}

cCube::~cCube()
{
}

void cCube::Setup()
{
	std::vector<D3DXVECTOR3> vecPosition;
	vecPosition.push_back(D3DXVECTOR3(-1, -1, -1));
	vecPosition.push_back(D3DXVECTOR3(-1,  1, -1));
	vecPosition.push_back(D3DXVECTOR3( 1,  1, -1));
	vecPosition.push_back(D3DXVECTOR3( 1, -1, -1));
	vecPosition.push_back(D3DXVECTOR3(-1, -1,  1));
	vecPosition.push_back(D3DXVECTOR3(-1,  1,  1));
	vecPosition.push_back(D3DXVECTOR3( 1,  1,  1));
	vecPosition.push_back(D3DXVECTOR3( 1, -1,  1));

	std::vector<int> vecIndex;
	vecIndex.push_back(0);
	vecIndex.push_back(1);
	vecIndex.push_back(2);

	vecIndex.push_back(0);
	vecIndex.push_back(2);
	vecIndex.push_back(3);

	vecIndex.push_back(4);
	vecIndex.push_back(6);
	vecIndex.push_back(5);

	vecIndex.push_back(4);
	vecIndex.push_back(7);
	vecIndex.push_back(6);

	vecIndex.push_back(4);
	vecIndex.push_back(5);
	vecIndex.push_back(1);

	vecIndex.push_back(4);
	vecIndex.push_back(1);
	vecIndex.push_back(0);

	vecIndex.push_back(3);
	vecIndex.push_back(2);
	vecIndex.push_back(6);

	vecIndex.push_back(3);
	vecIndex.push_back(6);
	vecIndex.push_back(7);

	vecIndex.push_back(1);
	vecIndex.push_back(5);
	vecIndex.push_back(6);

	vecIndex.push_back(1);
	vecIndex.push_back(6);
	vecIndex.push_back(2);

	vecIndex.push_back(4);
	vecIndex.push_back(0);
	vecIndex.push_back(3);

	vecIndex.push_back(4);
	vecIndex.push_back(3);
	vecIndex.push_back(7);

	ST_PN_VERTEX v;
	
	for (size_t i = 0; i < vecIndex.size(); ++i)
	{
		v.p = vecPosition[vecIndex[i]];
		m_vecVertex.push_back(v);
	}

	for (size_t i = 0; i < vecIndex.size(); i += 3)
	{
		D3DXVECTOR3 v0 = vecPosition[vecIndex[i]];
		D3DXVECTOR3 v1 = vecPosition[vecIndex[i + 1]];
		D3DXVECTOR3 v2 = vecPosition[vecIndex[i + 2]];
		D3DXVECTOR3 v01 = v1 - v0;
		D3DXVECTOR3 v02 = v2 - v0;
		D3DXVECTOR3 n;
		D3DXVec3Cross(&n, &v01, &v02);
		D3DXVec3Normalize(&n, &n);
		m_vecVertex[i].n = m_vecVertex[i + 1].n = m_vecVertex[i + 2].n = n;
	}

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
}

void cCube::Render(D3DXMATRIX* pWorldTM)
{
	if (pWorldTM)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, pWorldTM);
	}
	else
	{
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	}
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetFVF(ST_PN_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 
		m_vecVertex.size() / 3, 
		&m_vecVertex[0], 
		sizeof(ST_PN_VERTEX));
}