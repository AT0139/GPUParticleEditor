#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
public:
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;
	
private:
	D3DXVECTOR3 m_target;
};