#pragma once

//モデルデータを管理するクラス

#include <unordered_map>
#include <string>

class ResourceManager
{
public:
	static ResourceManager* GetInstance();

	class Model* GetModelData(std::string filePath);
	class AnimationModel* GetAnimationModelData(std::string filePath);
	ID3D11ShaderResourceView* GetTextureData(std::string filePath);

	void Release();

private:
	ResourceManager() {}
	~ResourceManager();

	static ResourceManager* m_singleton;

	std::unordered_map <std::string, Model*> m_modelList;
	std::unordered_map <std::string, AnimationModel*>m_animationModelList;
	std::unordered_map <std::string, ID3D11ShaderResourceView*> m_textureList;
};