#include "Foundation.h"
#include "Wall.h"

namespace Placement
{
	//床と床
	void Foundation::SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos)
	{
		auto objTrans = obj->GetComponent<Transform>();
		//一番近いスナップポイントを計算
		auto snapPoint = NearestSnapPoint(info.m_snapPointList, hitPos);

		//スナップポイントから一番近いスナップポイントを計算
		auto objSnapList = obj->CreateSnapInfo().m_snapPointList;

		auto itrNewEnd = std::remove_if(objSnapList.begin(), objSnapList.end(), [&snapPoint](SnapPoint mySnap) ->bool
			{
				if (snapPoint.axiz == Axiz::Z && mySnap.axiz != Axiz::Z)
					return true;
				else if (snapPoint.axiz == Axiz::X && mySnap.axiz != Axiz::X)
					return true;
				return false;
			});
		objSnapList.erase(itrNewEnd, objSnapList.end());


		SnapPoint mySnapPoint = NearestSnapPoint(objSnapList, info.m_obb.Center);

		Vector3 pointDiff = objTrans->GetPosition() - mySnapPoint.position;

		objTrans->SetPosition(snapPoint.position + pointDiff);
		objTrans->SetQuaternion(info.m_obb.Orientation);
	}

	//床と壁
	void Foundation::SetSnapPoint(Wall* obj, SnapObjectInfo info, Vector3 hitPos)
	{
		auto objTrans = obj->GetComponent<Transform>();
		//一番近いスナップポイントを計算
		auto snapPoint = NearestSnapPoint(info.m_snapPointList, hitPos);


		objTrans->SetPosition(snapPoint.position);
		if (snapPoint.axiz == Axiz::X)
		{
			objTrans->SetQuaternion(info.m_obb.Orientation);
		}
		else if (snapPoint.axiz == Axiz::Z)
		{
			objTrans->SetQuaternion(info.m_obb.Orientation);
			objTrans->AddQuaternion(Quaternion::CreateFromAxisAngle(Vector3::Up, (float)TO_RADIAN(90)));
		}
	}

	SnapObjectInfo Foundation::CreateSnapInfo()
	{
		//スナップ用情報セット
		SnapObjectInfo info;
		auto transform = GetComponent<Transform>();
		auto position = transform->GetPosition();
		info.m_obb.Center = position;
		info.m_obb.Extents = GetComponent<CollisionComponent>()->GetCollisionScale();
		info.m_obb.Orientation = transform->GetQuaternion();


		auto scale = transform->GetScale();
		auto quat = transform->GetQuaternion();
		quat.Normalize();

		//x軸
		{
			Vector3 axizScale(scale.x, 0, 0);

			axizScale = Vector3::Transform(axizScale, quat);
			SnapPoint snapPoint;
			snapPoint.position = position + axizScale;
			snapPoint.axiz = Axiz::X;
			info.m_snapPointList.push_back(snapPoint);

			snapPoint.position = position - axizScale;
			info.m_snapPointList.push_back(snapPoint);
		}

		//z軸
		{
			Vector3 axizScale(0, 0, scale.z);

			axizScale = Vector3::Transform(axizScale, quat);
			SnapPoint snapPoint;
			snapPoint.position = position + axizScale;
			snapPoint.axiz = Axiz::Z;
			info.m_snapPointList.push_back(snapPoint);

			snapPoint.position = position - axizScale;
			info.m_snapPointList.push_back(snapPoint);
		}
		info.m_obj = this;

		return info;
	}

	void Foundation::SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos)
	{
		obj->SetSnapPoint(this, info, hitPos);
	}
}