#pragma once

#include "Singleton.h"

class DataTableBase
{
public:
	DataTableBase() {}
	virtual ~DataTableBase() {}

	virtual void Load(std::vector<std::string> line) = 0;
	virtual bool CheckID(int id) = 0;
};

class ModelData : public DataTableBase
{
public:
	// DataTableBase を介して継承されました
	virtual void Load(std::vector<std::string> line) override;
	virtual bool CheckID(int id) override;

private:

	int m_id;
	std::string m_path;
	Vector3 m_scale;
	Vector3 m_collisionScale;

};

class PlacementData : public DataTableBase
{
public:
	// DataTableBase を介して継承されました
	virtual void Load(std::vector<std::string> line) override;
	virtual bool CheckID(int id) override;

	const int GetModelID() { return m_modelId; }
	const std::string GetIconPath() { return m_iconPath; }
private:

	int m_modelId;
	std::string m_iconPath;
};

class StaticDataTable : public Singleton<StaticDataTable>
{
public:
	friend class Singleton<StaticDataTable>;

	StaticDataTable();
	~StaticDataTable();

	std::shared_ptr<ModelData> GetModelData(int id);

	const std::list<std::shared_ptr<ModelData>>* GetModelDataList() { return &m_modelDataTable; }
	const std::list<std::shared_ptr<PlacementData>>* GetPlacementDataList() { return &m_placementDataTable; }
private:
	std::list<std::shared_ptr<ModelData>> m_modelDataTable;
	std::list<std::shared_ptr<PlacementData>> m_placementDataTable;

};