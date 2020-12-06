#include "Goomba.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "GoombaMove.h"
#include "Game.h"
#include "Actor.h"

Goomba::Goomba(Game* game, Vector2 iPos) : Actor(game)
{
	//Add to vector 
	mGame->AddGoomba(this);

	//Set Position
	SetPosition(iPos);

	//Animation
	mSprite = new AnimatedSprite(this,100);
	LoadAnimation();
	mSprite->SetIsVisible(true);

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(32, 32);

	//Move Component
	mMov = new GoombaMove(this);
}

void Goomba::Stompage()
{
	mStomped = true;
}

bool Goomba::GetStompState()
{
	return mStomped;
}

void Goomba::LoadAnimation()
{
	mWalk.push_back(mGame->GetTexture("Assets/Goomba/Walk0.png"));
	mWalk.push_back(mGame->GetTexture("Assets/Goomba/Walk1.png"));
	mSprite->AddAnimation("WalkGoomba", mWalk);
	mSprite->SetAnimation("WalkGoomba");
}

void Goomba::DeathAnimation()
{
	mDeath.push_back(mGame->GetTexture("Assets/Goomba/Dead.png"));
	mSprite->AddAnimation("Dead", mDeath);
	mSprite->SetAnimation("Dead",true);
}

void Goomba::ReverseDirection()
{
	mMov->SetForwardSpeed(mMov->GetForwardSpeed()*-1);
}

float Goomba::GetForwardSpeed()
{
	return mMov->GetForwardSpeed();
}

void Goomba::SetForwardSpeed(float speed)
{
	mMov->SetForwardSpeed(speed);
}

Goomba::~Goomba()
{
	mGame->RemoveGoomba(this);
}

void Goomba::OnUpdate(float deltaTime)
{
	//Start counting down
	if (mStomped)
	{
		if (mDeathTime == DEATH_ANIM) //Only change the texture once 
		{
			DeathAnimation();
		}
		mDeathTime -= deltaTime;
		//Animation timed for .25 seconds
		if (mDeathTime < 0)
		{
			SetState(ActorState::Destroy);
		}
	}
	else if (mPosition.y > FALL_DEATH)
	{
		SetState(ActorState::Destroy);
	}

}
