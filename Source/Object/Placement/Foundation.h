#pragma once

#include "PlacementObjectSnap.h"


namespace Placement
{
	class Foundation : public PlacementObject
	{
	public:
		Foundation() {}
		~Foundation() {}

		// PlacementObject を介して継承されました
		virtual void SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos) override;
		virtual void SetSnapPoint(Wall* obj, SnapObjectInfo info, Vector3 hitPos) override;

		virtual SnapObjectInfo CreateSnapInfo() override;

		virtual void SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos) override;
	};

}