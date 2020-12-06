#include "Bat.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "EnemyComponent.h"
#include "Random.h"
#include "Game.h"
#include "Collider.h"

Bat::Bat(Vector2 iPos, Game* game) : Actor(game)
{
	SetPosition(iPos);

	//Collision Component 
	mColl = new CollisionComponent(this);
	mColl->SetSize(25.0f, 25.0f);

	//Enemy Component
	mEnemyC = new EnemyComponent(this);

	//Animated Component
	mSprite = new AnimatedSprite(this);
	LoadAnimation();
	mSprite->SetAnimation("fly");

	//Initialize Parameters
	Vector2 mini(-3.0f, -2.0f);
	Vector2 maxi(2.0f, 1.0f);
	min = mini;
	max = maxi; // Why cant I initialize these in header file?
	mFlyDir = Random::GetVector(min, max);
}

void Bat::OnUpdate(float deltatime)
{
	if (mState == ActorState::Active)
	{
		//Reactivate animation if paused
		if (mSprite->GetIsPaused())
		{
			OnSight(true);
		}

		//Move and Check Colliders
		Movement(deltatime);
		CheckColliders();
	}
	else
	{
		//Deactivate animation if active
		if (!mSprite->GetIsPaused())
		{
			OnSight(false);
		}
	}

}

void Bat::LoadAnimation()
{
	std::vector<struct SDL_Texture*> BatFly;
	BatFly.push_back(mGame->GetTexture("Assets/Bat0.png"));
	BatFly.push_back(mGame->GetTexture("Assets/Bat1.png"));
	mSprite->AddAnimation("fly", BatFly);
}

void Bat::OnSight(bool inRoom)
{
	mSprite->SetIsPaused(!inRoom);
	mSprite->SetIsVisible(inRoom);
}

void Bat::Movement(float deltatime)
{
	if (mFlyTime > 0.0f)
	{
		Vector2 dl = mFlyDir * deltatime * BAT_SPEED;
		SetPosition(GetPosition() + dl);
		mFlyTime -= deltatime;
	}
	else
	{
		mFlyDir = Random::GetVector(min, max);
		mFlyTime = FLY_TIME;
	}
}

void Bat::CheckColliders()
{
	//Useful Variables 
	Vector2 offset;
	CollisionComponent* otherColl = nullptr;
	CollSide side = CollSide::None;

	//Iterate through colliders for collision 
	for (Collider* col : mGame->GetColliders())
	{
		otherColl = col->GetCollComp();
		side = GetCollComp()->GetMinOverlap(otherColl, offset);
		if (side != CollSide::None)
		{
			BAT_SPEED *= -1;
			Vector2 newPos = GetPosition() + offset; //Position Correction
			SetPosition(newPos);
		}
	}
}
