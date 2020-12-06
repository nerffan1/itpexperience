#pragma once
#include <vector>
#include <SDL/SDL_stdinc.h>
#include "Math.h"
#include <unordered_map>

enum class ActorState
{
	Active,
	Paused,
	Destroy
};

enum class Direction
{
	Up,
	Down,
	Left,
	Right,
	Stationary
};

class Actor
{
public:
	Actor(class Game* game);
	virtual ~Actor();

	// Update function called from Game (not overridable)
	void Update(float deltaTime);
	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const Uint8* keyState);

	// Getters/setters
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }
	void SetDirection(Direction newDir) { mDirection = newDir; }
	Vector3 GetForward();
	class CollisionComponent* GetCollComp();
	ActorState GetState() const { return mState; }
	void SetState(ActorState state) { mState = state; }
	class Game* GetGame() { return mGame; }
	class MeshComponent* GetMeshComp() { return mMeshComp;}
	void SetMeshComp(class MeshComponent* set) { mMeshComp = set; }

	//Additional Getters/Setters
	Direction GetDirection() { return mDirection; }

	// Adds component to Actor (this is automatically called
	// in the component constructor)
	void AddComponent(class Component* c);

	//Tool 
	void LoadMaps();

	// Returns component of type T, or null if doesn't exist
	template <typename T>
	T* GetComponent() const
	{
		for (auto c : mComponents)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
			{
				return t;
			}
		}

		return nullptr;
	}

	Direction mDirection;

	//3D Functions
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	//Movement Functions
	Vector3 GetRight();

protected:
	// Any actor-specific update code (overridable)
	virtual void OnUpdate(float deltaTime);
	// Any actor-specific update code (overridable)
	virtual void OnProcessInput(const Uint8* keyState);

	class Game* mGame;
	class CollisionComponent* mColl = nullptr;
	class MeshComponent* mMeshComp = nullptr;
	class Mesh* mMesh = nullptr;

	// Actor's state
	ActorState mState;

	// Transform
	Vector3 mPosition;
	float mScale;
	float mRotation;

	// Components
	std::vector<class Component*> mComponents;

	//3D Components
	Matrix4 mWorldTransform;
	
};
