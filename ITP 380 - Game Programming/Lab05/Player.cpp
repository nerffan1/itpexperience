#include "Player.h"
#include "SpriteComponent.h"
#include "AnimatedSprite.h"
#include "CollisionComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PlayerMove.h"
#include "SDL/SDL_mixer.h"

Player::Player(Game* game) : Actor(game)
{
	//Sprite/Animation Component 
	mSprite = new AnimatedSprite(this, 200);
	LoadAnimations();
	mSprite->SetIsVisible(true);
	mSprite->SetAnimation("walk up");
	mSprite->SetIsPaused(true);
	this->SetScale(1);

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(20, 20);

	//Move Component
	mMov = new PlayerMove(this);
}

void Player::OnUpdate(float deltatime)
{
	if (mDead == true)
	{
		Death(deltatime);
	}
	else
	{
		UpdateAnimation(deltatime);
	}
}

void Player::LoadAnimations()
{
	std::vector<struct SDL_Texture*> anim;

	//Up
	anim.push_back(mGame->GetTexture("Assets/Link/WalkUp0.png"));
	anim.push_back(mGame->GetTexture("Assets/Link/WalkUp1.png"));
	mSprite->AddAnimation("walk up", anim);
	anim.clear();
	//Down
	anim.push_back(mGame->GetTexture("Assets/Link/WalkDown0.png"));
	anim.push_back(mGame->GetTexture("Assets/Link/WalkDown1.png"));
	mSprite->AddAnimation("walk down", anim);
	anim.clear();
	//Right
	anim.push_back(mGame->GetTexture("Assets/Link/WalkRight0.png"));
	anim.push_back(mGame->GetTexture("Assets/Link/WalkRight1.png"));
	mSprite->AddAnimation("walk right", anim);
	anim.clear();
	//left
	anim.push_back(mGame->GetTexture("Assets/Link/WalkLeft0.png"));
	anim.push_back(mGame->GetTexture("Assets/Link/WalkLeft1.png"));
	mSprite->AddAnimation("walk left", anim);
	anim.clear();
	//Attack left
	anim.push_back(mGame->GetTexture("Assets/Link/AttackLeft.png"));
	mSprite->AddAnimation("attack left", anim);
	anim.clear();
	//Attack right
	anim.push_back(mGame->GetTexture("Assets/Link/AttackRight.png"));
	mSprite->AddAnimation("attack right", anim);
	anim.clear();
	//Attack Up
	anim.push_back(mGame->GetTexture("Assets/Link/AttackUp.png"));
	mSprite->AddAnimation("attack up", anim);
	anim.clear();
	//Attack Down
	anim.push_back(mGame->GetTexture("Assets/Link/AttackDown.png"));
	mSprite->AddAnimation("attack down", anim);
	anim.clear();
}

void Player::KillPlayer()
{
	mDead = true;
}

void Player::Death(const float& deltatime)
{
	//Play sound only once, before pausing
	if (GetState() != ActorState::Paused)
	{
		Mix_HaltChannel(mGame->GetMusicChannel());
		mMov->SetForwardSpeed(0.0f);
		SetState(ActorState::Paused);
	}

	//Timed Death Animation
	if (mDeathTime > 0)
	{
		mSprite->SetAnimation("dead");
		mDeathTime -= deltatime;
	}
	else
	{
		SetState(ActorState::Destroy);
	}
}

void Player::UpdateAnimation(float& deltaTime)
{
	if (mMov->GetAttackTime() > 0.0f)
	{
		SetAttackAnimation();
	}
	else 
	{
		SetMovementAnimation();
	}
	if (mMov->GetStationary())
	{
		mSprite->SetIsPaused(true);
	}
}

void Player::SetAttackAnimation()
{
	if (mMov->GetSwordEdge())
	{
		mSprite->SetIsPaused(false);
		if (GetDirection() == Direction::Right)
		{
			mSprite->SetAnimation("attack right", true);
		}
		else if (GetDirection() == Direction::Left)
		{
			mSprite->SetAnimation("attack left", true);
		}
		else if (GetDirection() == Direction::Up)
		{
			mSprite->SetAnimation("attack up", true);
		}
		else if (GetDirection() == Direction::Down)
		{
			mSprite->SetAnimation("attack down", true);
		}
	}
}

void Player::SetMovementAnimation()
{
	mSprite->SetIsPaused(false);
	if (GetDirection() == Direction::Right)
	{
		mSprite->SetAnimation("walk right");
	}
	else if (GetDirection() == Direction::Left)
	{
		mSprite->SetAnimation("walk left");
	}
	else if (GetDirection() == Direction::Up)
	{
		mSprite->SetAnimation("walk up");
	}
	else if (GetDirection() == Direction::Down)
	{
		mSprite->SetAnimation("walk down");
	}
}


