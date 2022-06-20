#include "main.h"
#include "renderer.h"
#include "model.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::m_singleton = nullptr;


ResourceManager* ResourceManager::GetInstance()
{
	if (!m_singleton)
	{
		m_singleton = new ResourceManager;
	}
	return m_singleton;
}


Model* ResourceManager::GetModelData(std::string filePath)
{
	//キーが存在しているか
	if (m_modelList.find(filePath) == m_modelList.end())
	{
		//ない場合作成
		m_modelList[filePath] = new Model(filePath.c_str());
	}
	return m_modelList[filePath];
}


ID3D11ShaderResourceView* ResourceManager::GetTextureData(std::string filePath)
{
	//キーが存在しているか
	if (m_textureList.find(filePath) == m_textureList.end())
	{
		ID3D11ShaderResourceView* texture;
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetInstance()->GetDevice(),filePath.c_str(), NULL, NULL, &texture, NULL);
		m_textureList[filePath] = texture;
	}

	return m_textureList[filePath];
}

void ResourceManager::Release()
{
	for (const auto model : m_modelList)
	{
		model.second->Unload();
	}
	m_modelList.clear();
	for (const auto texture : m_textureList)
	{
		texture.second->Release();
	}
	m_textureList.clear();
}


ResourceManager::~ResourceManager()
{
	for (const auto model : m_modelList)
	{
		model.second->Unload();
	}
	m_modelList.clear();
	for (const auto texture : m_textureList)
	{
		texture.second->Release();
	}
	m_textureList.clear();
	
	delete m_singleton;
	m_singleton = nullptr;
}
