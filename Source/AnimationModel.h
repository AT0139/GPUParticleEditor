#pragma once


#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D position;
	aiVector3D normal;
	int BoneNum;
	std::string BoneName[4];	//本来はボーンインデックスで管理すべき
	float BoneWeight[4];
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 TransMatrix;
	aiMatrix4x4 OffsetMatrix;
};

class AnimationModel
{
public:
	AnimationModel() {}
	AnimationModel(const char* fileName);

	void Load(const char* FileName);
	void LoadAnimation(const char* fileName, const char* animarionName);
	void Unload();
	void Update(const char* animationName, float blendRate, int frame);
	void Draw();

private:
	const aiScene* m_aiScene = NULL;
	ID3D11Buffer** m_vertexBuffer;
	ID3D11Buffer** m_indexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_texture;
	std::unordered_map<std::string, const aiScene*>m_animation;

	std::vector<DEFORM_VERTEX>* m_deformVertex;	//変形後頂点データ
	std::unordered_map<std::string, BONE>m_bone;	//ボーンデータ(名前で参照)

	std::string m_currentAnimationName;

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
};