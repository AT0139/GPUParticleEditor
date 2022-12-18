#pragma once

#include "GameObject.h"

struct testVector
{
	testVector(float x, float y, float z) :x(x), y(y), z(z) {}
	float x, y, z;
};

template<class Archive>
void serialize(Archive& archive, testVector& vector)
{
	archive(cereal::make_nvp("x", vector.x), cereal::make_nvp("y", vector.y), cereal::make_nvp("z", vector.z));
}

class Wall : public GameObject
{
public:
	Wall();
	~Wall();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	template<class Archive>
	void serialize(Archive& archive)
	{
		auto position = GetComponent<Transform>()->GetPosition();
		testVector pos(position.x, position.y, position.z);
		archive(CEREAL_NVP(pos));
	}

private:

	void Cereal();
};