#pragma once

#include "Camera.h"

class ParticleDemoSceneCamera : public Camera
{
public:
	ParticleDemoSceneCamera();
	virtual ~ParticleDemoSceneCamera();
	virtual void Update()override;

private:

};