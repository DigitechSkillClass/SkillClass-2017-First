#pragma once
class cRay
{
public:
	cRay();
	~cRay();

	D3DXVECTOR3 m_vRayOrg;
	D3DXVECTOR3 m_vRayDir;

	static cRay GetRayAtView(int nScreenX, int nScreenY);
	static cRay GetRayAtWorld(int nScreenX, int nScreenY);
};

