#pragma once
class cMtlTex : public cObject
{	
protected:
	CC_SYNTHESIZE_PASS_BY_REF(int, m_nAttrID, AttrID);
	CC_SYNTHESIZE_PASS_BY_REF(D3DMATERIAL9, m_stMtl, Mtl);
	CC_SYNTHESIZE_ADDREF(LPDIRECT3DTEXTURE9, m_pTexture, Texture);

	//LPDIRECT3DTEXTURE9 m_pTexture;
public:
	cMtlTex();
	virtual ~cMtlTex();
	/*
	void SetTexture(LPDIRECT3DTEXTURE9 pTexture)
	{
		if (m_pTexture != pTexture)
		{
			SAFE_RELEASE(m_pTexture);
			SAFE_ADDREF(pTexture);
			m_pTexture = pTexture;
		}
	}*/
};


