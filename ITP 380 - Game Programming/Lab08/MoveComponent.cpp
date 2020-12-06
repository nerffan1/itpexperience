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
	//Update position
	Vector3 dl = mNormal * mForwardSpeed * deltaTime;
	Vector3 dpos = mOwner->GetPosition() + dl;
	mOwner->SetPosition(dpos);
}
