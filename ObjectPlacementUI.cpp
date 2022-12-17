#include "ObjectPlacementUI.h"
#include "Draw2DPolygon.h"

ObjectPlacementUI::ObjectPlacementUI()
{
	m_backGround = AddComponent<Draw2DPolygon>(this);
	m_backGround->LoadTexture("Asset\\Texture\\Black.jpg");
	m_backGround->SetPosition(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	m_backGround->SetSize(Vector2(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200));
	m_backGround->SetAlpha(0.4f);


}

ObjectPlacementUI::~ObjectPlacementUI()
{
}

void ObjectPlacementUI::Update()
{
}

void ObjectPlacementUI::Draw()
{
}
