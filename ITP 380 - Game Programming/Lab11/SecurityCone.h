#pragma once
#include "Actor.h"

class SecurityCone : public Actor
{
public:
	enum ConeState
	{
		Searching,
		Alert,
		InSight
	};
	SecurityCone(class Game* game, Actor* parent);
	void OnUpdate(float deltaTime);
	void SetState(ConeState stat);
	void CheckForPlayer();
	ConeState GetConeState() { return mConeState; }

protected:
	class Player* mPlayer = nullptr;
	ConeState mConeState = Searching;
	int mAlertSFX = 0;

	//Timers
	float mAlertTimer = 2.0f;
	float mInSightTimer = 0.5f;

	//Constants
	const float CONEHEIGHT = 100.0f * 3.0f;
	const float ALERT_TIMER = 2.0f;
	const float IN_SIGHT_TIMER = 0.5f;
};