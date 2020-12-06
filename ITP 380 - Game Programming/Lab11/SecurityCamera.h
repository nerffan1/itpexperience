#pragma once
#include "Actor.h"

class SecurityCamera : public Actor
{
public: 
	SecurityCamera(class Game* game);
	~SecurityCamera();
	void OnUpdate(float deltaTime) override;
	void SetStartQ(Quaternion start) { mStartQ = start; }
	void SetEndQ(Quaternion end) { mEndQ = end; }
	void SetIntTime(float IntTime) { INT_TIME = IntTime; }
	void SetPauseTime(float PTime) { PAUSE_TIME = PTime; }

	//Camera Rot
	void CameraRotation(float& deltaTime);
	int CameraVolume();

protected:
	class SecurityCone* mCone = nullptr;
	Quaternion mStartQ;
	Quaternion mEndQ;
	float InterpTimer = 0.0f;
	float PauseTimer = 0.0f;

	//Sound
	int mRotSFX = 0;
	int mStopSFX = 0;

	//Variables
	float INT_TIME = 0.0f;
	float PAUSE_TIME = 0.0f;
};