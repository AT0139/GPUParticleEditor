#pragma once


#include "GameObject.h"

enum class SerializeObjectType
{
	WALL,
	FLOOR,
};

struct SerializeInfo
{
	SerializeData data;
	SerializeObjectType type;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(type),
		CEREAL_NVP(data.positionX), CEREAL_NVP(data.positionY), CEREAL_NVP(data.positionZ),
		CEREAL_NVP(data.rotateX), CEREAL_NVP(data.rotateY), CEREAL_NVP(data.rotateZ), CEREAL_NVP(data.rotateW));
	}
};


class SerializeManager
{
public:
	SerializeManager();
	~SerializeManager();

	void ToSerialize();
	void ToDeserialize();
private:
	template<class Archive>
	void serialize(Archive& archive, std::list<SerializeInfo>& objects)
	{
		archive(CEREAL_NVP(objects));
	}
};