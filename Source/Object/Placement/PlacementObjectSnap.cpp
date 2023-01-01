#include "PlacementObjectSnap.h"
#include "Wall.h"

namespace Placement
{
	PlacementObject::PlacementObject()
	{}

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
}