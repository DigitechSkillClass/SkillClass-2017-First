#pragma once

#define g_pTextureManager cTextureManager::GetInstance()

class cTextureManager
{
private:
	std::map<std::string, LPDIRECT3DTEXTURE9> m_mapTexture;

private:
	SINGLETONE(cTextureManager);

public:
	LPDIRECT3DTEXTURE9 GetTexture(char* szPath);
	LPDIRECT3DTEXTURE9 GetTexture(std::string sPath);
	void Destroy();
};

