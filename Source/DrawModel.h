#pragma once

#include "Component.h"

class DrawModel : public Component
{
public:
	DrawModel(GameObject* pGameObject);
	~DrawModel();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	void Load(const char* filePath);
private:
	class Model* m_model;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;
};