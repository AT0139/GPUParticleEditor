#pragma once

#include <vector>
#include "GameObject.h"

#define FIELD_X (255)
#define FIELD_Z (255)
#define INDEX_NUM (((FIELD_Z + 2) * 2) * FIELD_X - 2)

class MeshField : public GameObject
{
public:
	virtual void Init()override;
	virtual void Uninit()override;
	virtual void Update()override;
	virtual void Draw()override;

	float GetHeight(D3DXVECTOR3 position);

private:
	bool FileReader(const char* filename);

	ID3D11Buffer* m_vertexBuffer = NULL;
	ID3D11Buffer* m_indexBuffer = NULL;
	ID3D11ShaderResourceView* m_texture = NULL;

	VERTEX_3D m_vertex[FIELD_X + 1][FIELD_Z + 1];

	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;

	int m_terrainWidth, m_terrainHeight;
	std::vector<std::vector<float>> m_heightMap;
};