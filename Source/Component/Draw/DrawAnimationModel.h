#pragma once

#include "Component.h"


class DrawAnimationModel : public Component
{
public:
	DrawAnimationModel(GameObject* pGameObject);
	~DrawAnimationModel();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;


	void Load(std::string filePath);
	void LoadAnimation(const char* fileName, const char* animarionName);

	void SetAnimationName(std::string animationName);

	//todo: シェーダーのセット
	void SetShader(SHADER_TYPE type);

	void AddBlendRate();
	void AddBlendRate(float factor);

	void ResetBlendRate() { m_blendRate = 0.0f; }

private:
	class AnimationModel* m_model;
	SHADER_TYPE m_shaderType;

	int m_frame;
	float m_blendRate;
	std::string m_animationName;
};