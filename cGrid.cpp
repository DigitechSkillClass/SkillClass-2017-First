#include "DXUT.h"
#include "cGrid.h"
#include "cPyramid.h"

cGrid::cGrid()
{
}

cGrid::~cGrid()
{
	for each (auto p in m_vecPyramid)
	{
		SAFE_DELETE(p);
	}
	m_vecPyramid.clear();
}

void cGrid::Setup(int nNumLine, float fInterval)
{
	ST_PC_VERTEX v;
	int nHalf = nNumLine / 2;
	float fMax = nHalf * fInterval;
	for (int i = 1; i <= nHalf; ++i)
	{
		if (i % 5 == 0)
			v.c = D3DCOLOR_XRGB(255, 255, 255);
		else
			v.c = D3DCOLOR_XRGB(128, 128, 128);

		v.p = D3DXVECTOR3(-fMax, 0, i * fInterval);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(fMax, 0, i * fInterval);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(-fMax, 0, -i * fInterval);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(fMax, 0, -i * fInterval);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(i * fInterval, 0, -fMax);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(i * fInterval, 0, fMax);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(-i * fInterval, 0, -fMax);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(-i * fInterval, 0, fMax);
		m_vecVertex.push_back(v);
	}

	v.c = D3DCOLOR_XRGB(255, 0, 0);
	v.p = D3DXVECTOR3(-fMax, 0, 0);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(fMax, 0, 0);
	m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 255, 0);
	v.p = D3DXVECTOR3(0, -fMax, 0);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, fMax, 0);
	m_vecVertex.push_back(v);

	v.c = D3DCOLOR_XRGB(0, 0, 255);
	v.p = D3DXVECTOR3(0, 0, -fMax);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, 0, fMax);
	m_vecVertex.push_back(v);

	D3DXMATRIX matS, matR, mat;
	D3DXMatrixScaling(&matS, 0.1f, 2.0f, 0.1f);
	D3DXMatrixRotationZ(&matR, D3DX_PI / 2.0f);
	mat = matS * matR;
	cPyramid* pPyramid = new cPyramid;
	pPyramid->Setup(D3DCOLOR_XRGB(255, 0, 0), &mat);
	m_vecPyramid.push_back(pPyramid);

	D3DXMatrixRotationZ(&matR, D3DX_PI);
	mat = matS * matR;
	pPyramid = new cPyramid;
	pPyramid->Setup(D3DCOLOR_XRGB(0, 255, 0), &mat);
	m_vecPyramid.push_back(pPyramid);

	D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	mat = matS * matR;
	pPyramid = new cPyramid;
	pPyramid->Setup(D3DCOLOR_XRGB(0, 0, 255), &mat);
	m_vecPyramid.push_back(pPyramid);
}

void cGrid::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,
		m_vecVertex.size() / 2,
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));

	for each(auto p in m_vecPyramid)
	{
		p->Render();
	}
}