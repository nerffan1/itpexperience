#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner);

	// Update the move component
	virtual void Update(float deltaTime) override;

	// Getters/setters
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	Vector3 GetNormal() const { return mNormal; }
	void SetNormal(Vector3 NewNormal) { mNormal = NewNormal; }
	void SetStrafeSpeed(float speed) { mStrafeSpeed += speed; }
	float GetStrafeSpeed() { return mStrafeSpeed; }
protected:
	float mAngularSpeed;
	float mForwardSpeed;
	float mStrafeSpeed = 0.0f;

	//Normal Component
	Vector3 mNormal;

};
