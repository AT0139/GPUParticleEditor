#pragma once

#include "GameObject.h"

struct CollisionInfo
{
	shared_ptr<CollisionComponent> collision;
	float m_minX;
	float m_maxX;
	float m_minY;
	float m_maxY;
	float m_minZ;
	float m_maxZ;

	bool operator==(const CollisionInfo& info)const
	{
		if (this == &info)
		{
			return true;
		}
		return false;
	}

};

class CollisionManager
{
public:
	// GameObject を介して継承されました
	void Update();

private:
	void Collision(int index);

	std::vector<CollisionInfo> m_collisionVector;
};