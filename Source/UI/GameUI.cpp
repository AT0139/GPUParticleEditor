#include "GameUI.h"
#include "ObjectPlacementUI.h"
#include "Input.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MainCamera.h"
#include "MeshField.h"
#include "Renderer.h"
#include "Player.h"
#include "Wall.h"
#include "Foundation.h"

GameUI::GameUI()
	: m_pPlaceObject(nullptr)
{
	auto scene = SceneManager::GetInstance().GetScene();
	//UI
	m_pPlacementUI = scene->AddGameObject<ObjectPlacementUI>(scene->UI);
}

GameUI::~GameUI()
{
}

void GameUI::Update()
{
	//UIを開く
	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::OPEN_UI))
	{
		m_pPlacementUI->InOutUI(true);
	}
	if (GET_INPUT.GetKeyRelease(KEY_CONFIG::OPEN_UI))
	{
		m_pPlacementUI->InOutUI(false);
	}
	
	PlacementUIUpdate();
}

void GameUI::Draw()
{
}

void GameUI::PlacementUIUpdate()
{
	//作成フラグが帰ってきていたら
	int staticObjectId = m_pPlacementUI->GetCreateStaticObjectID();
	if (staticObjectId != -1)
	{
		//オブジェクトの作成
		CreateObjectAtID(staticObjectId);
	}
	//設置物がnullじゃない場合
	if (m_pPlaceObject)
	{
		auto mousePos = GET_INPUT.GetMousePoint();
		auto scene = SceneManager::GetInstance().GetScene();

		//マウスポジションから座標を計算
		auto camera = scene->GetGameObject<MainGame::MainCamera>(scene->CAMERA);
		auto field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);
		auto trans = m_pPlaceObject->GetComponent<Transform>();

		Matrix view = camera->GetViewMatrix();
		Matrix proj = camera->GetProjectionMatrix();

		//レイを飛ばす
		Ray ray = Utility::ScreenPosToRay(mousePos.x, mousePos.y - SCREEN_HEIGHT_HALF * 0.2f, &view, &proj);
		auto playerTrans = scene->GetGameObject<MainGame::Player>(scene->OBJECT)->GetComponent<Transform>();
		std::list<Triangle> triangles;
		field->GetTriangles(triangles, playerTrans->GetPosition());

		//離されたら
		if (GET_INPUT.GetKeyRelease(KEY_CONFIG::MOUSE_L))
		{
			auto col = m_pPlaceObject->GetComponent<CollisionComponent>();
			if (col)
			{
				col->SetIsStaticObject(true);
			}

			m_pSnapObjectList.push_back(m_pPlaceObject->CreateSnapInfo());

			m_pPlaceObject = nullptr;
			m_placeObjectData.reset();

			//経路探索ノードに設置物の反映
			{
				field->SetNotTraffic(trans->GetPosition());
				auto position = trans->GetPosition();
				auto scale = trans->GetScale();
				auto quat = trans->GetQuaternion();

				//x軸
				{
					Vector3 axizScale(scale.x, 0, 0);

					axizScale = Vector3::Transform(axizScale, quat);
					field->SetNotTraffic(position + axizScale);

					field->SetNotTraffic(position - axizScale);
				}
				{
					//z軸
					Vector3 axizScale(0, 0, scale.z);

					axizScale = Vector3::Transform(axizScale, quat);
					field->SetNotTraffic(position + axizScale);

					field->SetNotTraffic(position - axizScale);
				}
			}
			return;
		}


		//スナップ
		for (auto snap : m_pSnapObjectList)
		{
			float snapDist = 0;
			if (snap.m_obb.Intersects(ray.position, ray.direction, snapDist))
			{
				Vector3 hitPos = ray.position + ray.direction * snapDist; //オブジェクト上の交差点

				m_pPlaceObject->SnapBridge(snap.m_obj, snap, hitPos);
				return;
			}
		}

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

		//当たりの距離から位置の計算
		if (dist >= 1.0f)
		{
			Vector3 pos;
			pos = ray.position + ray.direction * dist;

			float playerLength = (playerTrans->GetPosition() - pos).LengthSquared();

			if (playerLength < 8)
			{
				Vector3 cameraForward = camera->GetCamaraForward();
				pos += cameraForward * 0.1f;
				pos.y = field->GetHeight(pos);
				trans->SetPosition(pos);
			}
			else
				trans->SetPosition(pos);
		}

		//回転
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_L))
			trans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, -Placement::ROTATION_SPEED));
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_R))
			trans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, Placement::ROTATION_SPEED));
	}
}

void GameUI::CreateObjectAtID(int staticObjectID)
{
	auto scene = SceneManager::GetInstance().GetScene();


	m_placeObjectData = StaticDataTable::GetInstance().GetStaticObjectData(staticObjectID);
	auto modelData = StaticDataTable::GetInstance().GetModelData(m_placeObjectData->GetModelID());//モデルデータ取得

	switch (m_placeObjectData->GetType())
	{
	case STATICOBJECT_TYPE::WALL:
		m_pPlaceObject = scene->AddGameObject<Placement::Wall>(scene->OBJECT);
		break;
	case STATICOBJECT_TYPE::FOUNDATION:
		m_pPlaceObject = scene->AddGameObject<Placement::Foundation>(scene->OBJECT);
		break;
	}

	auto model = m_pPlaceObject->AddComponent<DrawModel>(this);

	model->Load(modelData->GetPath().c_str());
	model->SetShader(SHADER_TYPE::NORMAL_MAPPING);

	m_pPlaceObject->GetComponent<Transform>()->SetScale(modelData->GetScale());
	auto col = m_pPlaceObject->AddComponent<OBBCollision>();
	col->SetScale(m_placeObjectData->GetCollisionScale());
	col->SetIsStaticObject(false);
	col->SetHitAction(HitAction::None);
	col->SetCenterPosition(m_placeObjectData->GetPositionOffset());

	auto rigid = m_pPlaceObject->AddComponent<Rigidbody>();
	rigid->SetIsKinematic(true);

	m_pPlaceObject->SetTag(TAG::STATIC_OBJECT);
	//UIを消す
	m_pPlacementUI->InOutUI(false);
	m_pPlacementUI->ResetModelID();
}