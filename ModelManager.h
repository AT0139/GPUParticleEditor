#pragma once

//モデルデータを管理するクラス

#include <map>
#include <string>

class Model;

class ModelManager
{
public:
	static ModelManager* GetInstance();

	Model* GetModelData(std::string key);

private:
	ModelManager() {}
	~ModelManager();

	static ModelManager* m_singleton;

	std::map <std::string,Model*> m_modelList;
};