#pragma once

// マテリアル構造体
struct MODEL_MATERIAL
{
	char name[256];
	MATERIAL material;
	char textureName[256];
	ID3D11ShaderResourceView* pTexture;
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int startIndex;
	unsigned int indexNum;
	MODEL_MATERIAL material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D* vertexArray;
	unsigned int vertexNum;

	unsigned int* indexArray;
	unsigned int indexNum;

	SUBSET* subsetArray;
	unsigned int subsetNum;
};

class Model
{
public:

	Model();
	Model(const char* fileName);

	void Draw();

	void Load(const char* fileName);
	void Unload();

private:

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	SUBSET* m_pSubsetArray;
	unsigned int m_subsetNum;

	void LoadObj(const char* fileName, MODEL* model);
	void LoadMaterial(const char* fileName, MODEL_MATERIAL** materialArray, unsigned int* materialNum);
};