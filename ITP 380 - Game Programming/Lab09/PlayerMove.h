#pragma once
#include "MoveComponent.h"
#include "Math.h"

class PlayerMove : public MoveComponent
{
public:
	enum MoveState
	{
		OnGround,
		Jump,
		Falling
	};
	PlayerMove(class Player* owner);
	void Update(float deltaTime) override;
	void ProcessInput(const Uint8* keyState);
	
	//Control
	void CameraControl();
	void WASDMovement(const Uint8* keyState);
	void FixXYVelocity();

	//State
	void SetState(MoveState stat) { mState = stat; }
	MoveState GetState() { return mState; }

protected:
	class Player* mPlayer = nullptr;
	MoveState mState;
	Vector3 mVel;
	Vector3 mAccel;
	Vector3 mPendingForces;
	float mMass = 1.0f;

	//State Update
	void UpdateOnGround(float& deltaTime);
	void UpdateJump(float& deltaTime);
	void UpdateFalling(float& deltaTime);

	//Fix Coll
	enum class CollSide FixCollision(class CollisionComponent* self, class CollisionComponent* block);
	void CheckCollision();

	//Movement
	void PhysicsUpdate(float deltaTime);
	void AddForce(const Vector3& force);

	//Bools
	bool LastInput = false;

	//Constants 
	const Vector3 GRAVITY = Vector3(0.0f, 0.0f, -980.0f);
	const Vector3 JUMP_FORCE = Vector3(0.0f, 0.0f, 35000.0f);
	const float MAX_VEL = 400.0f;
	const float BRAKING_FACTOR = 0.9f;
};