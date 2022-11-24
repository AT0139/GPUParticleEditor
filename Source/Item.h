#pragma once

#include"GameObject.h"

class Item :public GameObject
{
public:
	Item();
	~Item();
	virtual void Update() override;
	virtual void Draw() override;
private:
	class Model* m_model;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;
};