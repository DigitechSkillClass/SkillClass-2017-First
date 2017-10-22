#include "DXUT.h"
#include "cMainGame.h"
#include "cCube.h"
#include "cGrid.h"
#include "cCharController.h"
#include "cCamera.h"
#include "cCharacter.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cHeightMap.h"
#include "cGroup.h"
#include "cObjLoader.h"
#include "cFrustum.h"
#include "cPyramid.h"
#include "cRay.h"

cMainGame::cMainGame()
	: m_pCube(NULL)
	, m_pGrid(NULL)
	, m_pCharController(NULL)
	, m_pCamera(NULL)
	, m_pRootFrame(NULL)
	, m_pHeightMap(NULL)
	, m_pMesh(NULL)
	, m_pFrustum(NULL)
	, m_pPyramid(NULL)
	, m_vPyramidPos(5, 0, 6)
{
	//D3DXMatrixRotationQuaternion(¾Æ¿ôÇ²¸ÅÆ®¸¯½º, ÀÎÇ² Äõ);
	//D3DXQuaternionSlerp(¾Æ¿ôÇ², Äõ1, Äõ2, f)
	//D3DXQUATERNION
	//qx = ax * sin( angle / 2 )
	//qy = ay * sin( angle / 2 )
	//qz = az * sin( angle / 2 )
	//qw = cos(angle / 2)
}

cMainGame::~cMainGame()
{
	SAFE_DELETE(m_pCube);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pCharController);
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pHeightMap);
	SAFE_DELETE(m_pFrustum);
	SAFE_DELETE(m_pPyramid);

	SAFE_RELEASE(m_pMesh);

	for each(auto p in m_vecGroup)
	{
		SAFE_DELETE(p);
	}
	
	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}

	for each(auto p in m_vecCharacter)
	{
		SAFE_RELEASE(p);
	}

	SAFE_DELETE(m_pRootFrame);
	
	assert(g_setObject.empty() && "¸Þ¸ð¸®¸¯!!");

	g_pTextureManager->Destroy();
}

void cMainGame::Setup()
{
	m_pFrustum = new cFrustum;

	//cObjLoader l;
	//l.Load("map/Map.obj", m_vecGroup);
	//m_pMesh = l.LoadMesh("map/Map.obj", m_vecMtlTex);
	
	D3DXCreateSphere(g_pD3DDevice, 0.5, 100, 100, &m_pMesh, NULL);

	for (int x = -5; x <= 5; ++x)
	{
		for (int y = -5; y <= 5; ++y)
		{
			for (int z = -5; z <= 5; ++z)
			{
				ST_SPHERE s;
				s.vCenter = D3DXVECTOR3(x * 3, y * 3, z * 3);
				s.fRadius = 0.5;
				s.isPicked = false;
				m_vecSphere.push_back(s);
			}
		}
	}
	
	ST_PC_VERTEX v;
	
	v.p = D3DXVECTOR3(-1, 1, 0); m_vecRect1.push_back(v);
	v.p = D3DXVECTOR3( 1, 1, 0); m_vecRect1.push_back(v);
	v.p = D3DXVECTOR3( 1,-1, 0); m_vecRect1.push_back(v);
	v.p = D3DXVECTOR3(-1, 1, 0); m_vecRect1.push_back(v);
	v.p = D3DXVECTOR3( 1,-1, 0); m_vecRect1.push_back(v);
	v.p = D3DXVECTOR3(-1,-1, 0); m_vecRect1.push_back(v);

	v.p = D3DXVECTOR3(-1, 1, 1); m_vecRect2.push_back(v);
	v.p = D3DXVECTOR3( 1, 1, 1); m_vecRect2.push_back(v);
	v.p = D3DXVECTOR3( 1,-1, 1); m_vecRect2.push_back(v);
	v.p = D3DXVECTOR3(-1, 1, 1); m_vecRect2.push_back(v);
	v.p = D3DXVECTOR3( 1,-1, 1); m_vecRect2.push_back(v);
	v.p = D3DXVECTOR3(-1,-1, 1); m_vecRect2.push_back(v);

	for (size_t i = 0; i < 6; ++i)
	{
		m_vecRect1[i].c = D3DCOLOR_ARGB(180, 180, 40, 40);
		m_vecRect2[i].c = D3DCOLOR_ARGB(128, 23, 120, 55);
	}

	v.c = D3DCOLOR_XRGB(50, 50, 50);
	v.p = D3DXVECTOR3(-10, 0, 10);
	m_vecPlane.push_back(v);
	v.p = D3DXVECTOR3(10, 0, 10);
	m_vecPlane.push_back(v);
	v.p = D3DXVECTOR3(10, 0, -10);
	m_vecPlane.push_back(v);
	v.p = D3DXVECTOR3(-10, 0, 10);
	m_vecPlane.push_back(v);
	v.p = D3DXVECTOR3(10, 0, -10);
	m_vecPlane.push_back(v);
	v.p = D3DXVECTOR3(-10, 0, -10);
	m_vecPlane.push_back(v);

	//128, 23, 120, 55
	//0.5 11, 60, 27
	//180, 180, 40, 40
	//0.7 126, 28, 28
	//    137, 88, 55
	m_pCube = new cCube;
	m_pCube->Setup();

	m_pGrid = new cGrid;
	m_pGrid->Setup(30);

	m_pCharController = new cCharController;

	m_pCamera = new cCamera;
	m_pCamera->Setup();

	m_pHeightMap = new cHeightMap;
	m_pHeightMap->Load("heightMap.raw", "terrain.jpg");

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	cAseLoader loader;
	m_pRootFrame = loader.Load("./ase/woman2.ase");
	SetLight();

	// Ä³¸¯ÅÍ »ý¼º
	LPD3DXMESH pMesh = NULL;
	D3DXCreateTeapot(g_pD3DDevice, &pMesh, NULL);

	cCharacter* pCharacter = new cCharacter;
	pCharacter->SetMesh(pMesh);
	pCharacter->SetPosition(D3DXVECTOR3(10, 0, 0));

	m_vecCharacter.push_back(pCharacter);
	SAFE_RELEASE(pMesh);

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = m_stMtl.Diffuse = m_stMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	ZeroMemory(&m_stPickedMtl, sizeof(D3DMATERIAL9));
	m_stPickedMtl.Ambient = m_stPickedMtl.Diffuse = m_stPickedMtl.Specular = D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f);
	
	m_pPyramid = new cPyramid();
	D3DXMATRIX matWorld, matS, matR, matT;
	D3DXMatrixScaling(&matS, 0.2f, 1.0f, 0.2f);
	D3DXMatrixRotationX(&matR, D3DXToRadian(90.0f));
	D3DXMatrixTranslation(&matT, 0, 0.5f, 0);
	matWorld = matS * matT * matR;
	m_pPyramid->Setup(D3DCOLOR_XRGB(0, 0, 255), &matWorld);
	
	D3DXMatrixIdentity(&m_matWorld);
}

void cMainGame::Update()
{
	if (m_pCharController)
		m_pCharController->Update(NULL);
	
	if (m_pCamera)
	{
		if (m_pCharController)
			m_pCamera->Update(m_pCharController->GetPosition());
		else
			m_pCamera->Update();
	}

	//if (GetKeyState(VK_SPACE) & 0x8000)
	{
		m_pFrustum->Update();
	}

	int n = GetTickCount() % (3200 - 640) + 640;
	m_pRootFrame->Update(n, NULL);

	for each(auto p in m_vecCharacter)
	{
		p->Update();
	}
}

void cMainGame::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	if (m_pPyramid)
	{
		m_pPyramid->Render(&m_matWorld);
	}
	
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecPlane[0], sizeof(ST_PC_VERTEX));
	//g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	
	//g_pD3DDevice->SetMaterial(&m_stMtl);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	for each(auto p in m_vecSphere)
	{
		if (p.isPicked)
		{
			g_pD3DDevice->SetMaterial(&m_stPickedMtl);
		}
		else
		{
			g_pD3DDevice->SetMaterial(&m_stMtl);
		}
		if (m_pFrustum->IsIn(&p))
		{
			matWorld._41 = p.vCenter.x;
			matWorld._42 = p.vCenter.y;
			matWorld._43 = p.vCenter.z;
			g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
			m_pMesh->DrawSubset(0);
		}
	}
	
	/*
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	ULONGLONG ullStart, ullFinish;
	static ULONGLONG ullGroup, ullMesh;
	ullStart = GetTickCount64();
	
	for (size_t k = 0; k < 100; ++k)
	{
		for each(auto p in m_vecGroup)
		{
			p->Render();
		}
	}
	
	ullFinish = GetTickCount64();
	ullGroup += (ullFinish - ullStart);
	ullStart = ullFinish;

	for (size_t k = 0; k < 100; ++k)
	{
		for (size_t i = 0; i < m_vecMtlTex.size(); ++i)
		{
			g_pD3DDevice->SetMaterial(&m_vecMtlTex[i]->GetMtl());
			g_pD3DDevice->SetTexture(0, m_vecMtlTex[i]->GetTexture());
			m_pMesh->DrawSubset(i);
		}
	}

	ullFinish = GetTickCount64();
	ullMesh += (ullFinish - ullStart);
	
	char szTemp[1024];
	sprintf(szTemp, "%.2f¹è", ullGroup / (float)ullMesh);
	SetWindowTextA(DXUTGetHWND(), szTemp);
	

	
	D3DXMATRIXA16 matI;
	D3DXMatrixIdentity(&matI);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matI);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	
	D3DXMATRIX matView, matInvView;
	g_pD3DDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);
	matWorld = matInvView;
	matWorld._41 = 0;
	matWorld._42 = 0;
	matWorld._43 = 0;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecRect2[0], sizeof(ST_PC_VERTEX));
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, &m_vecRect1[0], sizeof(ST_PC_VERTEX));
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	*/
	char szTemp[1024];
	sprintf(szTemp, "%.2f", DXUTGetFPS());
	SetWindowTextA(DXUTGetHWND(), szTemp);

	/*
	if (m_pCube)
	{
		if (m_pCharController)
			m_pCube->Render(m_pCharController->GetWorldTM());
		else
			m_pCube->Render(NULL);
	}

	if (m_pHeightMap) m_pHeightMap->Render(m_pCamera);

	for each(auto p in m_vecCharacter)
	{
		p->Render();
	}
	*/
}

void cMainGame::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
		m_pCamera->MsgProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_MOUSEMOVE:
	{
	}
	break;
	case WM_LBUTTONDOWN:
	{	
		cRay r = cRay::GetRayAtWorld(LOWORD(lParam), HIWORD(lParam));
		for (size_t i = 0 ; i < m_vecSphere.size(); ++i)
		{
			m_vecSphere[i].isPicked = r.IsPicked(&m_vecSphere[i]);
		}
	}
	break;
	}
}

void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9)); 
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = stLight.Diffuse = stLight.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	D3DXVECTOR3 vDirection(1, -1, 1);
	D3DXVec3Normalize(&vDirection, &vDirection);
	stLight.Direction = vDirection;

	g_pD3DDevice->SetLight(0, &stLight);

	g_pD3DDevice->LightEnable(0, true);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}

