#pragma once

//モデルデータを管理するクラス

#include <map>
#include <string>

class Model;

class ResourceManager
{
public:
	static ResourceManager* GetInstance();

	Model* GetModelData(std::string filePath);
	ID3D11ShaderResourceView* GetTextureData(std::string filePath);

private:
	ResourceManager() {}
	~ResourceManager();

	static ResourceManager* m_singleton;

	std::map <std::string,Model*> m_modelList;
	std::map <std::string, ID3D11ShaderResourceView*> m_textureList;
};