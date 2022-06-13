#include "main.h"
#include "renderer.h"
#include "model.h"
#include "ModelManager.h"

ModelManager* ModelManager::m_singleton = nullptr;


ModelManager* ModelManager::GetInstance()
{
	if (!m_singleton)
	{
		m_singleton = new ModelManager;
	}
	return m_singleton;
}

Model* ModelManager::GetModelData(std::string filePath)
{
	//キーが存在しているか
	if (m_modelList.find(filePath) == m_modelList.end())
	{
		//ない場合作成
		m_modelList[filePath] = new Model(filePath.c_str());
	}
	return m_modelList[filePath];
}


ModelManager::~ModelManager()
{
	for (const auto model : m_modelList)
	{
		model.second->Unload();
	}
	m_modelList.clear();
	
	delete m_singleton;
	m_singleton = nullptr;
}
