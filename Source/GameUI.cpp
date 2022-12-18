#include "GameUI.h"
#include "ObjectPlacementUI.h"
#include "Input.h"
#include "Manager.h"
#include "Scene.h"
#include "Wall.h"
#include "Camera.h"
#include "MeshField.h"
#include "Renderer.h"
#include "Player.h"
#include "Floor.h"

static const float ROTATION_SPEED = 0.05f;

GameUI::GameUI()
	: m_pPlaceObject(nullptr)
{
	auto scene = Manager::GetInstance().GetScene();
	//UI
	m_pPlacementUI = scene->AddGameObject<ObjectPlacementUI>(scene->UI);
	m_pPlacementUI->SetHidden(true);
}

GameUI::~GameUI()
{
}

void GameUI::Update()
{
	//UIを開く
	if (Input::GetKeyTrigger(KEY_CONFIG::OPEN_UI))
	{
		m_pPlacementUI->SetHidden(false);
	}
	if (Input::GetKeyRelease(KEY_CONFIG::OPEN_UI))
	{
		m_pPlacementUI->SetHidden(true);
	}
	//作成フラグが帰ってきていたら
	auto createType = m_pPlacementUI->IsCreate();
	if (createType != OBJECT_TYPE::NONE)
	{
		m_pPlacementUI->ResetIsCreate();
		//オブジェクトの作成
		auto scene = Manager::GetInstance().GetScene();

		switch (createType)
		{
		case OBJECT_TYPE::WALL:
			m_pPlaceObject = scene->AddGameObject<Wall>(scene->OBJECT);
			break;
		case OBJECT_TYPE::FLOOR:
			m_pPlaceObject = scene->AddGameObject<Floor>(scene->OBJECT);
			break;

		}
		m_pPlacementUI->SetHidden(true);

		auto col = m_pPlaceObject->GetComponent<CollisionComponent>();
		if (col)
		{
			col->SetIsStaticObject(false);
		}
	}
	//設置物がnullじゃない場合
	if (m_pPlaceObject)
	{
		auto mousePos = GetMousePos();
		auto scene = Manager::GetInstance().GetScene();

		//マウスポジションから座標を計算
		Vector3 pos;
		auto camera = scene->GetGameObject<MainGame::Camera>(scene->CAMERA);
		auto field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);
		auto trans = m_pPlaceObject->GetComponent<Transform>();

		Matrix view = camera->GetViewMatrix();
		Matrix proj = camera->GetProjectionMatrix();

		//マウス座標からレイを飛ばす
		Ray ray = Utility::ScreenPosToRay(mousePos.x, mousePos.y, &view, &proj);
		std::list<Triangle> triangles;
		auto playerTrans = scene->GetGameObject<MainGame::Player>(scene->OBJECT)->GetComponent<Transform>();
		field->GetTriangles(triangles, playerTrans->GetPosition());

		//メッシュフィールド上のどのポリゴンにいるか
		float dist = 0;
		Triangle colTri;
		for (auto tri : triangles)
		{
			if (ray.Intersects(tri.tri1, tri.tri2, tri.tri3, dist))
			{
				colTri = tri;
				break;
			}
		}

		if (dist >= 1.0f)
		{
			//当たりの距離から位置の計算
			pos = ray.position + ray.direction * dist;

			trans->SetPosition(pos);
		}

		//回転
		if (Input::GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_L))
			trans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, -ROTATION_SPEED));
		if (Input::GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_R))
			trans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, ROTATION_SPEED));

		//離されたら
		if (Input::GetKeyRelease(KEY_CONFIG::MOUSE_L))
		{
			auto col = m_pPlaceObject->GetComponent<CollisionComponent>();
			if (col)
			{
				col->SetIsStaticObject(true);
			}
			m_pPlaceObject = nullptr;
		}
	}

}

void GameUI::Draw()
{
}
