#include "Skeleton.h"
#include "CollisionComponent.h"
#include "AnimatedSprite.h"
#include "Game.h"
#include "EnemyComponent.h"
#include "Random.h"
#include "FireBall.h"
#include "Math.h"
#include "Player.h"

Skeleton::Skeleton(Vector2 iPos, Game* game) : 
	Actor(game)
	,mCoolDown(Random::GetFloatRange(1.0f, 1.5f))
	,mShootInterval(4.0f)
{
	SetPosition(iPos);

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(25.0f, 25.0f);

	//Animated Component
	mSprite = new AnimatedSprite(this);
	LoadAnimation();
	mSprite->SetAnimation("skel");
	mSprite->SetIsVisible(true);

	//Move Component
	mEnemyC = new EnemyComponent(this);
}

void Skeleton::LoadAnimation()
{
	std::vector<struct SDL_Texture*> SkeltAnim;
	SkeltAnim.push_back(mGame->GetTexture("Assets/Skeleton0.png"));
	SkeltAnim.push_back(mGame->GetTexture("Assets/Skeleton1.png"));
	mSprite->AddAnimation("skel", SkeltAnim);
}

void Skeleton::OnUpdate(float deltatime)
{
	if (mState == ActorState::Active)
	{
		//Reactivate animation if paused
		if (mSprite->GetIsPaused())
		{
			OnSight(true);
		}

		//Shoot fireballs
		GenerateFireball(deltatime);
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

void Skeleton::GenerateFireball(float& deltatime)
{
	if (mCoolDown > 0.0f)
	{
		mCoolDown -= deltatime;
	}
	else
	{
		//Wait for timer before shooting
		if (mShootInterval > 0.0f)
		{
			mShootInterval -= deltatime;
		}
		//Shoot
		else
		{
			//Find Player Direction
			FireBall* newBall = new FireBall(mPosition, mGame);
			
			//Reset Cooltime
			mCoolDown = Random::GetFloatRange(1.0f, 3.0f);
			mShootInterval = 4.0f;
		}
	}

}

void Skeleton::OnSight(bool b)
{
	mSprite->SetIsPaused(!b);
	mSprite->SetIsVisible(b);
}
