#pragma once

#include "main.h"
#include "GameObject.h"

namespace MainGame
{
	class Bullet :public GameObject
	{
	public:
		Bullet();
		~Bullet();
		virtual void Update()override;
		virtual void Draw()override;

		static void Load();
		static void Unload();
	private:
		const float MOVE_SPEED = 0.3f;
		const int DESTROY_COUNT = 60;

		static class Model* m_model;

		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_vertexLayout;

		D3DXVECTOR3 m_forward;

		int m_count;
	};
}