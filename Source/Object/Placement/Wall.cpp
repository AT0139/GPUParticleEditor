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
		auto objSnapList = CreateSnapInfo().m_snapPointList;
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
	void Wall::SetSnapPoint(Wall* placeObj, SnapObjectInfo placedInfo, Vector3 hitPos)
	{
		auto placeTrans = placeObj->GetComponent<Transform>();	//設置する方のトランスフォーム
		auto placedObjectSnapPoint = NearestSnapPoint(placedInfo.m_snapPointList, hitPos);	//一番近いスナップポイントを計算

		auto placeInfo = placeObj->CreateSnapInfo();
		auto placeSnapList = placeInfo.m_snapPointList;

		//スナップできないポイントの削除
		auto itrNewEnd = std::remove_if(placeSnapList.begin(), placeSnapList.end(), [&placedObjectSnapPoint](SnapPoint snapPoint) ->bool
			{
				if (placedObjectSnapPoint.axiz == Axiz::Z && snapPoint.axiz != Axiz::Z)
					return true;
				else if (placedObjectSnapPoint.axiz == Axiz::Y && snapPoint.axiz != Axiz::Y)
					return true;
				return false;
			});
		placeSnapList.erase(itrNewEnd, placeSnapList.end());

		//回転
		{
			if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_L))
				placeTrans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, -ROTATION_SPEED));
			if (GET_INPUT.GetKeyPress(KEY_CONFIG::OBJECT_ROTATE_R))
				placeTrans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, ROTATION_SPEED));
		}

		//スナップポイントから一番近い自分(設置されてる方)のスナップポイントを計算
		SnapPoint mySnapPoint;
		if (placedObjectSnapPoint.axiz == Axiz::Y)
		{
			//Y軸方向設置
			Vector3 hitDir = hitPos - placeInfo.m_obb.Center;
	
			if (placeSnapList[0].position.y < placeSnapList[1].position.y)
			{
				mySnapPoint = placeSnapList[0];
			}
			else
			{
				mySnapPoint = placeSnapList[1];
			}

			Vector3 pointDiff = placeTrans->GetPosition() - mySnapPoint.position;

			placeTrans->SetPosition(placedObjectSnapPoint.position + pointDiff);
			placeTrans->SetQuaternion(this->GetComponent<Transform>()->GetQuaternion());
		}
		else
		{
			mySnapPoint = NearestSnapPoint(placeSnapList, placedInfo.m_obb.Center);

			Vector3 pointDiff = placeTrans->GetPosition() - mySnapPoint.position;

			if (pointDiff.Length() <= 0.01f)
			{
				Vector3 pointDiff = placeTrans->GetPosition() - mySnapPoint.position;
			}
			placeTrans->SetPosition(placedObjectSnapPoint.position + pointDiff);
		}
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