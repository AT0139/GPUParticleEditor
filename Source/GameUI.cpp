#include "GameUI.h"
#include "ObjectPlacementUI.h"
#include "Input.h"
#include "Manager.h"
#include "Scene.h"
#include "Enemy.h"
#include "Camera.h"
#include "MeshField.h"
#include "Renderer.h"
#include "Player.h"

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
	if (m_pPlacementUI->IsCreate())
	{
		m_pPlacementUI->ResetIsCreate();
		//オブジェクトの作成
		auto scene = Manager::GetInstance().GetScene();
		m_pPlaceObject = scene->AddGameObject<Enemy>(scene->OBJECT);
		m_pPlacementUI->SetHidden(true);
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

		Matrix view = camera->GetViewMatrix();
		Matrix proj = camera->GetProjectionMatrix();

		Ray ray = Utility::ScreenPosToRay(mousePos.x, mousePos.y, &view, &proj);
		std::list<Triangle> triangles;
		auto playerTrans = scene->GetGameObject<MainGame::Player>(scene->OBJECT)->GetComponent<Transform>();
		field->GetTriangles(triangles, playerTrans->GetPosition());

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

		pos = ray.position + ray.direction * dist;

		auto trans = m_pPlaceObject->GetComponent<Transform>();
		trans->SetPosition(pos);


		//離されたら
		if (Input::GetKeyRelease(KEY_CONFIG::MOUSE_L))
			m_pPlaceObject = nullptr;
	}

}

void GameUI::Draw()
{
}
