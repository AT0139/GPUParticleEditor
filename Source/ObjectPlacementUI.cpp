#include "ObjectPlacementUI.h"
#include "Draw2DPolygon.h"
#include "Main.h"
#include "Input.h"
#include "MeshField.h"

static const std::string TEXTURE_ROOT_PATH = "Asset/Texture/";
static const Vector2 ICON_SIZE(540, 260);
static const Vector2 SELECT_ICON_SIZE(840, 560);
static const float ICON_X_OFFSET = 500;

ObjectPlacementUI::ObjectPlacementUI()
	: m_selectIndex(0)
	, m_createModelID(-1)
	, m_xMovingDistance(0)
{
	//背景
	m_backGround = AddComponent<Draw2DPolygon>(this);
	m_backGround->LoadTexture(TEXTURE_ROOT_PATH + "Black.jpg");
	m_backGround->SetPosition(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	m_backGround->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT - 200));
	m_backGround->SetAlpha(0.4f);

	//todo: マスタ化
	auto pDataList = StaticDataTable::GetInstance().GetPlacementDataList();
	int count = 0;
	for (auto pData : *pDataList)
	{
		auto icon = AddComponent<Draw2DPolygon>(this);
		icon->LoadTexture(TEXTURE_ROOT_PATH + pData->GetIconPath());
		icon->SetPosition(Vector2((SCREEN_WIDTH * 0.5f) + (count * ICON_X_OFFSET), SCREEN_HEIGHT * 0.5f));
		icon->SetSize(ICON_SIZE);

		IconInfo info;
		info.modelID = pData->GetModelID();
		info.icon = icon;
		m_pIconInfos.push_back(info);
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
			m_createModelID = -1;

			MoveIcons(mouseAcc.x);

			//m_pIconInfos[i].icon->SetSize(ICON_SIZE);
			//中心を大きく

	/*		if()
				m_pIconInfos[i].icon->SetSize(SELECT_ICON_SIZE);
				if (m_selectIndex != i)
				{

				}
				m_selectIndex = i;*/
			
		}

	}
	else if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::MOUSE_L))
	{
		if(Utility::MouseOver(mousePos, Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), SELECT_ICON_SIZE))
		//アイコンにマウスオーバー
		m_createModelID = m_pIconInfos[m_selectIndex].modelID;
	}
}

void ObjectPlacementUI::Draw()
{
}

void ObjectPlacementUI::MoveIcons(float xValue)
{
	for (int i = 0; i < m_pIconInfos.size(); i++)
	{
		m_xMovingDistance += xValue;
		float posX = i * ICON_X_OFFSET + SCREEN_WIDTH_HALF;
		float iconXpos = posX - m_xMovingDistance;

		m_pIconInfos[i].icon->SetPosition(Vector2(iconXpos, SCREEN_HEIGHT_HALF));
	}
}
