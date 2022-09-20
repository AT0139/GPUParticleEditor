#pragma once

#include "GameObject.h"

#define MAX_BONES 48

struct SkinningBuffer
{
	D3DXMATRIX SkinTransform[MAX_BONES];
};

class Player :public GameObject
{
public:
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;

private:
	class AnimationModel *m_model;
	class Audio* m_shotSE;
	class Shadow* m_shadow;

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;
	
	const float MOVE_SPEED = 0.1f;

	int m_frame;
};