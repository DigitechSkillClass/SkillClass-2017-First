#pragma once
#include "cMtlTex.h"

class cFrame
{
	CC_SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matLocalTM, LocalTM);
	CC_SYNTHESIZE_PASS_BY_REF(D3DXMATRIX, m_matWorldTM, WorldTM);
	CC_SYNTHESIZE_PASS_BY_REF(std::vector<ST_PNT_VERTEX>, m_vecVertex, Vertex);
	CC_SYNTHESIZE_ADDREF(cMtlTex*, m_pMtlTex, MtlTex);
public:
	std::vector<ST_POS_SAMPLE> m_vecPosTrack;
	std::vector<ST_ROT_SAMPLE> m_vecRotTrack;
protected:
	std::vector<cFrame*> m_vecChild;

public:
	cFrame();
	virtual ~cFrame();

	void Render();
	void AddChild(cFrame* pChild);
	void CalcOrigLocalTM(cFrame* pParentWorldTM);
	void Update(int nKeyFrame, D3DXMATRIX* pParent);
	void LocalTranslationMatrixAtTime(IN int nKeyFrame, OUT D3DXMATRIX& mat);
	void LocalRotationMatrixAtTime(IN int nKeyFrame, OUT D3DXMATRIX& mat);
};

