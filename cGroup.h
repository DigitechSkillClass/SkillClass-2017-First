#pragma once
#include "cMtlTex.h"

class cGroup
{
	CC_SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	CC_SYNTHESIZE_ADDREF(cMtlTex*, m_pMtlTex, MtlTex);

public:
	cGroup();
	virtual ~cGroup();
	
	void Render();
};

