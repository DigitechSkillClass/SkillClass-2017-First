#pragma once
#include "cAction.h"
class cActionRepeat
	: public cAction
	, public iActionDelegate
{
protected:
	CC_SYNTHESIZE_ADDREF(cAction*, m_pAction, Action);

public:
	cActionRepeat();
	virtual ~cActionRepeat();

	// cAction override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish(cAction* pSender) override;
};

