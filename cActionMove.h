#pragma once
#include "cAction.h"

class cActionMove : public cAction
{
private:
	CC_SYNTHESIZE(D3DXVECTOR3, m_vFrom, From);
	CC_SYNTHESIZE(D3DXVECTOR3, m_vTo, To);
	CC_SYNTHESIZE(float, m_fActionTime, ActionTime);

private:
	float m_fPassedActionTime;

public:
	cActionMove();
	virtual ~cActionMove();

	virtual void Start() override;
	virtual void Update() override;
};

