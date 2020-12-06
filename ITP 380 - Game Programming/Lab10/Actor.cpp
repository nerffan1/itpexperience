#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include <algorithm>
#include "Game.h"
#include "CollisionComponent.h"

Actor::Actor(Game* game, Actor* parent)
	:mGame(game)
	, mState(ActorState::Active)
	, mPosition(Vector3::Zero)
	, mScale(1.0f)
	, mRotation(0.0f)
	, mParent(parent)
{
	LoadMaps();
	if (mParent == nullptr)
	{
		mGame->AddActor(this);
	}
	else
	{
		mParent->AddChild(this);
	}
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
	if (mParent == nullptr)
	{
		mGame->RemoveActor(this);
	}
	else
	{
		mParent->RemoveChild(this);
	}
}

void Actor::Update(float deltaTime)
{
	CalcWorldTransform();

	if (GetState() == ActorState::Active)
	{
		for (auto i : mComponents)
		{
			i->Update(deltaTime);
		}
		OnUpdate(deltaTime);
	}

	//3D Update 
	CalcWorldTransform();
	for (Actor* child : mChildren)
	{
		child->Update(deltaTime);
	}
}

Vector3 Actor::GetRight()
{
	return Vector3(-sin(mRotation), cos(mRotation), 0.0f);
}

void Actor::CalcWorldTransform()
{
	Matrix4 quatMatrix = Matrix4::CreateFromQuaternion(mQuat);
	Matrix4 scaleMatrix = Matrix4::CreateScale(mScale);
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
	Matrix4 positionMatrix = Matrix4::CreateTranslation(mPosition);
	mWorldTransform = scaleMatrix * rotationMatrix * quatMatrix * positionMatrix;

	if (mParent != nullptr)
	{
		if (mInheritScale)
		{
			mWorldTransform *= mParent->GetWorldTransform();
		}
		else
		{
			mWorldTransform *= mParent->CalcWorldRotTrans();
		}
	}
}

Matrix4 Actor::CalcWorldRotTrans()
{
	Matrix4 quatMatrix = Matrix4::CreateFromQuaternion(mQuat);
	Matrix4 rotationMatrix = Matrix4::CreateRotationZ(mRotation);
	Matrix4 positionMatrix = Matrix4::CreateTranslation(mPosition);
	if (mParent != nullptr)
	{
		return rotationMatrix * quatMatrix * positionMatrix * mParent->CalcWorldRotTrans();
	}
	else
	{
		return rotationMatrix * quatMatrix * positionMatrix;
	}
}

Vector3 Actor::GetWorldPosition()
{
	return mWorldTransform.GetTranslation();
}

Vector3 Actor::GetWorldForward()
{
	return mWorldTransform.GetXAxis();
}

void Actor::RemoveChild(Actor* ByeChild)
{
	auto iter = std::find(mChildren.begin(), mChildren.end(), ByeChild);
	if (iter != mChildren.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		auto iter2 = mChildren.end() - 1;
		std::iter_swap(iter, iter2);
		mChildren.pop_back();
	}
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
	return Vector3(cos(GetRotation()), sin(GetRotation()), 0.0f);
}

CollisionComponent* Actor::GetCollComp()
{
	return mColl;
}

void Actor::OnProcessInput(const Uint8* keyState)
{
}

Vector3 Actor::GetQuatForward()
{
	Vector3 forwardVec = Vector3::Transform(Vector3::UnitX, mQuat);
	forwardVec.Normalize();
	return forwardVec;
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