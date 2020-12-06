#include "PlayerMove.h"
#include "Player.h"
#include "SDL/SDL_keyboard.h"
#include "Math.h"
#include "Actor.h"
#include "Block.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Goomba.h"
#include "SDL/SDL_mixer.h"

PlayerMove::PlayerMove(Player* owner) : MoveComponent(owner)
{
	mPlayer = owner;
	mSpacePressed = false;
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	if (mPlayer->GetState() != ActorState::Paused)
	{
		//Jumping
		if (keyState[SDL_SCANCODE_SPACE])
		{	
			//Jump Edge 
			if (mJumpEdge == false && mSpacePressed == false)
			{
				mJumpEdge = true;
			}
			else
			{
				mJumpEdge = false;
			}
			mSpacePressed = true;
		}
		else
		{
			mSpacePressed = false;
		}

		//Movement
		if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT])
		{
			SetForwardSpeed(-SPEED);
		}
		else if (keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT])
		{
			SetForwardSpeed(SPEED);
		}
		else
		{
			SetForwardSpeed(0.0f);
		}
	}
}

void PlayerMove::Update(float deltaTime)
{
	if (mPlayer->GetState() != ActorState::Paused)
	{
		//Left Side of Camera conditional
		if (mOwner->GetCollComp()->GetMin().x <= mOwner->GetGame()->GetCamera().x && mForwardSpeed < 0)
		{
			mForwardSpeed = 0;
		}

		//Update new Coordinate
		Vector2 dl(mForwardSpeed * deltaTime, mYSpeed * deltaTime);
		Vector2 newPos = mOwner->GetPosition() + dl;
		mOwner->SetPosition(newPos);

		//Check for actors (ggomabs/blocks)
		CheckActors();

		// Update Jumping and Gravity
		VerticalMovement(deltaTime);

		//Update Camera
		UpdateCamera();
	}
}

void PlayerMove::Jump(float magnitude)
{
	mYSpeed = magnitude;
	mInAir = true;
}

void PlayerMove::VerticalMovement(float &deltaTime)
{
	//Jumping
	if (mJumpEdge == true && mInAir == false)
	{
		Jump(JUMP);
		mInAir = true;
		Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Jump.wav"), 0);
	}
	//Update Gravity 
	if (mInAir == true)
	{
		mYSpeed += ACCEL * deltaTime;
	}
	//Death by fall
	if (mOwner->GetPosition().y > FALL_DEATH)
	{
		mPlayer->PlayerDeath();
	}
}

void PlayerMove::UpdateCamera()
{
	//Update Camera
	if (mForwardSpeed > 0 && mOwner->GetPosition().x >= mOwner->GetGame()->GetCamera().x + 300.0f)
	{
		//Since camera doesn't change vertically, just add a change to the x-axis
		Vector2 updateCamera(mOwner->GetPosition().x - 300.0f, 0.0f);
		mOwner->GetGame()->SetCamera(updateCamera);
	}
}

bool PlayerMove::GetAirBool()
{
	return mInAir;
}

bool PlayerMove::GetSpacePressed()
{
	return mSpacePressed;
}

void PlayerMove::CheckActors()
{
	//Useful Variables 
	Vector2 offset;
	CollisionComponent* otherColl = nullptr;
	CollSide side = CollSide::None;

	//Iterate through blocks for collision detection
	for (Block* blok : mOwner->GetGame()->GetBlocks())
	{
		if (MarioXNeighbourhood(blok))
		{
			otherColl = blok->GetCollComp();
			side = mOwner->GetCollComp()->GetMinOverlap(otherColl, offset);
			switch (side)
			{
			case (CollSide::Top):
			{
				offset.y += 0.1f; //Slightly move him so that collisison doesn't become None
				mInAir = false;
				if (mYSpeed > 0) 
				{ 
					mYSpeed = 0.0f; 
				}
				break;
			}
			case (CollSide::Bottom):
			{
				Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Bump.wav"), 0);
				if (mYSpeed < 0) 
				{ 
					mYSpeed = 0.0f; 
				};
				break;
			}
			case (CollSide::None):
			{
				mInAir = true;
				break;
			}
			}
		
			//Position Correction
			Vector2 newPos = mOwner->GetPosition() + offset;
			mOwner->SetPosition(newPos);
		}
		if (side == CollSide::Top || side == CollSide::Bottom)
		{
			break;
		}
	}

	//Iterate through Goombas to see if they've been stomped
	for (Goomba* Genemy : mOwner->GetGame()->GetGoombas())
	{
		if (MarioXNeighbourhood(Genemy))
		{
			otherColl = Genemy->GetCollComp();
			side = mOwner->GetCollComp()->GetMinOverlap(otherColl, offset);

			//Destroy Goomba if you collide on top
			if (side == CollSide::Top)
			{
				Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
				mOwner->SetPosition(mOwner->GetPosition() + offset);
				Genemy->Stompage();
				Jump(-350.0f);
				break;
			}
			//Destroy Goomba if in air 
			else if (side == CollSide::Left || side == CollSide::Right)
			{
				if (mInAir && mPlayer->GetPosition().y < Genemy->GetPosition().y - 1.0f)
				{
					Mix_PlayChannel(-1, mPlayer->GetGame()->GetSound("Assets/Sounds/Stomp.wav"), 0);
					Genemy->Stompage();
					Jump(-350.0f);
					break;
				}
				else
				{
					//Kill Mario
					mPlayer->PlayerDeath();
					break;
				}
			}
		}

	}
}

bool PlayerMove::MarioXNeighbourhood(Actor* otherActor)
{
	return (otherActor->GetPosition().x > mOwner->GetPosition().x - mXBoundary && otherActor->GetPosition().x < mOwner->GetPosition().x + mXBoundary) ? true : false;
}

bool PlayerMove::MarioYNeighbourhood(Actor* otherActor)
{
	return (otherActor->GetPosition().y > mOwner->GetPosition().y - mYBoundary && otherActor->GetPosition().y < mOwner->GetPosition().y + mYBoundary) ? true : false;
}
