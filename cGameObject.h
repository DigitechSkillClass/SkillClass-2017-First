#pragma once
#include "cAction.h"

class cGameObject : public cObject
{
private:
	CC_SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);
	CC_SYNTHESIZE_ADDREF(LPD3DXMESH, m_pMesh, Mesh);
	CC_SYNTHESIZE_ADDREF(cAction*, m_pAction, Action);

public:
	cGameObject();
	virtual ~cGameObject();

	virtual void Update();
	virtual void Render();
};

