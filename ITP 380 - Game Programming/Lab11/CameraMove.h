#pragma once
#include "Component.h"
#include "Math.h"

class CameraMove : public Component
{
public:
	CameraMove(class Actor* owner);
	void Update(float deltaTime) override;
	Vector3 GetIdealPos();
	Vector3 GetCamPos(float& deltatime);
	Vector3 PlayerNormal();

protected:
	class Player* player = nullptr;
	Vector3 mCamVel;
	Vector3 mCamPos;
	Vector3 mSpringAccel;
	Vector3 mSpringVelocity;

	//Constant
	const float HDist = 70.0f;
	const float VDist = 70.0f;
	const float TargetOffset = 75.0f;
};