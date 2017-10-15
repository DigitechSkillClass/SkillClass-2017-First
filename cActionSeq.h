#pragma once

#include "cAction.h"

class cActionSeq 
	: public cAction
	, public iActionDelegate
{
protected:
	std::vector<cAction*> m_vecAction;
	int m_nCurrAction;

public:
	cActionSeq();
	virtual ~cActionSeq();

	virtual void AddAction(cAction* pAction);

	// cAction override
	virtual void Start() override;
	virtual void Update() override;

	// iActionDelegate override
	virtual void OnActionFinish(cAction* pSender) override;
};

