﻿#pragma once

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

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;
};