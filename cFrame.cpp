#include "DXUT.h"
#include "cFrame.h"
#include "cMtlTex.h"

cFrame::cFrame()
	: m_pMtlTex(NULL)
{
	D3DXMatrixIdentity(&m_matLocalTM);
	D3DXMatrixIdentity(&m_matWorldTM);
}


cFrame::~cFrame()
{
	SAFE_RELEASE(m_pMtlTex);
	for each(auto p in m_vecChild)
	{
		SAFE_DELETE(p);
	}
}

void cFrame::Render()
{
	if (m_pMtlTex)
	{
		g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);

		LPDIRECT3DTEXTURE9 p = m_pMtlTex->GetTexture();
		g_pD3DDevice->SetTexture(0, m_pMtlTex->GetTexture());
		g_pD3DDevice->SetMaterial(&(m_pMtlTex->GetMtl()));
		g_pD3DDevice->SetFVF(ST_PNT_VERTEX::FVF);
		g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PNT_VERTEX));
	}

	for each(auto p in m_vecChild)
	{
		p->Render();
	}
}

void cFrame::AddChild(cFrame* pChild)
{
	m_vecChild.push_back(pChild);
}

void cFrame::CalcOrigLocalTM(cFrame* pParent)
{
	// W = L * PW
	// L = W * PW-1
	if (pParent)
	{
		D3DXMATRIX matInvParentWorld;
		D3DXMatrixInverse(&matInvParentWorld, 0, &(pParent->GetWorldTM()));
		m_matLocalTM = m_matWorldTM * matInvParentWorld;
	}
	else
	{
		m_matLocalTM = m_matWorldTM;
	}
	for each(auto p in m_vecChild)
	{
		p->CalcOrigLocalTM(this);
	}
}

void cFrame::Update(int nKeyFrame, D3DXMATRIX* pParentWorldTM)
{
	D3DXMATRIX matT, matR;
	LocalTranslationMatrixAtTime(nKeyFrame, matT);
	LocalRotationMatrixAtTime(nKeyFrame, matR);
	m_matLocalTM = matR * matT;

	if (pParentWorldTM)
	{
		m_matWorldTM = m_matLocalTM * *pParentWorldTM;
	}
	else
	{
		m_matWorldTM = m_matLocalTM;
	}

	for each(auto p in m_vecChild)
	{
		p->Update(nKeyFrame, &m_matWorldTM);
	}
}

void cFrame::LocalTranslationMatrixAtTime(IN int nKeyFrame, OUT D3DXMATRIX& mat)
{
	D3DXMatrixIdentity(&mat);
	if (m_vecPosTrack.empty())
	{
		mat._41 = m_matLocalTM._41;
		mat._42 = m_matLocalTM._42;
		mat._43 = m_matLocalTM._43;
	}
	else if (nKeyFrame <= m_vecPosTrack.front().n)
	{
		mat._41 = m_vecPosTrack.front().p.x;
		mat._42 = m_vecPosTrack.front().p.y;
		mat._43 = m_vecPosTrack.front().p.z;
	}
	else if (nKeyFrame >= m_vecPosTrack.back().n)
	{
		mat._41 = m_vecPosTrack.back().p.x;
		mat._42 = m_vecPosTrack.back().p.y;
		mat._43 = m_vecPosTrack.back().p.z;
	}
	else
	{
		int nNext = 0;
		for (size_t i = 0; i < m_vecPosTrack.size(); ++i)
		{
			if (nKeyFrame < m_vecPosTrack[i].n)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;
		float t = (nKeyFrame - m_vecPosTrack[nPrev].n) /
			(float)(m_vecPosTrack[nNext].n - m_vecPosTrack[nPrev].n);
		D3DXVECTOR3 p;
		D3DXVec3Lerp(&p, &m_vecPosTrack[nPrev].p, &m_vecPosTrack[nNext].p, t);
		mat._41 = p.x;
		mat._42 = p.y;
		mat._43 = p.z;
	}
}

void cFrame::LocalRotationMatrixAtTime(IN int nKeyFrame, OUT D3DXMATRIX& mat)
{
	D3DXMatrixIdentity(&mat);
	if (m_vecRotTrack.empty())
	{
		mat = m_matLocalTM;
		mat._41 = 0.0f;
		mat._42 = 0.0f;
		mat._43 = 0.0f;
	}
	else if (nKeyFrame <= m_vecRotTrack.front().n)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.front().q);
	}
	else if (nKeyFrame >= m_vecRotTrack.back().n)
	{
		D3DXMatrixRotationQuaternion(&mat, &m_vecRotTrack.back().q);
	}
	else
	{
		int nNext = 0;
		for (size_t i = 0; i < m_vecRotTrack.size(); ++i)
		{
			if (nKeyFrame < m_vecRotTrack[i].n)
			{
				nNext = i;
				break;
			}
		}
		int nPrev = nNext - 1;
		float t = (nKeyFrame - m_vecRotTrack[nPrev].n) /
			(float)(m_vecRotTrack[nNext].n - m_vecRotTrack[nPrev].n);
		
		D3DXQUATERNION q;
		D3DXQuaternionSlerp(&q, &m_vecRotTrack[nPrev].q, &m_vecRotTrack[nNext].q, t);
		D3DXMatrixRotationQuaternion(&mat, &q);
	}
}