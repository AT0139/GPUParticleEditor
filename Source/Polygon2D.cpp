#include "Polygon2D.h"
#include "Draw2DPolygon.h"

Polygon2D::Polygon2D()
{
	m_polygon = AddComponent<Draw2DPolygon>(this);
	m_polygon->LoadTexture(L"asset/texture/field.jpg");
}

Polygon2D::~Polygon2D()
{
}

void Polygon2D::Update()
{
#ifdef _DEBUG
	{
		ImGui::Begin("General");

		ImGui::SliderFloat("posX", &m_position.x, 0.0f, 1000);
		ImGui::SliderFloat("posY", &m_position.y, 0.0f, 1000);

		ImGui::SliderFloat("alpha", &m_alpha, 0.0f, 1.0f);


		ImGui::End();
	}

	{
		ImGui::Begin("test");

		ImGui::Text("Test");

		ImGui::End();
	}
#endif

	m_polygon->SetPosition(m_position);
	m_polygon->SetAlpha(m_alpha);
}

void Polygon2D::Draw()
{
}