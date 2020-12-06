#pragma once
#include "MoveComponent.h"

class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner);
	void ProcessInput(const Uint8* keyState) override;
	void Update(float deltaTime) override;

	//Actions
	void Jump(float magnitude);

	//Update
	void VerticalMovement(float& deltaTime);
	void UpdateCamera();

	//Bools
	bool GetAirBool();
	bool GetSpacePressed();

	//Check for other Actors
	void CheckActors();

	//Helper functions
	bool MarioXNeighbourhood(class Actor* otherActor);
	bool MarioYNeighbourhood(class Actor* otherActor);
protected:
	Player* mPlayer = nullptr;
	float mYSpeed = 0.0f;
	bool mSpacePressed = false;
	bool mJumpEdge = false;
	bool mInAir = true;

	//Useful variables to only check nearby objects
	float mXBoundary = 40.0f;
	float mYBoundary = 40.0f;

	//Constants
	const float SPEED = 300.0f;
	const float ACCEL = 2000.0f;
	const float FALL_DEATH = 460.0f;
	const float JUMP = -750.0f;

};