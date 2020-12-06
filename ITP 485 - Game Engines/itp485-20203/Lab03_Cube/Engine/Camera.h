#pragma once
#include "engineMath.h"
#include <d3d11.h>
#include "engineMath.h"

class Camera
{
public:

	virtual ~Camera() ;

	struct PerCameraConstants {
		Matrix4 viewProj;
		Matrix4 mWorldToCam;
		Vector3 mPosition;
		float Padding;
	};

	Camera(class Graphics* mGraphics);
	void SetActive();

protected:
	class Graphics* mGraphics;
	PerCameraConstants mConsts;
	ID3D11Buffer*  mCamBuff; //Const?
};