#pragma once

#include "Component.h"

class DrawBillboard : public Component
{
public:
	DrawBillboard(GameObject* pGameObject);
	~DrawBillboard();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	void Load(const wchar_t* filePath);
	void Load(const wchar_t* filePath, int xNum, int yNum);
private:
	ID3D11ShaderResourceView* m_texture;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;

	bool m_isAnimation;
	int m_count;
	int m_xNum, m_yNum;
	float m_xAdd, m_yAdd;
};