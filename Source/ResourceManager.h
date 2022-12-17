#pragma once

#include "Singleton.h"
#include <unordered_map>
#include <string>

class ResourceManager : public Singleton<ResourceManager>
{
public:
	friend class Singleton<ResourceManager>;

	class Model* GetModelData(std::string filePath);
	class AnimationModel* GetAnimationModelData(std::string filePath);
	ID3D11ShaderResourceView* GetTextureData(std::wstring filePath);

	void Release();

private:
	ResourceManager() {}
	~ResourceManager();

	std::unordered_map <std::string, Model*> m_modelList;
	std::unordered_map <std::string, AnimationModel*>m_animationModelList;
	std::unordered_map <std::wstring, ID3D11ShaderResourceView*> m_textureList;
};