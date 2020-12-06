#pragma once
#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState);
	void CheckBoundary();

	//Other Functions
	void UpdateCamera();
	void ShootControls(const Uint8* keyState);
	void CheckCollisions();
	void SpeedUpdate(float deltatime);

protected:
	Vector3 mVelocity;
	class Player* mPlayer;
	bool mStationary;
	float SpeedUpTimer = 10.0f;
	float SpeedMult = 1.0f;
	//Constant
	const float PLANAR_VEL = 300.0f;
	const float BoundY = 195;
	const float BoundZ = 225;
	const float HDist = 300.0f;
	const float VDist = 100.0f;
	const float TargetD = 20.0f;

	//Bools 
	bool LastInput = false;
};