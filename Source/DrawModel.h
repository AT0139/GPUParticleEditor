#pragma once

#include "Component.h"

enum class RASTERIZER;

class DrawModel : public Component
{
public:
	DrawModel(GameObject* pGameObject);
	~DrawModel();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	void SetRasterizerState(RASTERIZER state) { m_rasterizerState = state; }

	void Load(const char* filePath);
	void SetCollisionDraw(bool is) { m_isCollision = is; }
private:
	class Model* m_model;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;
	RASTERIZER m_rasterizerState;
	bool m_isCollision;
};