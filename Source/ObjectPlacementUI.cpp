#include "ObjectPlacementUI.h"
#include "Draw2DPolygon.h"
#include "DrawModel.h"
#include "Manager.h"
#include "Scene.h"
#include "Enemy.h"
#include "Transform.h"
#include "Input.h"
#include "Camera.h"
#include "MeshField.h"

ObjectPlacementUI::ObjectPlacementUI()
	: m_isCreate(false)
{
	//todo: マスタ化
	m_backGround = AddComponent<Draw2DPolygon>(this);
	m_backGround->LoadTexture(L"asset/texture/Black.jpg");
	m_backGround->SetPosition(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	m_backGround->SetSize(Vector2(SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200));
	m_backGround->SetAlpha(0.4f);

	m_icon = AddComponent<Draw2DPolygon>(this);
	m_icon->LoadTexture(L"asset/texture/Wall.png");
	m_icon->SetPosition(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	m_icon->SetSize(Vector2(640, 360));
}

ObjectPlacementUI::~ObjectPlacementUI()
{
}

void ObjectPlacementUI::Update()
{
	auto mousePos = GetMousePos();
	if(Utility::MouseOver(mousePos,Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), Vector2(100, 100)))
	{
		if (Input::GetKeyTrigger(KEY_CONFIG::MOUSE_L))
			m_isCreate = true;
	}
}

void ObjectPlacementUI::Draw()
{
}
