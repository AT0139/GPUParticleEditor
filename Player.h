#pragma once

#include "GameObject.h"

class Player :public GameObject
{
public:
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;

	std::string m_animationName;
private:
	class AnimationModel *m_model;
	class Audio* m_shotSE;
	class Shadow* m_shadow;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;
	
	const float MOVE_SPEED = 0.1f;

	int m_frame;
	float m_blendRate;
};