#pragma once

#include "Singleton.h"

struct ModelData
{
	int id;
	std::string path;
	Vector3 scale;
	Vector3 collisionScale;
};

struct PlacementData
{
	int modelId;
	std::string iconPath;
};

class StaticDataTable : public Singleton<StaticDataTable>
{
public:
	friend class Singleton<StaticDataTable>;

	StaticDataTable();
	~StaticDataTable();

private:
};