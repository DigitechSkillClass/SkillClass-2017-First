#include "DXUT.h"
#include "cAseLoader.h"
#include "cMtlTex.h"
#include "cFrame.h"

cAseLoader::cAseLoader()
	: m_fp(0)
{
}

cAseLoader::~cAseLoader()
{
}

cFrame* cAseLoader::Load(IN char* szFullPath)
{
	cFrame* pRoot = NULL;

	fopen_s(&m_fp, szFullPath, "r");

	while (true)
	{
		char* szToken = GetToken();
		//OutputDebugStringA(szToken);
		if (szToken == NULL) break;
		if (IsEqual(szToken, "*SCENE"))
		{
			SkipBlock();
		}
		else if (IsEqual(szToken, "*MATERIAL_LIST"))
		{
			ProcessMATERIAL_LIST();
		}
		else if (IsEqual(szToken, "*GEOMOBJECT"))
		{
			cFrame* pFrame = ProcessGEOMOBJECT();
			if (!pRoot)
			{
				pRoot = pFrame;
			}
		}
	}

	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}

	fclose(m_fp);

	if (pRoot)
		pRoot->CalcOrigLocalTM(NULL);

	return pRoot;
}

char* cAseLoader::GetToken()
{
	int nReadCount = 0;
	bool isQuote = false;

	while (!feof(m_fp))
	{
		char c = fgetc(m_fp);
		
		if (c == '\"')
		{
			if (isQuote) break;
			isQuote = true;
			continue;
		}

		if (IsWhite(c) && !isQuote)
		{
			if (nReadCount > 0) break;
			continue;
		}
		m_szToken[nReadCount++] = c;
	}

	if (nReadCount == 0)
		return NULL;
	
	m_szToken[nReadCount] = '\0';
	return m_szToken;
}

int	cAseLoader::GetInteger()
{
	return atoi(GetToken());
}

float cAseLoader::GetFloat()
{
	return (float)atof(GetToken());
}

bool cAseLoader::IsWhite(char c)
{
	return c < 33;
}

void cAseLoader::SkipBlock()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
	} while (nLevel > 0);
}

bool cAseLoader::IsEqual(char* str1, char* str2)
{
	return strcmp(str1, str2) == 0;
}

void cAseLoader::ProcessMATERIAL_LIST()
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*MATERIAL_COUNT"))
		{
			int nMtlCount = GetInteger();
			m_vecMtlTex.resize(nMtlCount);
		}
		else if (IsEqual(szToken, "*MATERIAL"))
		{
			int nMtlRef = GetInteger();
			m_vecMtlTex[nMtlRef] = new cMtlTex;
			ProcessMATERIAL(m_vecMtlTex[nMtlRef]);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMATERIAL(cMtlTex* pMtlTex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*MATERIAL_AMBIENT"))
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			stMtl.Ambient.r = GetFloat();
			stMtl.Ambient.g = GetFloat();
			stMtl.Ambient.b = GetFloat();
			stMtl.Ambient.a = 1.0f;
		}
		else if (IsEqual(szToken, "*MATERIAL_DIFFUSE"))
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			stMtl.Diffuse.r = GetFloat();
			stMtl.Diffuse.g = GetFloat();
			stMtl.Diffuse.b = GetFloat();
			stMtl.Diffuse.a = 1.0f;
		}
		else if (IsEqual(szToken, "*MATERIAL_SPECULAR"))
		{
			D3DMATERIAL9& stMtl = pMtlTex->GetMtl();
			stMtl.Specular.r = GetFloat();
			stMtl.Specular.g = GetFloat();
			stMtl.Specular.b = GetFloat();
			stMtl.Specular.a = 1.0f;
		}
		else if (IsEqual(szToken, "*MAP_DIFFUSE"))
		{
			ProcessMAP_DIFFUSE(pMtlTex);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMAP_DIFFUSE(cMtlTex* pMtlTex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*BITMAP"))
		{
			char* szPath = GetToken();
			pMtlTex->SetTexture(g_pTextureManager->GetTexture(szPath));
		}
	} while (nLevel > 0);
}

cFrame* cAseLoader::ProcessGEOMOBJECT()
{
	cFrame* pFrame = new cFrame;
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*NODE_NAME"))
		{
			m_mapFrame[GetToken()] = pFrame;
		}
		else if (IsEqual(szToken, "*NODE_PARENT"))
		{
			cFrame* pParent = m_mapFrame[GetToken()];
			pParent->AddChild(pFrame);
		}
		else if (IsEqual(szToken, "*NODE_TM"))
		{
			ProcessNODE_TM(pFrame);
		}
		else if (IsEqual(szToken, "*MESH"))
		{
			ProcessMESH(pFrame);
		}
		else if (IsEqual(szToken, "*MATERIAL_REF"))
		{
			int nMtlRef = GetInteger();
			pFrame->SetMtlTex(m_vecMtlTex[nMtlRef]);
		}
		else if (IsEqual(szToken, "*TM_ANIMATION"))
		{
			ProcessTM_ANIMATION(pFrame);
		}
	} while (nLevel > 0);


	return pFrame;
}

void cAseLoader::ProcessMESH(cFrame* pFrame)
{
	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<ST_PNT_VERTEX> vecVertex;

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*MESH_NUMVERTEX"))
		{
			vecV.resize(GetInteger());
		}
		else if (IsEqual(szToken, "*MESH_NUMFACES"))
		{
			vecVertex.resize(3 * GetInteger());
		}
		else if (IsEqual(szToken, "*MESH_VERTEX_LIST"))
		{
			ProcessMESH_VERTEX_LIST(vecV);
		}
		else if (IsEqual(szToken, "*MESH_FACE_LIST"))
		{
			ProcessMESH_FACE_LIST(vecV, vecVertex);
		}
		else if (IsEqual(szToken, "*MESH_NUMTVERTEX"))
		{
			vecVT.resize(GetInteger());
		}
		else if (IsEqual(szToken, "*MESH_TVERTLIST"))
		{
			ProcessMESH_TVERTLIST(vecVT);
		}
		else if (IsEqual(szToken, "*MESH_TFACELIST"))
		{
			ProcessMESH_TFACELIST(vecVT, vecVertex);
		}
		else if (IsEqual(szToken, "*MESH_NORMALS"))
		{
			ProcessMESH_NORMALS(vecVertex);
		}
		
	} while (nLevel > 0);

	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, 0, &(pFrame->GetWorldTM()));
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		D3DXVec3TransformCoord(&vecVertex[i].p, &vecVertex[i].p, &matInvWorld);
		D3DXVec3TransformNormal(&vecVertex[i].n, &vecVertex[i].n, &matInvWorld);
	}

	pFrame->SetVertex(vecVertex);
}

void cAseLoader::ProcessMESH_VERTEX_LIST(OUT std::vector<D3DXVECTOR3>& vecV)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*MESH_VERTEX"))
		{
			int nIndex = GetInteger();
			vecV[nIndex].x = GetFloat();
			vecV[nIndex].z = GetFloat();
			vecV[nIndex].y = GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_FACE_LIST(IN std::vector<D3DXVECTOR3>& vecV,
	OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*MESH_FACE"))
		{
			int nFaceIndex = GetInteger();
			GetToken(); // A:
			int nA = GetInteger();
			GetToken(); // B:
			int nB = GetInteger();
			GetToken(); // C:
			int nC = GetInteger();
			vecVertex[nFaceIndex * 3 + 0].p = vecV[nA];
			vecVertex[nFaceIndex * 3 + 1].p = vecV[nC];
			vecVertex[nFaceIndex * 3 + 2].p = vecV[nB];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TVERTLIST(OUT std::vector<D3DXVECTOR2>& vecVT)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*MESH_TVERT"))
		{
			int nIndex = GetInteger();
			vecVT[nIndex].x = GetFloat();
			vecVT[nIndex].y = 1.0f - GetFloat();
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_TFACELIST(IN std::vector<D3DXVECTOR2>& vecVT,
	OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*MESH_TFACE"))
		{
			int nFaceIndex = GetInteger();
			int nA = GetInteger();
			int nB = GetInteger();
			int nC = GetInteger();
			vecVertex[nFaceIndex * 3 + 0].t = vecVT[nA];
			vecVertex[nFaceIndex * 3 + 1].t = vecVT[nC];
			vecVertex[nFaceIndex * 3 + 2].t = vecVT[nB];
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessMESH_NORMALS(OUT std::vector<ST_PNT_VERTEX>& vecVertex)
{
	int nFaceIndex = 0;
	int nCnt = 0;
	int aModCnt[] = { 0, 2, 1 };

	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*MESH_FACENORMAL"))
		{
			nFaceIndex = GetInteger();
			nCnt = 0;
		}
		else if (IsEqual(szToken, "*MESH_VERTEXNORMAL"))
		{
			GetToken();
			float x = GetFloat();
			float z = GetFloat();
			float y = GetFloat();
			vecVertex[nFaceIndex * 3 + aModCnt[nCnt]].n = D3DXVECTOR3(x, y, z);
			nCnt++;
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessTM_ANIMATION(cFrame * pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*CONTROL_POS_TRACK"))
		{
			ProcessCONTROL_POS_TRACK(pFrame);
		}
		else if (IsEqual(szToken, "*CONTROL_ROT_TRACK"))
		{
			ProcessCONTROL_ROT_TRACK(pFrame);
		}
	} while (nLevel > 0);
}

void cAseLoader::ProcessNODE_TM(cFrame * pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*TM_ROW0"))
		{
			D3DXMATRIX& matWorld = pFrame->GetWorldTM();
			matWorld._11 = GetFloat();
			matWorld._13 = GetFloat();
			matWorld._12 = GetFloat();
			matWorld._14 = 0.0f;
		}
		else if (IsEqual(szToken, "*TM_ROW1"))
		{
			D3DXMATRIX& matWorld = pFrame->GetWorldTM();
			matWorld._31 = GetFloat();
			matWorld._33 = GetFloat();
			matWorld._32 = GetFloat();
			matWorld._34 = 0.0f;
		}
		else if (IsEqual(szToken, "*TM_ROW2"))
		{
			D3DXMATRIX& matWorld = pFrame->GetWorldTM();
			matWorld._21 = GetFloat();
			matWorld._23 = GetFloat();
			matWorld._22 = GetFloat();
			matWorld._24 = 0.0f;
		}
		else if (IsEqual(szToken, "*TM_ROW3"))
		{
			D3DXMATRIX& matWorld = pFrame->GetWorldTM();
			matWorld._41 = GetFloat();
			matWorld._43 = GetFloat();
			matWorld._42 = GetFloat();
			matWorld._44 = 1.0f;
		}
	} while (nLevel > 0);
}


void cAseLoader::ProcessCONTROL_POS_TRACK(cFrame* pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*CONTROL_POS_SAMPLE"))
		{
			ST_POS_SAMPLE ps;
			ps.n = GetInteger();
			ps.p.x = GetFloat();
			ps.p.z = GetFloat();
			ps.p.y = GetFloat();

			pFrame->m_vecPosTrack.push_back(ps);
		}
	} while (nLevel > 0);
}
void cAseLoader::ProcessCONTROL_ROT_TRACK(cFrame* pFrame)
{
	int nLevel = 0;
	do
	{
		char* szToken = GetToken();
		if (IsEqual(szToken, "{"))
		{
			nLevel++;
		}
		else if (IsEqual(szToken, "}"))
		{
			nLevel--;
		}
		else if (IsEqual(szToken, "*CONTROL_ROT_SAMPLE"))
		{
			ST_ROT_SAMPLE rs;
			rs.n = GetInteger();
			rs.q.x = GetFloat();
			rs.q.z = GetFloat();
			rs.q.y = GetFloat();
			rs.q.w = GetFloat();

			rs.q.x = rs.q.x * sinf(rs.q.w / 2.0f);
			rs.q.y = rs.q.y * sinf(rs.q.w / 2.0f);
			rs.q.z = rs.q.z * sinf(rs.q.w / 2.0f);
			rs.q.w = cosf(rs.q.w / 2.0f);

			if (!pFrame->m_vecRotTrack.empty())
			{
				rs.q = pFrame->m_vecRotTrack.back().q * rs.q;
			}
			pFrame->m_vecRotTrack.push_back(rs);
		}
	} while (nLevel > 0);
}