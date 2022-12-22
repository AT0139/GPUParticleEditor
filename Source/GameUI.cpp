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
		auto scene = Manager::GetInstance().GetScene();

		//マウスポジションから座標を計算
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

		//回転
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_L))
			trans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, -ROTATION_SPEED));
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_R))
			trans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, ROTATION_SPEED));

		//離されたら
		if (GET_INPUT.GetKeyRelease(KEY_CONFIG::MOUSE_L))
		{
			auto col = m_pPlaceObject->GetComponent<CollisionComponent>();
			if (col)
			{
				col->SetIsStaticObject(true);
			}

			m_pSnapObjectList.push_back(CreateSnapInfo());

			m_pPlaceObject = nullptr;
			m_placeObjectData.reset();
			return;
		}


		//スナップ
		for (auto snap : m_pSnapObjectList)
		{
			float snapDist = 0;
			if (snap.m_obb.Intersects(ray.position, ray.direction, snapDist))
			{
#ifdef _DEBUG
				ImGui::Begin("Mouse");
				{
					ImGui::Text("Ray Intersect!");
				}
				ImGui::End();
#endif
				Vector3 hitPos = ray.position + ray.direction * snapDist; //オブジェクト上の交差点

				//一番近いスナップポイントインデックスを計算
				Vector3 snapPoint;
				{
					float nearDistance = 100;
					int nearIndex = 0;
					for (int i = 0; i < snap.m_snapPoint.size(); i++)
					{
						float dis = (snap.m_snapPoint[i] - trans->GetPosition()).Length();
						if (dis < nearDistance)
						{
							nearDistance = dis;
							nearIndex = i;
						}
					}
					snapPoint = snap.m_snapPoint[nearIndex];
				}
				//スナップポイントから一番近い自分のスナップポイントを計算
				auto mySnapInfo = CreateSnapInfo();
				Vector3 mySnapPoint;
				{
					float nearDistance = 100;
					int nearIndex = 0;
					for (int i = 0; i < mySnapInfo.m_snapPoint.size(); i++)
					{
						float dis = (mySnapInfo.m_snapPoint[i] - snap.m_obb.Center).Length();
						if (dis < nearDistance)
						{
							nearDistance = dis;
							nearIndex = i;
						}
					}
					mySnapPoint = mySnapInfo.m_snapPoint[nearIndex];
				}
				Vector3 pointDiff = trans->GetPosition() - mySnapPoint;

				trans->SetPosition(snapPoint+ pointDiff);
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
			trans->SetPosition(pos);
		}
	}
}

void GameUI::CreateObjectAtID(int staticObjectID)
{
	auto scene = Manager::GetInstance().GetScene();
	m_pPlaceObject = scene->AddGameObject<BlankObject>(scene->OBJECT);
	m_placeObjectData = StaticDataTable::GetInstance().GetStaticObjectData(staticObjectID);
	auto modelData = StaticDataTable::GetInstance().GetModelData(m_placeObjectData->GetModelID());//モデルデータ取得

	auto model = m_pPlaceObject->AddComponent<DrawModel>(this);

	model->Load(modelData->GetPath().c_str());
	model->SetVertexShader("NormalMappingVS.cso");
	model->SetPixelShader("NormalMappingPS.cso");

	m_pPlaceObject->GetComponent<Transform>()->SetScale(modelData->GetScale());
	auto col = m_pPlaceObject->AddComponent<OBBCollision>();
	col->SetScale(m_placeObjectData->GetCollisionScale());
	col->SetIsStaticObject(false);

	auto rigid = m_pPlaceObject->AddComponent<Rigidbody>();
	rigid->SetIsKinematic(true);

	m_pPlaceObject->AddComponent<SerializeComponent>(this);
	//UIを消す
	m_pPlacementUI->SetHidden(true);
	m_pPlacementUI->ResetModelID();
}

GameUI::SnapObjectInfo GameUI::CreateSnapInfo()
{
	//スナップ用情報セット
	SnapObjectInfo info;
	auto transform = m_pPlaceObject->GetComponent<Transform>();
	auto position = transform->GetPosition();
	info.m_obb.Center = position;
	info.m_obb.Extents = m_pPlaceObject->GetComponent<CollisionComponent>()->GetCollisionScale();
	info.m_obb.Orientation = transform->GetRotation();


	auto scele = transform->GetScale();
	auto quat = transform->GetRotation();
	quat.Normalize();

	//x軸
	if(m_placeObjectData->IsSnapX())
	{
		Vector3 a(scele.x, 0, 0);

		a = Vector3::Transform(a, quat);
		info.m_snapPoint.push_back(position + a);

		info.m_snapPoint.push_back(position - a);
	}

	//y軸
	if (m_placeObjectData->IsSnapY())
	{
		Vector3 a(0, scele.y, 0);	

		a = Vector3::Transform(a, quat);
		info.m_snapPoint.push_back(position + a);

		info.m_snapPoint.push_back(position - a);
	}

	//z軸
	if (m_placeObjectData->IsSnapZ())
	{
		Vector3 a(0, 0, scele.z);

		a = Vector3::Transform(a, quat);
		info.m_snapPoint.push_back(position + a);

		info.m_snapPoint.push_back(position - a);
	}

	return info;
}
