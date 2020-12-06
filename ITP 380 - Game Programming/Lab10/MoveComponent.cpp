#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
	:Component(owner, 50)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	//Update Rotation
	mOwner->SetRotation(mOwner->GetRotation() + mAngularSpeed * deltaTime);
	
	//Update Linear Movement
	Vector3 WSMov = mForwardSpeed * mOwner->GetForward();
	Vector3 ADMov = mOwner->GetRight() * mStrafeSpeed;
	Vector3 dl = (WSMov + ADMov) * deltaTime;
	Vector3 dpos = mOwner->GetPosition() + dl;
	mOwner->SetPosition(dpos);


}
