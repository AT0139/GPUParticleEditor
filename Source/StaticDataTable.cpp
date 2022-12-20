#include "StaticDataTable.h"


namespace
{
	std::vector<std::string> CsvLineSprit(std::string& input)
	{
		std::stringstream stream(input);
		std::string field;
		std::vector<std::string>result;
		while (std::getline(stream, field, ','))
		{
			result.push_back(field);
		}

		return result;
	}

	template<class T>
	void LoadTable(std::list<std::shared_ptr<T>>& list, std::string filePath)
	{
		//ファイルパス読み込み
		std::ifstream ifs(filePath);

		std::string line;
		std::vector<std::string> strVec;
		bool continueHeader = false;
		//1行ごとにデータに格納
		while (std::getline(ifs, line))
		{
			if (!continueHeader)
			{
				continueHeader = true;
				continue;
			}
			strVec = CsvLineSprit(line);
			std::shared_ptr<T> ptr(new T);
			ptr->Load(strVec);

			
			list.push_back(ptr);
		}
	}

	template<typename T>
	std::shared_ptr<T> SearchTable(std::list<std::shared_ptr<T>> list, int id)
	{
		for (auto it : list)
		{
			if (it->CheckID(id))
			{
				return it;
			}
		}
	}

}

StaticDataTable::StaticDataTable()
{
	LoadTable(m_modelDataTable, "Asset\\Table\\ModelTable.csv");
	LoadTable(m_placementDataTable, "Asset\\Table\\PlacementUITable.csv");
}


StaticDataTable::~StaticDataTable()
{
}

std::shared_ptr<ModelData> StaticDataTable::GetModelData(int id)
{
	return std::shared_ptr<ModelData>();
}

void ModelData::Load(std::vector<std::string> line)
{
	m_id = std::atoi(line[0].c_str());
	m_path = line[1];
	Vector3 temp(std::stof(line[2].c_str()), std::stof(line[3].c_str()), std::stof(line[4].c_str()));
	m_scale = Vector3(temp);
	m_collisionScale = Vector3(std::stof(line[5].c_str()), std::stof(line[6].c_str()), std::stof(line[7].c_str()));
}

void PlacementData::Load(std::vector<std::string> line)
{
	m_modelId = std::atoi(line[0].c_str());
	m_iconPath = line[1];
}

bool ModelData::CheckID(int id)
{
	if (m_id = id)
		return true;
	return false;
}

bool PlacementData::CheckID(int id)
{
	return false;
}
