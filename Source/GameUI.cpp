#include "GameUI.h"
#include "ObjectPlacementUI.h"
#include "Input.h"
#include "Manager.h"
#include "Scene.h"
#include "BlankObject.h"
#include "Camera.h"
#include "MeshField.h"
#include "Renderer.h"
#include "Player.h"

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
	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::OPEN_UI))
	{
		m_pPlacementUI->SetHidden(false);
	}
	if (GET_INPUT.GetKeyRelease(KEY_CONFIG::OPEN_UI))
	{
		m_pPlacementUI->SetHidden(true);
	}
	
	PlacementUIUpdate();
}

void GameUI::Draw()
{
}

void GameUI::PlacementUIUpdate()
{
	//作成フラグが帰ってきていたら
	int modelId = m_pPlacementUI->GetCreateModelID();
	if (modelId != -1)
	{
		//オブジェクトの作成
		CreateObjectAtID(modelId);
	}
	//設置物がnullじゃない場合
	if (m_pPlaceObject)
	{
		auto mousePos = GET_INPUT.GetMousePoint();
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
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_L))
			trans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, -ROTATION_SPEED));
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_R))
			trans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, ROTATION_SPEED));

		//スナップ
		for (auto snap : m_pSnapPositionList)
		{
			float snapDist = 0;
			if (snap->Intersects(ray.position, ray.direction, snapDist))
			{
#ifdef _DEBUG
				ImGui::Begin("Mouse");
				{
					ImGui::Text("Ray Intersect!");
				}
				ImGui::End();
#endif
				Vector3 hitPos = ray.position + ray.direction * snapDist;
				snap->Center - hitPos;


				break;
			}
		}

		//離されたら
		if (GET_INPUT.GetKeyRelease(KEY_CONFIG::MOUSE_L))
		{
			auto col = m_pPlaceObject->GetComponent<CollisionComponent>();
			if (col)
			{
				col->SetIsStaticObject(true);
			}

			//スナップ用情報セット
			std::shared_ptr<BoundingOrientedBox> box(new BoundingOrientedBox);
			box->Center = m_pPlaceObject->GetComponent<Transform>()->GetPosition();
			box->Extents = m_pPlaceObject->GetComponent<CollisionComponent>()->GetCollisionScale();
			box->Orientation = m_pPlaceObject->GetComponent<Transform>()->GetRotation();
			m_pSnapPositionList.push_back(box);


			m_pPlaceObject = nullptr;
		}
	}
}

void GameUI::CreateObjectAtID(int modelID)
{
	auto scene = Manager::GetInstance().GetScene();
	m_pPlaceObject = scene->AddGameObject<BlankObject>(scene->OBJECT);
	auto modelData = StaticDataTable::GetInstance().GetModelData(modelID);//モデルデータ取得

	auto model = m_pPlaceObject->AddComponent<DrawModel>(this);
	model->Load(modelData->GetPath().c_str());
	model->SetVertexShader("NormalMappingVS.cso");
	model->SetPixelShader("NormalMappingPS.cso");
	m_pPlaceObject->GetComponent<Transform>()->SetScale(modelData->GetScale());
	auto col = m_pPlaceObject->AddComponent<OBBCollision>();
	col->SetScale(modelData->GetCollisionScale());
	col->SetIsStaticObject(false);

	auto rigid = m_pPlaceObject->AddComponent<Rigidbody>();
	rigid->SetIsKinematic(true);

	//UIを消す
	m_pPlacementUI->SetHidden(true);
	m_pPlacementUI->ResetModelID();
}
