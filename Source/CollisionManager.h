﻿#pragma once

#include "GameObject.h"

struct CollisionInfo
{
	shared_ptr<CollisionComponent> collision;

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