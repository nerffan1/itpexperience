#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include "Game.h"
#include "CollisionComponent.h"

Actor::Actor(Game* game)
	:mGame(game)
	,mState(ActorState::Active)
	,mPosition(Vector3::Zero)
	,mScale(1.0f)
	,mRotation(0.0f)
	,mDirection(Direction::Up)
{
	LoadMaps();
	mGame->AddActor(this);
}

Actor::~Actor()
{
	//Remove all components of actor
	for (auto i : this->mComponents)
	{
		delete i;
	}
	this->mComponents.clear();

	//Delete actors
	mGame->RemoveActor(this);
}

void Actor::Update(float deltaTime)
{
	// TODO
	if (GetState() == ActorState::Active || GetState() == ActorState::Paused)
	{								
		for (auto i : mComponents)
		{
			i->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}

	//3D Update 
	Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
	Matrix4 positionMatrix = Matrix4::CreateTranslation(mPosition);
	mWorldTransform = scaleMatrix * rotationMatrix * positionMatrix;
}

void Actor::OnUpdate(float deltaTime)
{
}

void Actor::ProcessInput(const Uint8* keyState)
{
	// TODO
	for (auto i : mComponents)
	{
		i->ProcessInput(keyState);
	}
	OnProcessInput(keyState);
}

Vector3 Actor::GetForward()
{
	Vector3 forward(cos(GetRotation()), sin(GetRotation()), 0.0f);
	return forward;
}

CollisionComponent* Actor::GetCollComp()
{
	return mColl;
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

void Actor::AddComponent(Component* c)
{
	mComponents.emplace_back(c);
	std::sort(mComponents.begin(), mComponents.end(), [](Component* a, Component* b) {
		return a->GetUpdateOrder() < b->GetUpdateOrder();
	});
}

void Actor::LoadMaps()
{
}