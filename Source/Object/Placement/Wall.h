#pragma once

#include "PlacementObjectSnap.h"


namespace Placement
{
	class Wall : public PlacementObject
	{
	public:
		Wall() {}
		~Wall() {}

		// PlacementObject を介して継承されました
		virtual void SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos) override;
		virtual void SetSnapPoint(Wall* placeObj, SnapObjectInfo placedInfo, Vector3 hitPos) override;

		virtual SnapObjectInfo CreateSnapInfo() override;

		virtual void SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos) override;
	};
}