#include "SecretBlock.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Door.h"

SecretBlock::SecretBlock(Vector2 pos, std::string dir, std::string room, float width, float height, Game* game) : Actor(game)
{
	//Set Parameters
	SetPosition(pos);
	mDirection = mDirectionMap[dir];
	InvertDirection(); // This to make the comparison easier in PlayerMove

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(width, height);

	//Sprite Component 
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/SecretBlock.png"));
	mSprite->SetIsVisible(true);

	//Add to Game
	mGame->AddSecretBlock(this,room);
}

void SecretBlock::OnUpdate(float deltatime)
{
	ContactAnimation(deltatime);
}

void SecretBlock::ContactAnimation(float deltatime)
{
	if (mContact)
	{
		if (MAX_DISPL > 0.0f)
		{
			Vector2 Mov = mDirVector[mDirection] * UNLOCK_SPEED * deltatime;
			SetPosition(GetPosition() + Mov);
			MAX_DISPL -= UNLOCK_SPEED * deltatime;
		}
		else
		{
			for (Door* dor : mGame->GetDoors())
			{
				if (dor->GetDoorState() == DoorState::Closed)
				{
					dor->SetDoorState(DoorState::Open);
					dor->UpdateTexture();
				}
			}
		}
		mContact = false; //Set false so that block doesn't keep moving
	}
}

void SecretBlock::Unlock()
{
	mLocked = false;
}

void SecretBlock::InvertDirection()
{
	if (mDirection == Direction::Down)
	{
		mDirection = Direction::Up;
	}
	else if (mDirection == Direction::Up)
	{
		mDirection = Direction::Down;
	}
	else if (mDirection == Direction::Right)
	{
		mDirection = Direction::Left;
	}
	else if (mDirection == Direction::Left)
	{
		mDirection = Direction::Right;
	}
}

