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
	
	//Camera Roll
	Vector3 CameraUp(float deltatime);
	void WallRunRot(class PlayerMove* PlayersMov, float& deltaTime);
	void FixUp(float& deltaTime);

protected:
	class Player* player = nullptr;
	Vector3 mCamVel;
	Vector3 mCamPos;
	Vector3 mCamNorm = Vector3::UnitZ;
	float mCamWallRot = 0.0f;

	//Camera Pitch
	float mPitchAngle = 0.0f;
	float mPitchSpeed = 0.0f;

	//Timers 
	float mFixNormalTimer = 0.0f;
	float mRotationTimer = 0.0f;

	//Bools
	bool LastStateWallRun = false;

	//Constant
	const float HDist = 70.0f;
	const float VDist = 70.0f;
	const float TargetOffset = 75.0f;
};