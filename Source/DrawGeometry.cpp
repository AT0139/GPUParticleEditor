#include "DrawGeometry.h"
#include "GameObject.h"
#include "Manager.h"
#include "Scene.h"
#include "Camera.h"
#include "Renderer.h"

DrawGeometry::DrawGeometry(GameObject* pGameObject)
	: Component(pGameObject)
	, m_isCollision(false)
	, m_shape(nullptr)
{}

DrawGeometry::~DrawGeometry()
{
}

void DrawGeometry::Update()
{
}

void DrawGeometry::Draw()
{
	Matrix world;
	if (m_isCollision)
	{
		auto scale = GetGameObject()->GetComponent<CollisionComponent>()->GetCollisionScale();
		world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix(scale);
	}
	else
		world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();

	auto scene = Manager::GetInstance().GetScene();
	auto camera = scene->GetGameObject<MainGame::Camera>(scene->CAMERA);
	Matrix view = camera->GetViewMatrix();
	Matrix proj = camera->GetProjectionMatrix();

	m_shape->Draw(world, view, proj, Colors::Green, nullptr, true);
}

void DrawGeometry::Create(GeometryType type, Vector3 size)
{
	auto context = Renderer::GetInstance().GetDeviceContext();
	m_shape.reset();

	switch (type)
	{
	case GeometryType::Box:
		m_shape = GeometricPrimitive::CreateBox(context, size, false);
		break;
	case GeometryType::Cube:
		m_shape = GeometricPrimitive::CreateCube(context, size.x, false);
		break;
	case GeometryType::Sphere:
		m_shape = GeometricPrimitive::CreateSphere(context, size.x, 16Ui64, false);
		break;
	}

}

void DrawGeometry::Create(GeometryType type, float size)
{
	auto context = Renderer::GetInstance().GetDeviceContext();
	m_shape.reset(nullptr);

	switch (type)
	{
	case GeometryType::Box:
		m_shape = GeometricPrimitive::CreateBox(context, Vector3(size, size, size), false);
		break;
	case GeometryType::Cube:
		m_shape = GeometricPrimitive::CreateCube(context, size, false);
		break;
	case GeometryType::Sphere:
		m_shape = GeometricPrimitive::CreateSphere(context, size, 16Ui64, true);
		break;
	}
}
