#pragma once

class cGameObject;
class cAction;

class iActionDelegate
{
public:
	virtual void OnActionFinish(cAction* pSender) PURE;
};

class cAction : public cObject
{
protected:
	cGameObject* m_pTarget;

	CC_SYNTHESIZE(iActionDelegate*, m_pDelegate, Delegate);

public:
	cAction();
	virtual ~cAction();

	virtual void Start() PURE;
	virtual void Update() PURE;
	virtual void SetTarget(cGameObject* pTarget);
};

