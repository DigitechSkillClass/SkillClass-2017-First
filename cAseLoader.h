#pragma once

class cMtlTex;
class cFrame;

class cAseLoader
{
private:
	FILE*	m_fp;
	char	m_szToken[1024];
	std::vector<cMtlTex*> m_vecMtlTex;
	std::map<std::string, cFrame*> m_mapFrame;

public:
	cAseLoader();
	~cAseLoader();

	cFrame* Load(IN char* szFullPath);

	char*	GetToken();
	int		GetInteger();
	float	GetFloat();
	bool	IsWhite(char c);
	void	SkipBlock();
	bool	IsEqual(char* str1, char* str2);
	
	void ProcessMATERIAL_LIST();
	void ProcessMATERIAL(cMtlTex* pMtlTex);
	void ProcessMAP_DIFFUSE(cMtlTex* pMtlTex);
	cFrame* ProcessGEOMOBJECT();
	void ProcessMESH(cFrame* pFrame);
	void ProcessMESH_VERTEX_LIST(OUT std::vector<D3DXVECTOR3>& vecV);
	void ProcessMESH_FACE_LIST(IN std::vector<D3DXVECTOR3>& vecV, OUT std::vector<ST_PNT_VERTEX>& vecVertex);
	void ProcessMESH_TVERTLIST(OUT std::vector<D3DXVECTOR2>& vecVT);
	void ProcessMESH_TFACELIST(IN std::vector<D3DXVECTOR2>& vecVT, OUT std::vector<ST_PNT_VERTEX>& vecVertex);
	void ProcessMESH_NORMALS(OUT std::vector<ST_PNT_VERTEX>& vecVertex);
	void ProcessTM_ANIMATION(cFrame* pFrame);
	void ProcessNODE_TM(cFrame* pFrame);
	void ProcessCONTROL_POS_TRACK(cFrame* pFrame);
	void ProcessCONTROL_ROT_TRACK(cFrame* pFrame);
};

