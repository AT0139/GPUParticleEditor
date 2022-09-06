#pragma once

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib,"assimp.lib")

class AnimationModel
{
private:
	const aiScene* m_aiScene = NULL;
	ID3D11Buffer** m_vertexBuffer;
	ID3D11Buffer** m_indexBuffer;

public:
	void Load(const char* FileName);
	void Unload();
	void Update();
	void Draw();
};