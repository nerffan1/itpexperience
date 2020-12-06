#pragma once
#include "Component.h"
#include "Math.h"

class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner);
	void Update(float deltaTime) override;
	Vector3 GetIdealPos();
	Vector3 GetCamPos(float& deltatime);
	Vector3 PlayerNormal();

	//Pitch Functions
	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	float GetPitchSpeed() { return mPitchSpeed; }

	//Target
	Vector3 UpdateTarget(float& deltatime);

protected:
	class Player* player = nullptr;
	Vector3 mCamVel;
	Vector3 mCamPos;

	//Camera Pitch
	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;

	//Constant
	const float HDist = 70.0f;
	const float VDist = 70.0f;
	const float TargetOffset = 75.0f;
};