#include "DXUT.h"
#include "cObjLoader.h"
#include "cMtlTex.h"
#include "cGroup.h"

cObjLoader::cObjLoader()
{
}

cObjLoader::~cObjLoader()
{
}

void cObjLoader::Load(IN char* szPath, OUT std::vector<cGroup*>& vecGroup)
{
	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR3> vecVN;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<ST_PNT_VERTEX> vecVertex;

	FILE* fp = NULL;
	fopen_s(&fp, szPath, "r");

	char szBuf[1024];
	std::string sMtlName;

	while (!feof(fp))
	{
		szBuf[0] = '\0';
		fgets(szBuf, 1024, fp);
		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (szBuf[0] == 'm')
		{
			char szMtlLibPath[1024] = { 0, };
			sscanf(szBuf, "%*s %s", szMtlLibPath);
			LoadMtlLib(szMtlLibPath);
		}
		else if (szBuf[0] == 'g')
		{
			if (!vecVertex.empty())
			{
				cGroup* pGroup = new cGroup;
				pGroup->SetMtlTex(m_mapMtlTex[sMtlName]);
				pGroup->SetVertex(vecVertex);
				vecGroup.push_back(pGroup);
				vecVertex.clear();
			}
		}
		else if (szBuf[0] == 'v')
		{
			if (szBuf[1] == 't')
			{
				float u, v;
				sscanf(szBuf, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, v));
			}
			else if (szBuf[1] == 'n')
			{
				float x, y, z;
				sscanf(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else
			{
				float x, y, z;
				sscanf(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szBuf[0] == 'u')
		{
			char szMtlName[1024] = { 0, };
			sscanf(szBuf, "%*s %s", szMtlName);
			sMtlName = std::string(szMtlName);
		}
		else if (szBuf[0] == 'f')
		{
			int aIndex[3][3];
			sscanf(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d", 
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

			for (size_t i = 0; i < 3; i++)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[aIndex[i][0] - 1];
				v.n = vecVN[aIndex[i][2] - 1];
				v.t = vecVT[aIndex[i][1] - 1];
				vecVertex.push_back(v);
			}
		}
	}

	fclose(fp);

	for each (auto it in m_mapMtlTex)
	{
		SAFE_RELEASE(it.second);
	}
}

LPD3DXMESH cObjLoader::LoadMesh(IN char * szPath, OUT std::vector<cMtlTex*>& vecMtlTex)
{
	LPD3DXMESH pMesh = NULL;

	std::vector<D3DXVECTOR3> vecV;
	std::vector<D3DXVECTOR3> vecVN;
	std::vector<D3DXVECTOR2> vecVT;
	std::vector<ST_PNT_VERTEX> vecVertex;
	std::vector<DWORD> vecAttr;

	FILE* fp = NULL;
	fopen_s(&fp, szPath, "r");

	char szBuf[1024];
	std::string sMtlName;

	while (!feof(fp))
	{
		szBuf[0] = '\0';
		fgets(szBuf, 1024, fp);
		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (szBuf[0] == 'm')
		{
			char szMtlLibPath[1024] = { 0, };
			sscanf(szBuf, "%*s %s", szMtlLibPath);
			LoadMtlLib(szMtlLibPath);
		}
		else if (szBuf[0] == 'g')
		{	
		}
		else if (szBuf[0] == 'v')
		{
			if (szBuf[1] == 't')
			{
				float u, v;
				sscanf(szBuf, "%*s %f %f %*f", &u, &v);
				vecVT.push_back(D3DXVECTOR2(u, v));
			}
			else if (szBuf[1] == 'n')
			{
				float x, y, z;
				sscanf(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecVN.push_back(D3DXVECTOR3(x, y, z));
			}
			else
			{
				float x, y, z;
				sscanf(szBuf, "%*s %f %f %f", &x, &y, &z);
				vecV.push_back(D3DXVECTOR3(x, y, z));
			}
		}
		else if (szBuf[0] == 'u')
		{
			char szMtlName[1024] = { 0, };
			sscanf(szBuf, "%*s %s", szMtlName);
			sMtlName = std::string(szMtlName);
		}
		else if (szBuf[0] == 'f')
		{
			int aIndex[3][3];
			sscanf(szBuf, "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
				&aIndex[0][0], &aIndex[0][1], &aIndex[0][2],
				&aIndex[1][0], &aIndex[1][1], &aIndex[1][2],
				&aIndex[2][0], &aIndex[2][1], &aIndex[2][2]);

			for (size_t i = 0; i < 3; i++)
			{
				ST_PNT_VERTEX v;
				v.p = vecV[aIndex[i][0] - 1];
				v.n = vecVN[aIndex[i][2] - 1];
				v.t = vecVT[aIndex[i][1] - 1];
				vecVertex.push_back(v);
			}
			vecAttr.push_back(m_mapMtlTex[sMtlName]->GetAttrID());
		}
	}

	fclose(fp);

	vecMtlTex.resize(m_mapMtlTex.size());
	for each(auto it in m_mapMtlTex)
	{
		int nAttrID = it.second->GetAttrID();
		vecMtlTex[nAttrID] = it.second;
	}

	D3DXCreateMeshFVF(vecVertex.size() / 3,
		vecVertex.size(),
		D3DXMESH_MANAGED,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&pMesh);

	ST_PNT_VERTEX* pV = 0;
	pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	pMesh->UnlockVertexBuffer();

	WORD* pI = 0;
	pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	for (size_t i = 0; i < vecVertex.size(); ++i)
	{
		pI[i] = i;
	}
	pMesh->UnlockIndexBuffer();

	DWORD* pA = 0;
	pMesh->LockAttributeBuffer(0, &pA);
	memcpy(pA, &vecAttr[0], vecAttr.size() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj;
	vecAdj.resize(pMesh->GetNumFaces() * 3);
	pMesh->GenerateAdjacency(0, &vecAdj[0]);

	pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE,
		&vecAdj[0], 0, 0, 0);

	return pMesh;
}

void cObjLoader::LoadMtlLib(char* szPath)
{
	m_mapMtlTex.clear();

	std::string sMtlName;

	FILE* fp = NULL;
	fopen_s(&fp, szPath, "r");

	char szBuf[1024];

	while (!feof(fp))
	{
		szBuf[0] = '\0';
		fgets(szBuf, 1024, fp);
		if (szBuf[0] == '#')
		{
			continue;
		}
		else if (szBuf[0] == 'n')
		{
			char szMtlName[1024] = { 0, };
			sscanf(szBuf, "%*s %s", szMtlName);
			sMtlName = std::string(szMtlName);
			int nSize = (int)m_mapMtlTex.size();
			m_mapMtlTex[sMtlName] = new cMtlTex;
			m_mapMtlTex[sMtlName]->SetAttrID(nSize);
		}
		else if (szBuf[0] == 'K')
		{
			float r, g, b;
			
			D3DMATERIAL9& stMtl = m_mapMtlTex[sMtlName]->GetMtl();

			if (szBuf[1] == 'a')
			{
				sscanf(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Ambient = D3DXCOLOR(r, g, b, 1.0f);
			}
			else if (szBuf[1] == 'd')
			{
				sscanf(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Diffuse = D3DXCOLOR(r, g, b, 1.0f);
			}
			else if (szBuf[1] == 's')
			{
				sscanf(szBuf, "%*s %f %f %f", &r, &g, &b);
				stMtl.Specular = D3DXCOLOR(r, g, b, 1.0f);
			}
		}
		else if (szBuf[0] == 'm')
		{
			char szTexPath[1024] = { 0, };
			sscanf(szBuf, "%*s %s", szTexPath);
			m_mapMtlTex[sMtlName]->SetTexture(g_pTextureManager->GetTexture(szTexPath));
		}
	}

	fclose(fp);
}
