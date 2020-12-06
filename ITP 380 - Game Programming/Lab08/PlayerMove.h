#pragma once
#include "VehicleMove.h"
#include "Math.h"

class PlayerMove : public VehicleMove
{
public:
	PlayerMove(class Player* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState);
	void CheckBoundary();

	//Other Functions
	void ShootControls(const Uint8* keyState);
	void CheckCollisions();
	void SpeedUpdate(float deltatime);

	//Lap/UI
	void OnLapChange(int newLap) override;

protected:
	Vector3 mVelocity;
	class Player* mPlayer;

	//Bools 
	bool LastInput = false;

	//other
	int mfastMusic = 0;
};