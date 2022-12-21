#include "ObjectPlacementUI.h"
#include "Draw2DPolygon.h"
#include "Main.h"
#include "Input.h"
#include "MeshField.h"

static const std::string textureRootPath = "Asset/Texture/";
static const Vector2 iconSize(640, 360);
static const float iconXOffset = 400;

ObjectPlacementUI::ObjectPlacementUI()
	: m_createObject(OBJECT_TYPE::NONE)
{
	//背景
	m_backGround = AddComponent<Draw2DPolygon>(this);
	m_backGround->LoadTexture(textureRootPath + "Black.jpg");
	m_backGround->SetPosition(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	m_backGround->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT - 200));
	m_backGround->SetAlpha(0.4f);

	//todo: マスタ化
	auto pDataList = StaticDataTable::GetInstance().GetPlacementDataList();
	int count = 0;
	for (auto pData : *pDataList)
	{
		auto icon = AddComponent<Draw2DPolygon>(this);
		icon->LoadTexture(textureRootPath + pData->GetIconPath());
		icon->SetPosition(Vector2((SCREEN_WIDTH * 0.5f) + (count * iconXOffset), SCREEN_HEIGHT * 0.5f));
		icon->SetSize(iconSize);
		m_pIcons.push_back(icon);
		count++;
	}	
}

ObjectPlacementUI::~ObjectPlacementUI()
{
}

void ObjectPlacementUI::Update()
{
	auto mousePos = GET_INPUT.GetMousePoint();


	if (GET_INPUT.GetKeyPress(KEY_CONFIG::MOUSE_L))
	{
		auto mouseAcc = GET_INPUT.GetMouseAcceleration();

		//横方向
		if (std::abs(mouseAcc.x) > std::abs(mouseAcc.y))
		{
			for (auto icon : m_pIcons)
			{
				Vector2 pos = icon->GetPosition();
				icon->SetPosition(Vector2(pos.x - mouseAcc.x, pos.y));
			}
		}
		else
		{
			//アイコンにマウスオーバー
			if (Utility::MouseOver(mousePos, Vector2(SCREEN_WIDTH * 0.5f - 200, SCREEN_HEIGHT * 0.5f), Vector2(400, 360)))
			{
				if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::MOUSE_L))
					m_createObject = OBJECT_TYPE::WALL;
			}
		}
	}





	//if (Utility::MouseOver(mousePos, Vector2(SCREEN_WIDTH * 0.5f + 200, SCREEN_HEIGHT * 0.5f), Vector2(400, 360)))
	//{
	//	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::MOUSE_L))
	//		m_createObject = OBJECT_TYPE::FLOOR;
	//}
}

void ObjectPlacementUI::Draw()
{
}
