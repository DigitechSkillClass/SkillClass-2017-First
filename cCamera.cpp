#include "DXUT.h"
#include "cCamera.h"


cCamera::cCamera()
	: m_vEye(0, 0, -5)
	, m_vAt(0, 0, 0)
	, m_vUp(0, 1, 0)
	, m_isLButtonDown(false)
	, m_fRotX(0.0f)
	, m_fRotY(0.0f)
	, m_fDistance(5.0)
{
}


cCamera::~cCamera()
{
}

void cCamera::Setup()
{
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp); // 뷰 매트릭스 생성 함수
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView); // 디바이스에게 뷰매트릭스 전달

	RECT rc;
	GetClientRect(DXUTGetHWND(), &rc);
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, rc.right / (float)rc.bottom, 1, 1000);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::Update(D3DXVECTOR3* pTarget)
{
	m_vEye = D3DXVECTOR3(0, 0, -m_fDistance);
	D3DXMATRIX matRX, matRY, matR;
	D3DXMatrixRotationX(&matRX, m_fRotX);
	D3DXMatrixRotationY(&matRY, m_fRotY);
	matR = matRX * matRY;
	D3DXVec3TransformCoord(&m_vEye, &m_vEye, &matR);

	m_vAt = D3DXVECTOR3(0, 0, 0);

	if (pTarget)
	{
		m_vEye += *pTarget;
		m_vAt += *pTarget;
	}
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vAt, &m_vUp); // 뷰 매트릭스 생성 함수
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView); // 디바이스에게 뷰매트릭스 전달
}

void cCamera::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	{
		m_isLButtonDown = true;
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
	}
	break;
	case WM_LBUTTONUP:
	{
		m_isLButtonDown = false;
	}
	break;
	case WM_MOUSEMOVE:
	{
		if (m_isLButtonDown)
		{
			POINT ptCurrMouse;
			ptCurrMouse.x = LOWORD(lParam);
			ptCurrMouse.y = HIWORD(lParam);

			POINT ptDeltaMouse;
			ptDeltaMouse.x = ptCurrMouse.x - m_ptPrevMouse.x;
			ptDeltaMouse.y = ptCurrMouse.y - m_ptPrevMouse.y;

			m_fRotX += (ptDeltaMouse.y / 100.0f);
			m_fRotY += (ptDeltaMouse.x / 100.0f);

			if (m_fRotX >= D3DX_PI / 2.0f - 0.00001f)
			{
				m_fRotX = D3DX_PI / 2.0f - 0.00001f;
			}

			if (m_fRotX <= -D3DX_PI / 2.0f + 0.00001f)
			{
				m_fRotX = -D3DX_PI / 2.0f + 0.00001f;
			}

			m_ptPrevMouse = ptCurrMouse;
		}
	}
	break;
	case WM_MOUSEWHEEL:
	{
		m_fDistance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f);
		if (m_fDistance < 0)
			m_fDistance = 0;
	}
	break;
	default:
		break;
	}
}

D3DXVECTOR3 cCamera::GetEyePosition()
{
	return m_vEye;
}
