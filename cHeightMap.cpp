#include "DXUT.h"
#include "cHeightMap.h"
#include "cCamera.h"

cHeightMap::cHeightMap()
	: m_pMesh(NULL)
	, m_nCol(0)
	, m_pEffect(NULL)
	, m_pBlurEffect(NULL)
	, m_pRenderTarget(NULL)
	, m_pDepthStencil(NULL)
{
}


cHeightMap::~cHeightMap()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pBlurEffect);
	SAFE_RELEASE(m_pRenderTarget);
	SAFE_RELEASE(m_pDepthStencil);
}

void cHeightMap::Load(char * szRawFile, char * szTexFile, int nBytesPerPixel)
{
	RECT rc;
	GetClientRect(DXUTGetHWND(), &rc);
	g_pD3DDevice->CreateTexture(rc.right, rc.bottom, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pRenderTarget, NULL);
	g_pD3DDevice->CreateDepthStencilSurface(rc.right, rc.bottom, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, true, &m_pDepthStencil, NULL);

	ST_PT_VERTEX v;
	v.p = D3DXVECTOR4(0, 0, 0, 0);
	v.t = D3DXVECTOR2(0, 0);
	m_vecScreen.push_back(v);

	v.p = D3DXVECTOR4(rc.right, rc.bottom, 0, 0);
	v.t = D3DXVECTOR2(1, 1);
	m_vecScreen.push_back(v);

	v.p = D3DXVECTOR4(0, rc.bottom, 0, 0);
	v.t = D3DXVECTOR2(0, 1);
	m_vecScreen.push_back(v);

	v.p = D3DXVECTOR4(0, 0, 0, 0);
	v.t = D3DXVECTOR2(0, 0);
	m_vecScreen.push_back(v);

	v.p = D3DXVECTOR4(rc.right, 0, 0, 0);
	v.t = D3DXVECTOR2(1, 0);
	m_vecScreen.push_back(v);

	v.p = D3DXVECTOR4(rc.right, rc.bottom, 0, 0);
	v.t = D3DXVECTOR2(1, 1);
	m_vecScreen.push_back(v);

	ID3DXBuffer* pBuffer = 0;
	D3DXCreateEffectFromFile(g_pD3DDevice,
		L"Lighting.fx",
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		NULL);

	D3DXCreateEffectFromFile(g_pD3DDevice,
		L"Blur.fx",
		NULL,
		NULL,
		0,
		NULL,
		&m_pBlurEffect,
		&pBuffer);
	if (pBuffer)
	{
		char* sz = (char*)pBuffer->GetBufferPointer();
		OutputDebugStringA(sz);
		SAFE_RELEASE(pBuffer);
	}
	

	std::vector<ST_PNT_VERTEX> vecVertex;

	FILE* fp = 0;

	fopen_s(&fp, szRawFile, "rb");

	fseek(fp, 0, SEEK_END);
	int nBytes = ftell(fp);
	int nRow = (int)sqrt(nBytes + FLT_EPSILON);
	int nCol = nRow;
	m_nCol = nCol;

	assert(nBytes == nRow * nCol);
	fseek(fp, 0, SEEK_SET);

	vecVertex.reserve(nBytes);
	m_vecVertex.reserve(nBytes);

	int nTile = nRow - 1;

	for (int z = 0; z < nRow; ++z)
	{
		for (int x = 0; x < nCol; ++x)
		{
			ST_PNT_VERTEX v;
			v.p = D3DXVECTOR3(x, fgetc(fp) / 10.f, z);
			v.n = D3DXVECTOR3(0, 1, 0);
			v.t = D3DXVECTOR2(x / (float)nTile, z / (float)nTile);
			vecVertex.push_back(v);
			m_vecVertex.push_back(v.p);
		}
	}

	fclose(fp);

	for (int z = 1; z < nTile; ++z)
	{
		for (int x = 1; x < nTile; ++x)
		{
			int nIndex = z * nRow + x;
			D3DXVECTOR3 l = vecVertex[nIndex - 1].p;
			D3DXVECTOR3 r = vecVertex[nIndex + 1].p;
			D3DXVECTOR3 d = vecVertex[nIndex - nCol].p;
			D3DXVECTOR3 u = vecVertex[nIndex + nCol].p;
			D3DXVECTOR3 lr = r - l;
			D3DXVECTOR3 du = u - d;
			D3DXVECTOR3 n;
			D3DXVec3Cross(&n, &du, &lr);
			D3DXVec3Normalize(&n, &n);
			vecVertex[nIndex].n = n;
		}
	}
	
	// 1-3
	// |\|
	// 0-2

	std::vector<DWORD> vecIndex;
	
	vecIndex.reserve(nTile * nTile * 2 * 3);

	for (int z = 0; z < nTile; ++z)
	{
		for (int x = 0; x < nTile; ++x)
		{
			DWORD _0 = (z + 0) * nCol + x + 0;
			DWORD _1 = (z + 1) * nCol + x + 0;
			DWORD _2 = (z + 0) * nCol + x + 1;
			DWORD _3 = (z + 1) * nCol + x + 1;
			vecIndex.push_back(_0); vecIndex.push_back(_1); vecIndex.push_back(_2);
			vecIndex.push_back(_3); vecIndex.push_back(_2); vecIndex.push_back(_1);
		}
	}

	D3DXCreateMeshFVF(nTile * nTile * 2,
		vecVertex.size(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT,
		ST_PNT_VERTEX::FVF,
		g_pD3DDevice,
		&m_pMesh);

	ST_PNT_VERTEX* pV = 0;
	m_pMesh->LockVertexBuffer(0, (LPVOID*)&pV);
	memcpy(pV, &vecVertex[0], vecVertex.size() * sizeof(ST_PNT_VERTEX));
	m_pMesh->UnlockVertexBuffer();

	DWORD* pI = 0;
	m_pMesh->LockIndexBuffer(0, (LPVOID*)&pI);
	memcpy(pI, &vecIndex[0], vecIndex.size() * sizeof(DWORD));
	m_pMesh->UnlockIndexBuffer();

	DWORD* pA = 0;
	m_pMesh->LockAttributeBuffer(0, &pA);
	ZeroMemory(pA, nTile * nTile * 2 * sizeof(DWORD));
	m_pMesh->UnlockAttributeBuffer();

	std::vector<DWORD> vecAdj;
	vecAdj.resize(m_pMesh->GetNumFaces() * 3);
	m_pMesh->GenerateAdjacency(0, &vecAdj[0]);

	m_pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |
		D3DXMESHOPT_COMPACT |
		D3DXMESHOPT_VERTEXCACHE, 
		&vecAdj[0], 0, 0, 0);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	m_sTexture = std::string(szTexFile);
}


float ComputeGaussianValue(float x, float mean, float std_deviation)
{
	// The gaussian equation is defined as such:
	/*
	-(x - mean)^2
	-------------
	1.0               2*std_dev^2
	f(x,mean,std_dev) = -------------------- * e^
	sqrt(2*pi*std_dev^2)

	*/

	return (1.0f / sqrt(2.0f * D3DX_PI * std_deviation * std_deviation))
		* expf((-((x - mean) * (x - mean))) / (2.0f * std_deviation * std_deviation));
}

void cHeightMap::Render(cCamera* pCamera)
{
	LPDIRECT3DSURFACE9 pCurrentSurface = 0;
	LPDIRECT3DSURFACE9 pCurrentDepthStencil = 0;

	g_pD3DDevice->GetRenderTarget(0, &pCurrentSurface);
	g_pD3DDevice->GetDepthStencilSurface(&pCurrentDepthStencil);

	LPDIRECT3DSURFACE9 pRenderTargetSurface = 0;
	m_pRenderTarget->GetSurfaceLevel(0, &pRenderTargetSurface);
	g_pD3DDevice->SetRenderTarget(0, pRenderTargetSurface);
	g_pD3DDevice->SetDepthStencilSurface(m_pDepthStencil);

	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(92, 186, 137), 1.0f, 0);

	D3DXMATRIXA16 matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	g_pD3DDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	
	m_pEffect->SetMatrix((D3DXHANDLE)"matWorld", &matWorld);
	m_pEffect->SetMatrix((D3DXHANDLE)"matView", &matView);
	m_pEffect->SetMatrix((D3DXHANDLE)"matViewProjection", &(matView * matProj));
	m_pEffect->SetVector((D3DXHANDLE)"fvEyePosition", &D3DXVECTOR4(pCamera->GetEyePosition(), 1));
	m_pEffect->SetTexture((D3DXHANDLE)"splat1", g_pTextureManager->GetTexture("adesert_stone_d.jpg"));
	m_pEffect->SetTexture((D3DXHANDLE)"splat2", g_pTextureManager->GetTexture("desert_cracks_d.jpg"));
	m_pEffect->SetTexture((D3DXHANDLE)"splat3", g_pTextureManager->GetTexture("desert_rocky_d.jpg"));
	m_pEffect->SetTexture((D3DXHANDLE)"splat4", g_pTextureManager->GetTexture("grass_green2y_d.jpg"));
	m_pEffect->SetTexture((D3DXHANDLE)"splatData", g_pTextureManager->GetTexture("splat_data.png"));

	g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
	UINT uiNumPasses = 0;
	m_pEffect->Begin(&uiNumPasses, 0);
	for (UINT i = 0; i < uiNumPasses; ++i)
	{
		m_pEffect->BeginPass(i);
		
		m_pMesh->DrawSubset(0);

		m_pEffect->EndPass();
	}

	m_pEffect->End();
	
	g_pD3DDevice->SetRenderTarget(0, pCurrentSurface);
	g_pD3DDevice->SetDepthStencilSurface(pCurrentDepthStencil);

	SAFE_RELEASE(pCurrentSurface);
	SAFE_RELEASE(pCurrentDepthStencil);

	


	float VBloomWeights[9];
	float VBloomOffsets[9];

	D3DSURFACE_DESC destDesc;
	pRenderTargetSurface->GetDesc(&destDesc);

	for (int i = 0; i < 9; i++)
	{
		// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
		//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
		//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
		VBloomOffsets[i] = (static_cast< float >(i) - 4.0f) * (1.0f / static_cast< float >(destDesc.Height));

		// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
		float x = (static_cast< float >(i) - 4.0f) / 4.0f;


		// Use a gaussian distribution. Changing the standard-deviation
		// (second parameter) as well as the amplitude (multiplier) gives
		// distinctly different results.
		float g_GaussMultiplier = 0.4f;             // Default multiplier
		float g_GaussMean = 0.0f;             // Default mean for gaussian distribution
		float g_GaussStdDev = 0.8f;             // Default standard deviation for gaussian distribution
		VBloomWeights[i] = g_GaussMultiplier * ComputeGaussianValue(x, g_GaussMean, g_GaussStdDev);
	}
	
	g_pD3DDevice->SetFVF(ST_PT_VERTEX::FVF);
	
	// Commit both arrays to the device:
	m_pBlurEffect->SetFloatArray((D3DXHANDLE)"VBloomWeights", VBloomWeights, 9);
	m_pBlurEffect->SetFloatArray((D3DXHANDLE)"VBloomOffsets", VBloomOffsets, 9);
	m_pBlurEffect->SetTexture((D3DXHANDLE)"tex", m_pRenderTarget);

	m_pBlurEffect->Begin(&uiNumPasses, 0);
	for (UINT i = 0; i < uiNumPasses; ++i)
	{
		m_pBlurEffect->BeginPass(i);

		//m_pMesh->DrawSubset(0);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 6, &m_vecScreen[0], sizeof(ST_PT_VERTEX));

		m_pBlurEffect->EndPass();
	}

	m_pBlurEffect->End();
}

bool cHeightMap::GetHeight(IN float x, OUT float & y, IN float z)
{
	if (x < 0 || z < 0 || x > m_nCol - 1 || z > m_nCol - 1) return false;
	
	// 1-3
	// |\|
	// 0-2

	int nX = x;
	int nZ = z;
	float fDeltaX = x - nX;
	float fDeltaZ = z - nZ;
	if (fDeltaX + fDeltaZ < 1)
	{
		DWORD _0 = (nZ + 0) * m_nCol + nX + 0;
		DWORD _1 = (nZ + 1) * m_nCol + nX + 0;
		DWORD _2 = (nZ + 0) * m_nCol + nX + 1;
		//DWORD _3 = (nZ + 1) * m_nCol + nX + 1;
		D3DXVECTOR3 _01 = m_vecVertex[_1] - m_vecVertex[_0];
		D3DXVECTOR3 _02 = m_vecVertex[_2] - m_vecVertex[_0];
		y = m_vecVertex[_0].y + (_01 * fDeltaZ + _02 * fDeltaX).y;
	}
	else
	{
		fDeltaX = 1 - fDeltaX;
		fDeltaZ = 1 - fDeltaZ;
		//DWORD _0 = (nZ + 0) * m_nCol + nX + 0;
		DWORD _1 = (nZ + 1) * m_nCol + nX + 0;
		DWORD _2 = (nZ + 0) * m_nCol + nX + 1;
		DWORD _3 = (nZ + 1) * m_nCol + nX + 1;
		D3DXVECTOR3 _31 = m_vecVertex[_1] - m_vecVertex[_3];
		D3DXVECTOR3 _32 = m_vecVertex[_2] - m_vecVertex[_3];
		y = m_vecVertex[_3].y + (_31 * fDeltaX + _32 * fDeltaZ).y;
	}

	return true;
}
