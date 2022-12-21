#include "ObjectPlacementUI.h"
#include "Draw2DPolygon.h"
#include "Main.h"
#include "Input.h"
#include "MeshField.h"

static const std::string TEXTURE_ROOT_PATH = "Asset/Texture/";
static const Vector2 ICON_SIZE(640, 360);
static const float ICON_X_OFFSET = 500;
static const float ICON_X_OFFSET_HALF = ICON_X_OFFSET * 0.5f;
static const float ICON_SIZE_MIN = 200;
static const float ICON_DIFF_FACTOR = 480;
static const float ICON_POW_FACTOR = 2.0f;

ObjectPlacementUI::ObjectPlacementUI()
	: m_selectIndex(0)
	, m_createModelID(-1)
	, m_xMovingDistance(0)
{
	//背景
	m_backGround = AddComponent<Draw2DPolygon>(this);
	m_backGround->LoadTexture(TEXTURE_ROOT_PATH + "Black.jpg");
	m_backGround->SetPosition(Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	m_backGround->SetSize(Vector2(SCREEN_WIDTH - 180, SCREEN_HEIGHT - 200));
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
	MoveIcons(0);
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

			//中心のインデックス
			int centerIndex = (int)(m_xMovingDistance / ICON_X_OFFSET_HALF);
			if (centerIndex < m_pIconInfos.size())
			{
				//変わった時に位置補正
				if (centerIndex != m_selectIndex)
				{

				}

				m_selectIndex = centerIndex;
			}
		}
	}
	else if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::MOUSE_L))
	{
		if(Utility::MouseOver(mousePos, Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f), ICON_SIZE))
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

		float diff = std::abs(SCREEN_WIDTH_HALF - iconXpos);
		diff /= SCREEN_WIDTH_HALF;
		diff = std::pow(diff, ICON_POW_FACTOR);
		diff *= ICON_DIFF_FACTOR;

		float diffX = diff * 1.6f;
		float diffY = diff * 0.9f;

		//最大値
		if (diffX >= ICON_SIZE.x)
			diffX = ICON_SIZE.x;

		if (diffY >= ICON_SIZE.y)
			diffY = ICON_SIZE.y;

		m_pIconInfos[i].icon->SetSize(Vector2(ICON_SIZE.x - diffX, ICON_SIZE.y - diffY));
	}
}
