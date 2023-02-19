#pragma once

#include "GameObject.h"

class Field : public GameObject
{
public:
	Field();
	~Field();
	virtual void Update()override;
	virtual void Draw()override;

private:

	ID3D11Buffer* m_vertexBuffer = NULL;
	ID3D11ShaderResourceView* m_texture = NULL;
};