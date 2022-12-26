#pragma once

#include "Component.h"

enum class GeometryType
{
	Box,
	Cube,
	Sphere,
};

class DrawGeometry : public Component
{
public:
	DrawGeometry(GameObject* pGameObject);
	~DrawGeometry();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	void Create(GeometryType type, Vector3 size);
	void Create(GeometryType type, float size);

	void SetCollisionDraw(bool is) { m_isCollision = is; }
private:
	std::unique_ptr<GeometricPrimitive> m_shape;

	bool m_isCollision;
};