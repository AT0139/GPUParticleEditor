﻿#pragma once

#include <vector>
#include "GameObject.h"

#define FIELD_X (255)
#define FIELD_Z (255)
#define INDEX_NUM (((FIELD_Z + 2) * 2) * FIELD_X - 2)

struct Triangle
{
	Triangle() {}
	Triangle(Vector3 t1, Vector3 t2, Vector3 t3)
		:tri1(t1), tri2(t2), tri3(t3)
	{}

	Vector3 tri1, tri2, tri3;
};

struct AdjacentNode;
struct Node
{
	Vector3 pos;
	std::list<AdjacentNode> adjacentNode;
	bool notTraffic;
};

struct AdjacentNode
{
	Node* node;
	float cost;
};

namespace MainGame
{
	class MeshField : public GameObject
	{
	public:
		MeshField();
		~MeshField();
		virtual void Update()override;
		virtual void Draw()override;

		float GetHeight(Vector3 position);
		void GetTriangles(std::list<Triangle>& ret, Vector3 pos);
		void SetNotTraffic(Vector3 pos);

		Node* GetNodes(Vector3 pos);

	private:
		bool FileReader(const char* filename);

		ID3D11Buffer* m_vertexBuffer = NULL;
		ID3D11Buffer* m_indexBuffer = NULL;
		ID3D11ShaderResourceView* m_texture = NULL;
		ID3D11ShaderResourceView* m_normalTexture = NULL;

		VERTEX_3D m_vertex[FIELD_X + 1][FIELD_Z + 1];

		int m_terrainWidth, m_terrainHeight;
		std::vector<std::vector<float>> m_heightMap;
		std::vector<std::vector<Node>> m_nodes;
	};
}