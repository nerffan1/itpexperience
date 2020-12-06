#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner)
:Component(owner, 50)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
{
}

void MoveComponent::Update(float deltaTime)
{
	//Update rotation
	mOwner->SetRotation(mOwner->GetRotation() + (mAngularSpeed*deltaTime));
	//Update position
	Vector2 dpos(mOwner->GetPosition().x + (mOwner->GetForward().x * mForwardSpeed*deltaTime),mOwner->GetPosition().y + (mOwner->GetForward().y * mForwardSpeed*deltaTime));
	mOwner->SetPosition(dpos);
}
