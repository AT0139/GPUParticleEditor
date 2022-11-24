﻿#include "main.h"
#include "renderer.h"
#include "model.h"
#include "AnimationModel.h"
#include "ResourceManager.h"

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

AnimationModel* ResourceManager::GetAnimationModelData(std::string filePath)
{
	if (m_animationModelList.find(filePath) == m_animationModelList.end())
	{
		m_animationModelList[filePath] = new AnimationModel(filePath.c_str());
	}
	return m_animationModelList[filePath];
}

ID3D11ShaderResourceView* ResourceManager::GetTextureData(std::string filePath)
{
	//キーが存在しているか
	if (m_textureList.find(filePath) == m_textureList.end())
	{
		ID3D11ShaderResourceView* texture;
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetInstance().GetDevice(), filePath.c_str(), NULL, NULL, &texture, NULL);
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