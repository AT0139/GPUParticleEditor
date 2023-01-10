#pragma once

#include "Component.h"

class Draw2DPolygon : public Component
{
public:
	Draw2DPolygon(GameObject* gameObject);
	~Draw2DPolygon();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	Vector2 GetPosition() { return m_position; }
	Vector2 GetSize() { return m_size; }

	void SetPosition(Vector2 pos);
	void SetSize(Vector2 size);
	void SetUV(Vector2 uv);
	void SetUV(Vector2 uv, Vector2 size);
	void SetAlpha(float alpha);

	void LoadTexture(std::wstring fileName);
	void LoadTexture(std::string fileName);
private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11ShaderResourceView* m_texture;

	Vector2 m_position;
	Vector2 m_size;
	Vector2 m_uv;
	Vector2 m_uvSize;
	float m_alpha;
};