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
	mSprite->SetAnimation("idle");
	this->SetScale(1);

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(32, 32);

	//Move Component
	mMov = new PlayerMove(this);
}
void Player::OnUpdate(float deltatime)
{
	if (mDead == true) 
	{	
		//Play sound only once, before pausing
		if (GetState() != ActorState::Paused)
		{
		Mix_HaltChannel(mGame->GetMusicChannel());
		Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/scream.wav"), 0);
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
	else
	{
		UpdateAnimation(deltatime);
	}

	//Completed
	if (GetPosition().x > 6368 && GetState() != ActorState::Paused)
	{
		Mix_HaltChannel(mGame->GetMusicChannel());
		Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/StageClear.wav"), 0);
		SetState(ActorState::Paused);
	}
}

void Player::LoadAnimations()
{
	std::vector<struct SDL_Texture*> mRunR, mRunL;
	std::vector<struct SDL_Texture*> mJumpR, mJumpL;
	std::vector<struct SDL_Texture*> mDeadAnim, mIdle;
	//Set up and Add Idle Animation
	mIdle.push_back(mGame->GetTexture("Assets/Mario/Idle.png"));
	mSprite->AddAnimation("idle", mIdle);
	//Set up and Add Dead Animation
	for (int i = 1; i <= 30; i++)
	{
		std::string file = "Assets/Mario/DieMario-";
		file += std::to_string(i);
		file += ".png";
		mDeadAnim.push_back(mGame->GetTexture(file));
	}
	mSprite->AddAnimation("dead", mDeadAnim);
	//Set up and Add Run-Left 
	mRunL.push_back(mGame->GetTexture("Assets/Mario/RunLeft0.png"));
	mRunL.push_back(mGame->GetTexture("Assets/Mario/RunLeft1.png"));
	mRunL.push_back(mGame->GetTexture("Assets/Mario/RunLeft2.png"));
	mSprite->AddAnimation("run left", mRunL);
	//Set up and Add Run-Right Animation
	mRunR.push_back(mGame->GetTexture("Assets/Mario/RunRight0.png"));
	mRunR.push_back(mGame->GetTexture("Assets/Mario/RunRight1.png"));
	mRunR.push_back(mGame->GetTexture("Assets/Mario/RunRight2.png"));
	mSprite->AddAnimation("run right", mRunR);
	//Set up and Add Jump-Right Animation
	mJumpR.clear();
	mJumpR.push_back(mGame->GetTexture("Assets/Mario/JumpRight.png"));
	mSprite->AddAnimation("jump right", mJumpR);
	//Set up and Add Jump-Left Animation
	mJumpL.push_back(mGame->GetTexture("Assets/Mario/JumpLeft.png"));
	mSprite->AddAnimation("jump left", mJumpL);
}

void Player::PlayerDeath()
{
	mDead = true;
}

void Player::UpdateAnimation(float& deltaTime)
{
	//Running/Jumping Animations
	
	
	if (mMov->GetAirBool() == false)
	{
		if (mMov->GetForwardSpeed() > 0)
		{
			mSprite->SetAnimation("run right");
		}
		else if (mMov->GetForwardSpeed() < 0 )
		{
			mSprite->SetAnimation("run left");
		}
	}
	else
	{
		if (mMov->GetForwardSpeed() > 0 && mSprite->GetAnimName() != "jump left")
		{
			mSprite->SetAnimation("jump right");
		}
		else if (mMov->GetForwardSpeed() < 0 && mSprite->GetAnimName() != "jump right")
		{
			mSprite->SetAnimation("jump left");
		}
	}

	// In case of zero speed
	if (mMov->GetForwardSpeed() == 0.0f)
	{
		if (mMov->GetAirBool() == false)
		{
			mSprite->SetAnimation("idle");
		}
		else
		{
			if (mSprite->GetAnimName() == "idle")
			{
				mSprite->SetAnimation("jump right");
			}
		}
	}
}


