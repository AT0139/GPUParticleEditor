﻿#pragma once

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

	std::string GetPath() { return m_path; }
	Vector3 GetScale() { return m_scale; }

private:
	int m_id;
	std::string m_path;
	Vector3 m_scale;
};

class StaticObjectData : public DataTableBase
{
public:
	// DataTableBase を介して継承されました
	virtual void Load(std::vector<std::string> line) override;
	virtual bool CheckID(int id) override;

	int GetModelID() { return m_modelID; }
	Vector3 GetCollisionScale() { return m_collisionScale; }
	bool IsSnapX() { return m_isSnapX; }
	bool IsSnapY() { return m_isSnapY; }
	bool IsSnapZ() { return m_isSnapZ; }

private:
	int m_id;
	int m_modelID;
	Vector3 m_collisionScale;
	bool m_isSnapX, m_isSnapY, m_isSnapZ;
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
	std::shared_ptr<StaticObjectData> GetStaticObjectData(int id);

	const std::list<std::shared_ptr<StaticObjectData>>* GetModelDataList() { return &m_pStaticObjectDataTable; }
	const std::list<std::shared_ptr<PlacementData>>* GetPlacementDataList() { return &m_pPlacementDataTable; }

private:
	std::list<std::shared_ptr<ModelData>> m_pModelDataTable;
	std::list<std::shared_ptr<StaticObjectData>> m_pStaticObjectDataTable;
	std::list<std::shared_ptr<PlacementData>> m_pPlacementDataTable;
};