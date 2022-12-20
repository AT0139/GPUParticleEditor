#pragma once

#include "GameObject.h"

class Draw2DPolygon;

class Polygon2D : public GameObject
{
public:
	Polygon2D();
	~Polygon2D();
	virtual void Update()override;
	virtual void Draw()override;

private:
	std::shared_ptr<Draw2DPolygon> m_polygon;

	Vector2 m_position;
	float m_alpha;
};