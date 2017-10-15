#include "DXUT.h"
#include "cRay.h"


cRay::cRay()
	: m_vRayOrg(0, 0, 0)
	, m_vRayDir(0, 0, 1)
{
}


cRay::~cRay()
{
}

cRay cRay::GetRayAtView(int nScreenX, int nScreenY)
{
	D3DVIEWPORT9 vp;
	g_pD3DDevice->GetViewport(&vp);

	D3DXMATRIX matProj;
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	cRay r;
	r.m_vRayDir.x = (2.0f * nScreenX / vp.Width - 1) / matProj._11;
	r.m_vRayDir.y = (-2.0f * nScreenY / vp.Height + 1) / matProj._22;
	r.m_vRayDir.z = 1.0f;
	return r;
}

cRay cRay::GetRayAtWorld(int nScreenX, int nScreenY)
{
	D3DXMATRIX matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	cRay r = cRay::GetRayAtView(nScreenX, nScreenY);
	D3DXVec3TransformCoord(&r.m_vRayOrg, &r.m_vRayOrg, &matInvView);
	D3DXVec3TransformNormal(&r.m_vRayDir, &r.m_vRayDir, &matInvView);
	D3DXVec3Normalize(&r.m_vRayDir, &r.m_vRayDir);
	return r;
}
