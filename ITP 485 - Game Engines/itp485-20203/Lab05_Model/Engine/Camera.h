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
		Vector3 mposition;
		float Padding;
	};

	Camera(class Graphics* mGraphics);
	void SetActive();

	Matrix4 mWorldToCam;
	Matrix4 mPerspective;
	PerCameraConstants mConsts;
protected:
	class Graphics* mGraphics;
	ID3D11Buffer*  mCamBuff; //Const?
};