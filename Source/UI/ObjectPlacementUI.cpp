#include "ObjectPlacementUI.h"
#include "Draw2DPolygon.h"
#include "Main.h"
#include "Input.h"

static const std::string TEXTURE_ROOT_PATH = "Asset/Texture/";
static const float BACKGROUND_WIDTH = 400;
static const float ICON_SIZE = 300.0f;
static const float ICON_Y_OFFSET = 300;
static const float ICON_X_OFFSET_HALF = ICON_Y_OFFSET * 0.5f;
static const float ICON_DIFF_FACTOR = 480;
static const float ICON_POW_FACTOR = 2.0f;
static const int SCROLL_DIAMETER = 20;
static const int UI_X_POSITION_MAX = 330;
static const float UI_INOUT_SPEED = 15.0f;

ObjectPlacementUI::ObjectPlacementUI()
	: m_selectIndex(0)
	, m_createModelID(-1)
	, m_yMovingDistance(0)
	, m_xPositionOffset(UI_X_POSITION_MAX)
	, m_isInOutMoving(false)
	, m_isIn(false)
	, m_isActive(false)
{
	//背景
	m_backGround = AddComponent<Draw2DPolygon>(this);
	m_backGround->LoadTexture(TEXTURE_ROOT_PATH + "PlaceUI.png");
	m_backGround->SetPosition(Vector2(SCREEN_WIDTH - BACKGROUND_WIDTH * 0.5f + UI_X_POSITION_MAX, SCREEN_HEIGHT * 0.5f));
	m_backGround->SetSize(Vector2(400, SCREEN_HEIGHT));

	auto pDataList = StaticDataTable::GetInstance().GetPlacementDataList();
	int count = 0;
	for (auto pData : *pDataList)
	{
		auto icon = AddComponent<Draw2DPolygon>(this);
		icon->LoadTexture(TEXTURE_ROOT_PATH + pData->GetIconPath());
		IconInfo info;
		info.modelID = pData->GetModelID();
		info.icon = icon;
		m_pIconInfos.push_back(info);
		count++;
	}
	MoveIcons(0);

	//枠
	m_flame = AddComponent<Draw2DPolygon>(this);
	m_flame->LoadTexture(TEXTURE_ROOT_PATH + "Flame.png");
	auto selectIconPos = m_pIconInfos[m_selectIndex].icon->GetPosition();
	selectIconPos.x += m_xPositionOffset;
	m_flame->SetPosition(selectIconPos);
	m_flame->SetSize(Vector2(440, 440));
}

ObjectPlacementUI::~ObjectPlacementUI()
{
}

void ObjectPlacementUI::Update()
{
	auto mousePos = GET_INPUT.GetMousePoint();
	auto mouseAcc = GET_INPUT.GetMouseAcceleration();

	//出し入れ
	if (m_isInOutMoving)
	{
		//出すかしまうか
		if (m_isIn)
		{
			m_xPositionOffset -= UI_INOUT_SPEED;
			MoveAlltoXPositionOffset();

			//完全に出た
			if (m_xPositionOffset <= 0)
			{
				m_isInOutMoving = false;
				m_isActive = true;
				m_xPositionOffset = 0;
			}
		}
		else
		{
			m_xPositionOffset += UI_INOUT_SPEED;
			MoveAlltoXPositionOffset();

			//完全にしまった
			if (m_xPositionOffset >= UI_X_POSITION_MAX)
			{
				m_isInOutMoving = false;
				m_isActive = false;
				m_xPositionOffset = UI_X_POSITION_MAX;
			}
		}
	}
	//スクロール
	if (m_isActive)
	{
		{
			int wheelScroll = GetMouseWheel();
			if (wheelScroll)
			{

				m_createModelID = -1;

				MoveIcons((float)-wheelScroll * SCROLL_DIAMETER);

				//中心のインデックス
				int centerIndex = (int)(m_yMovingDistance / ICON_X_OFFSET_HALF);
				if (centerIndex < m_pIconInfos.size())
				{
					m_selectIndex = centerIndex;
				}
				m_flame->SetPosition(m_pIconInfos[m_selectIndex].icon->GetPosition());
			}
		}
		//決定
		if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::DETERMINATION))
		{
			m_createModelID = m_pIconInfos[m_selectIndex].modelID;
		}
	}
}

void ObjectPlacementUI::Draw()
{
}

void ObjectPlacementUI::InOutUI(bool isIn)
{
	m_isInOutMoving = true;
	m_isIn = isIn;
}

void ObjectPlacementUI::MoveIcons(float value)
{
	for (int i = 0; i < m_pIconInfos.size(); i++)
	{
		m_yMovingDistance += value;
		if (m_yMovingDistance < 0)
			m_yMovingDistance = 0;
		if (m_yMovingDistance > (m_pIconInfos.size() - 1) * ICON_Y_OFFSET)
			m_yMovingDistance = (m_pIconInfos.size() - 1) * ICON_Y_OFFSET;

		float posX = i * ICON_Y_OFFSET + SCREEN_HEIGHT_HALF;
		float iconYpos = posX - m_yMovingDistance;

		m_pIconInfos[i].icon->SetPosition(Vector2(SCREEN_WIDTH - ICON_SIZE * 0.5f + m_xPositionOffset, iconYpos));

		float diff = std::abs(SCREEN_HEIGHT_HALF - iconYpos);
		diff /= SCREEN_HEIGHT_HALF;
		diff = std::pow(diff, ICON_POW_FACTOR);
		diff *= ICON_DIFF_FACTOR;

		//最大値
		if (diff >= ICON_SIZE)
			diff = ICON_SIZE;

		m_pIconInfos[i].icon->SetSize(Vector2(ICON_SIZE - diff, ICON_SIZE - diff));
	}
}

void ObjectPlacementUI::MoveAlltoXPositionOffset()
{
	m_backGround->SetPosition(Vector2(SCREEN_WIDTH - BACKGROUND_WIDTH * 0.5f + m_xPositionOffset, SCREEN_HEIGHT * 0.5f));

	for (int i = 0; i < m_pIconInfos.size(); i++)
	{
		m_pIconInfos[i].icon->SetPosition(Vector2(SCREEN_WIDTH - ICON_SIZE * 0.5f + m_xPositionOffset, m_pIconInfos[i].icon->GetPosition().y));
	}

	m_flame->SetPosition(m_pIconInfos[m_selectIndex].icon->GetPosition());
}
