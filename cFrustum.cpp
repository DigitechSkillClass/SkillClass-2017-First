#include "DXUT.h"
#include "cFrustum.h"


cFrustum::cFrustum()
{
	m_vecProjVertex.push_back(D3DXVECTOR3(-1, 1, 0));
	m_vecProjVertex.push_back(D3DXVECTOR3( 1, 1, 0));
	m_vecProjVertex.push_back(D3DXVECTOR3( 1,-1, 0));
	m_vecProjVertex.push_back(D3DXVECTOR3(-1,-1, 0));
	m_vecProjVertex.push_back(D3DXVECTOR3(-1, 1, 1));
	m_vecProjVertex.push_back(D3DXVECTOR3( 1, 1, 1));
	m_vecProjVertex.push_back(D3DXVECTOR3( 1,-1, 1));
	m_vecProjVertex.push_back(D3DXVECTOR3(-1,-1, 1));

	m_vecPlane.resize(6);
}


cFrustum::~cFrustum()
{
}

void cFrustum::Update()
{
	D3DXMATRIX matProj, matView, matInvProj, matInvView, matInvProjView;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvProj, 0, &matProj);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	matInvProjView = matInvProj * matInvView;
	D3DXVECTOR3 aWorldVertex[8];
	for (int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&aWorldVertex[i], &m_vecProjVertex[i], &matInvProjView);
	}
	int nIndex = 0;
	D3DXPlaneFromPoints(&m_vecPlane[nIndex++], &aWorldVertex[0], &aWorldVertex[1], &aWorldVertex[2]);
	D3DXPlaneFromPoints(&m_vecPlane[nIndex++], &aWorldVertex[1], &aWorldVertex[5], &aWorldVertex[6]);
	D3DXPlaneFromPoints(&m_vecPlane[nIndex++], &aWorldVertex[5], &aWorldVertex[4], &aWorldVertex[7]);
	D3DXPlaneFromPoints(&m_vecPlane[nIndex++], &aWorldVertex[4], &aWorldVertex[0], &aWorldVertex[3]);
	D3DXPlaneFromPoints(&m_vecPlane[nIndex++], &aWorldVertex[4], &aWorldVertex[5], &aWorldVertex[1]);
	D3DXPlaneFromPoints(&m_vecPlane[nIndex++], &aWorldVertex[2], &aWorldVertex[6], &aWorldVertex[7]);
}

bool cFrustum::IsIn(ST_SPHERE * pSphere)
{
	for each(auto p in m_vecPlane)
	{
		if (D3DXPlaneDotCoord(&p, &pSphere->vCenter) > pSphere->fRadius)
			return false;
	}

	return true;
}
