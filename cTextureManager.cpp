#include "DXUT.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(char* szPath)
{
	return GetTexture(std::string(szPath));
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(std::string sPath)
{
	if (m_mapTexture.find(sPath) == m_mapTexture.end())
	{
		D3DXCreateTextureFromFileA(g_pD3DDevice, sPath.c_str(), &m_mapTexture[sPath]);
	}
	return m_mapTexture[sPath];

}

void cTextureManager::Destroy()
{
	for each (auto it in m_mapTexture)
	{
		SAFE_RELEASE(it.second); 
	}
}