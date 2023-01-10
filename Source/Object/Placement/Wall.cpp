#include "Wall.h"
#include "Foundation.h"
#include "SceneManager.h"
#include "Scene.h"
#include "MeshField.h"
#include "Input.h"

namespace Placement
{
	//壁と床
	void Wall::SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos)
	{
		auto objTrans = obj->GetComponent<Transform>();

		auto staticSnapPointList = CreateSnapInfo().m_snapPointList;

		auto scene = SceneManager::GetInstance().GetScene();
		auto field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);

		auto it = std::remove_if(staticSnapPointList.begin(), staticSnapPointList.end(), [&field](SnapPoint it)->bool
			{
				if (it.axiz != Axiz::Y)
					return true;
				float y = field->GetHeight(it.position);
				return y < it.position.y;
			});
		staticSnapPointList.erase(it, staticSnapPointList.end());

		//一番近いスナップポイントを計算
			SnapPoint snapPoint;
		if (staticSnapPointList.size() != 0)
			snapPoint = NearestSnapPoint(staticSnapPointList, hitPos);

		//スナップポイントから一番近い自分のスナップポイントを計算
		auto objSnapList = obj->CreateSnapInfo().m_snapPointList;
		auto itEnd = std::remove_if(objSnapList.begin(), objSnapList.end(), [](SnapPoint it)->bool
			{
				return it.axiz == Axiz::Z;
			});
		objSnapList.erase(itEnd, objSnapList.end());
		SnapPoint mySnapPoint = NearestSnapPoint(objSnapList, snapPoint.position);

		Vector3 pointDiff = objTrans->GetPosition() - mySnapPoint.position;
		objTrans->SetPosition(snapPoint.position + pointDiff);
		objTrans->SetQuaternion(info.m_obb.Orientation);
	}

	//壁と壁のスナップ
	void Wall::SetSnapPoint(Wall* obj, SnapObjectInfo info, Vector3 hitPos)
	{
		auto objTrans = obj->GetComponent<Transform>();

		//一番近いスナップポイントを計算
		auto staticObjectSnapPoint = NearestSnapPoint(info.m_snapPointList, hitPos);



		//スナップポイントから一番近い自分のスナップポイントを計算
		auto snapInfo = obj->CreateSnapInfo();
		auto objSnapList = snapInfo.m_snapPointList;

		//スナップできないポイントの削除
		auto itrNewEnd = std::remove_if(objSnapList.begin(), objSnapList.end(), [&staticObjectSnapPoint](SnapPoint snapPoint) ->bool
			{
				if (staticObjectSnapPoint.axiz == Axiz::Z && snapPoint.axiz != Axiz::Z)
					return true;
				else if (staticObjectSnapPoint.axiz == Axiz::Y && snapPoint.axiz != Axiz::Y)
					return true;
				return false;
			});
		objSnapList.erase(itrNewEnd, objSnapList.end());

		SnapPoint mySnapPoint;
		if (staticObjectSnapPoint.axiz == Axiz::Y)
		{
			Vector3 hitDir = hitPos - snapInfo.m_obb.Center;
			if (hitDir.y >= 0)
			{
				if (objSnapList[0].position.y < objSnapList[1].position.y)
					mySnapPoint = objSnapList[0];
				else
					mySnapPoint = objSnapList[1];
			}
			else
			{
				if (objSnapList[0].position.y < objSnapList[1].position.y)
					mySnapPoint = objSnapList[0];
				else
					mySnapPoint = objSnapList[1];
			}

			Vector3 pointDiff = objTrans->GetPosition() - mySnapPoint.position;

			objTrans->SetPosition(staticObjectSnapPoint.position + pointDiff);
			objTrans->SetQuaternion(this->GetComponent<Transform>()->GetQuaternion());
		}
		else
		{
			mySnapPoint = NearestSnapPoint(objSnapList, info.m_obb.Center);
			Vector3 pointDiff = objTrans->GetPosition() - mySnapPoint.position;

			objTrans->SetPosition(staticObjectSnapPoint.position + pointDiff);

		}

		//回転
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_L))
			objTrans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, -ROTATION_SPEED));
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_R))
			objTrans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, ROTATION_SPEED));
	}

	SnapObjectInfo Wall::CreateSnapInfo()
	{
		//スナップ用情報セット
		SnapObjectInfo info;
		auto transform = GetComponent<Transform>();
		auto position = transform->GetPosition();
		info.m_obb.Extents = GetComponent<CollisionComponent>()->GetCollisionScale();
		info.m_obb.Center = position;
		info.m_obb.Center.y += info.m_obb.Extents.y;
		info.m_obb.Orientation = transform->GetQuaternion();


		auto scale = transform->GetScale();
		auto quat = transform->GetQuaternion();
		quat.Normalize();

		//y軸
		{
			Vector3 axizScale(0, scale.y, 0);

			axizScale = Vector3::Transform(axizScale, quat);

			SnapPoint snapPoint;
			snapPoint.position = info.m_obb.Center + axizScale;
			snapPoint.axiz = Axiz::Y;
			info.m_snapPointList.push_back(snapPoint);

			snapPoint.position = info.m_obb.Center - axizScale;
			info.m_snapPointList.push_back(snapPoint);
		}

		//z軸
		{
			Vector3 axizScale(0, 0, scale.z);

			axizScale = Vector3::Transform(axizScale, quat);

			SnapPoint snapPoint;
			snapPoint.position = info.m_obb.Center + axizScale;
			snapPoint.axiz = Axiz::Z;
			info.m_snapPointList.push_back(snapPoint);

			snapPoint.position = info.m_obb.Center - axizScale;
			info.m_snapPointList.push_back(snapPoint);
		}

		info.m_obj = this;

		return info;
	}

	void Wall::SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos)
	{
		obj->SetSnapPoint(this, info, hitPos);
	}
}