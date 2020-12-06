#include "Bullet.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Block.h"
#include "Player.h"

Bullet::Bullet(Vector3 pos, Vector3 normal, Game* game) : Actor(game)
{
	Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/Shoot.wav"), 0);
	mPosition = pos;
	mScale = 0.5f;

	//Collision Component 
	mColl = new CollisionComponent(this);
	mColl->SetSize(10, 10, 10);

	//Mesh Component
	mMesh = new MeshComponent(this);
	Mesh* newMesh = mGame->GetRenderer()->GetMesh("Assets/Sphere.gpmesh");
	mMesh->SetMesh(newMesh);
	mMesh->SetTextureIndex(2);

	//Move Component
	MoveComponent* mMov = new MoveComponent(this);
	mMov->SetNormal(normal);
	float shipMov = mGame->GetPlayer()->GetComponent<MoveComponent>()->GetForwardSpeed();
	mMov->SetForwardSpeed(shipMov + 500.0f);
}

void Bullet::OnUpdate(float deltaTime)
{
	CheckLife(deltaTime);
	if (mState == ActorState::Active)
	{
		CheckCollision();
	}
}

void Bullet::CheckCollision()
{
	for (size_t  i = 0; i < 2; i++) //Loop fir. Where the bullets are most likely to be
	{
		for (Block* obs : mGame->GetBlockLayers()[i])
		{
			CollisionComponent* other = obs->GetCollComp();
			//If they collide, then explode the block
			if (mColl->Intersect(other))
			{

				if (obs->mType == type::exploding)
				{
					Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
					obs->Explode();
				}
				SetState(ActorState::Destroy);
			}
		}
	}
}

void Bullet::CheckLife(float& deltatime)
{
	mLifeTime -= deltatime;
	if (mLifeTime <= 0.0f)
	{
		mState = ActorState::Destroy;
	}
}
