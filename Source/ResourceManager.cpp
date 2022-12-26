#include "Renderer.h"
#include "CModel.h"
#include "AnimationModel.h"
#include "ResourceManager.h"

CModel* ResourceManager::GetModelData(std::string filePath)
{
	//キーが存在しているか
	if (m_modelList.find(filePath) == m_modelList.end())
	{
		//ない場合作成
		m_modelList[filePath] = new CModel(filePath.c_str());
	}
	return m_modelList[filePath];
}

AnimationModel* ResourceManager::GetAnimationModelData(std::string filePath)
{
	if (m_animationModelList.find(filePath) == m_animationModelList.end())
	{
		m_animationModelList[filePath] = new AnimationModel(filePath.c_str());
	}
	return m_animationModelList[filePath];
}

ID3D11ShaderResourceView* ResourceManager::GetTextureData(std::wstring filePath)
{
	//キーが存在しているか
	if (m_textureList.find(filePath) == m_textureList.end())
	{
		ID3D11ShaderResourceView* texture;

		//外部ファイルから読み込み
		HRESULT hr = CreateWICTextureFromFile(Renderer::GetInstance().GetDevice(), filePath.c_str(), nullptr, &texture);

		assert(SUCCEEDED(hr));

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
	for (const auto model : m_animationModelList)
	{
		model.second->Unload();
	}
	m_animationModelList.clear();
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
	for (const auto model : m_animationModelList)
	{
		model.second->Unload();
	}
	m_animationModelList.clear();
}