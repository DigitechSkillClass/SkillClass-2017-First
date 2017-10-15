#pragma once

class cMtlTex;
class cGroup;

class cObjLoader
{
protected:
	std::map<std::string, cMtlTex*> m_mapMtlTex;

public:
	cObjLoader();
	virtual ~cObjLoader();

	void Load(IN char* szPath, OUT std::vector<cGroup*>& vecGroup);
	LPD3DXMESH LoadMesh(IN char* szPath, OUT std::vector<cMtlTex*>& vecMtlTex);
	void LoadMtlLib(char* szPath);
};

