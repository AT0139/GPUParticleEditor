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

	void SetShader(SHADER_TYPE type);

private:
	class CModel* m_model;
	SHADER_TYPE m_shaderType;
	RASTERIZER m_rasterizerState;
	bool m_isCollision;
};