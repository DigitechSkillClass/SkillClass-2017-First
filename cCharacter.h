#pragma once
class cCharacter : public cGameObject
{
protected:
	D3DXVECTOR3 m_vPrevPosition;
	D3DXVECTOR3 m_vDirection;

public:
	cCharacter();
	virtual ~cCharacter();

	virtual void Update() override;
	virtual void Render() override;
};

