#include "PlacementObjectSnap.h"

void Foundation::SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos)
{
	auto objTrans = obj->GetComponent<Transform>();
	//一番近いスナップポイントを計算
	auto snapPoint = NearestSnapPoint(info.m_snapPointList, hitPos);

	//スナップポイントから一番近い自分のスナップポイントを計算
	auto objSnapInfo = obj->CreateSnapInfo();
	SnapPoint mySnapPoint = NearestSnapPoint(objSnapInfo.m_snapPointList, info.m_obb.Center);

	Vector3 pointDiff = objTrans->GetPosition() - mySnapPoint.position;

	objTrans->SetPosition(snapPoint.position + pointDiff);
}

void Foundation::SetSnapPoint(Wall* obj, SnapObjectInfo info, Vector3 hitPos)
{
	auto objTrans = obj->GetComponent<Transform>();
	//一番近いスナップポイントを計算
	auto snapPoint = NearestSnapPoint(info.m_snapPointList, hitPos);

	//スナップポイントから一番近い自分のスナップポイントを計算
	auto objSnapInfo = obj->CreateSnapInfo();
	SnapPoint mySnapPoint = NearestSnapPoint(objSnapInfo.m_snapPointList, info.m_obb.Center);

	Vector3 pointDiff = objTrans->GetPosition() - mySnapPoint.position;

	objTrans->SetPosition(snapPoint.position + pointDiff);
}

void Wall::SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos)
{
	auto objTrans = obj->GetComponent<Transform>();
	//一番近いスナップポイントを計算
	auto snapPoint = NearestSnapPoint(info.m_snapPointList, hitPos);

	//スナップポイントから一番近い自分のスナップポイントを計算
	auto objSnapInfo = obj->CreateSnapInfo();
	SnapPoint mySnapPoint = NearestSnapPoint(objSnapInfo.m_snapPointList, info.m_obb.Center);

	Vector3 pointDiff = objTrans->GetPosition() - mySnapPoint.position;

	objTrans->SetPosition(snapPoint.position + pointDiff);
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
		objTrans->SetQuaternion(this->GetComponent<Transform>()->GetRotation());
	}
	else
	{
		mySnapPoint = NearestSnapPoint(objSnapList, info.m_obb.Center);
		Vector3 pointDiff = objTrans->GetPosition() - mySnapPoint.position;

		objTrans->SetPosition(staticObjectSnapPoint.position + pointDiff);

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
	info.m_obb.Orientation = transform->GetRotation();


	auto scale = transform->GetScale();
	auto quat = transform->GetRotation();
	quat.Normalize();

	//y軸
	{
		Vector3 axizScale(0, scale.y, 0);

		axizScale = Vector3::Transform(axizScale, quat);

		SnapPoint snapPoint;
		snapPoint.position = position + axizScale;
		snapPoint.axiz = Axiz::Y;
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

SnapObjectInfo Foundation::CreateSnapInfo()
{
	//スナップ用情報セット
	SnapObjectInfo info;
	auto transform = GetComponent<Transform>();
	auto position = transform->GetPosition();
	info.m_obb.Center = position;
	info.m_obb.Extents = GetComponent<CollisionComponent>()->GetCollisionScale();
	info.m_obb.Orientation = transform->GetRotation();


	auto scale = transform->GetScale();
	auto quat = transform->GetRotation();
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
		snapPoint.axiz = Axiz::X;
		info.m_snapPointList.push_back(snapPoint);

		snapPoint.position = position - axizScale;
		info.m_snapPointList.push_back(snapPoint);
	}
	info.m_obj = this;

	return info;
}

void Wall::SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos)
{
	obj->SetSnapPoint(this, info, hitPos);
}

void Foundation::SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos)
{
	obj->SetSnapPoint(this, info, hitPos);
}

SnapPoint PlacementObject::NearestSnapPoint(std::vector<SnapPoint> snapPointList, Vector3 position)
{
	SnapPoint snapPoint;
	{
		float nearDistance = 100;
		int nearIndex = 0;
		for (int i = 0; i < snapPointList.size(); i++)
		{
			float dis = (snapPointList[i].position - position).Length();
			if (dis < nearDistance)
			{
				nearDistance = dis;
				nearIndex = i;
			}
		}
		snapPoint = snapPointList[nearIndex];
	}
	return snapPoint;
}
