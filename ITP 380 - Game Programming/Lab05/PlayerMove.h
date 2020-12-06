#pragma once
#include "MoveComponent.h"
#include "Math.h"
class PlayerMove : public MoveComponent
{
public:
	PlayerMove(class Player* owner);
	void ProcessInput(const Uint8* keyState) override;
	void Update(float deltaTime) override;

	//Actions
	void Teleport();

	//Update
	void UpdateCamera();
	void UpdateMovement(float &deltaTime);
	
	//Sword
	void UpdateSword(float& deltatime);
	void SwordEdge(const Uint8* key);
	bool GetSwordEdge() { return mSwordEdge; }
	void SetSword();

	//Bools,
	bool GetSpacePressed();
	bool GetStationary() { return mStationary; }

	//Check for other Actors
	void CheckColliders();
	void CheckDoors();
	void CheckSecretBlocks();

	//Getter
	float GetAttackTime() { return mAttackTime; }

protected:
	class Player* mPlayer = nullptr;
	bool mSpacePressed = false;
	bool mSwordEdge = false;
	bool mStationary = true;

	//Useful variables to only check nearby objects
	float mXBoundary = 40.0f;
	float mYBoundary = 40.0f;

	//Constants
	const float SPEED = 130.0f;
	const float TELEPORT = 145.0f;

	//Map parameters
	const float WINDOW_HEIGHT = 448.0f;
	const float WINDOW_WIDTH = 600.0f;

	//Sword Parameters 
	class Sword* mSword = nullptr;
	float mAttackTime;
	bool mSpaceIsPressed = false;
};