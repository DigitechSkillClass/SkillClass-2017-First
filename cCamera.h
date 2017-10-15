#pragma once
class cCamera
{
private:
	D3DXVECTOR3 m_vEye;
	D3DXVECTOR3 m_vAt;
	D3DXVECTOR3 m_vUp;
	bool		m_isLButtonDown;
	POINT		m_ptPrevMouse;
	float		m_fRotX;
	float		m_fRotY;
	float		m_fDistance;
public:
	cCamera();
	virtual ~cCamera();

	void Setup();
	void Update(D3DXVECTOR3* pTarget = NULL);
	void MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	D3DXVECTOR3 GetEyePosition();
};

