#include "PlayerMove.h"
#include "Player.h"
#include "SDL/SDL_keyboard.h"
#include "Math.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Collider.h"
#include "SDL/SDL_mixer.h"
#include "Door.h"
#include "Sword.h"
#include "SecretBlock.h"

PlayerMove::PlayerMove(Player* owner) : MoveComponent(owner)
, mAttackTime(0.0f)
{
	mPlayer = owner;

	//Create Sword 
	mSword = new Sword(mOwner->GetGame());
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	//Check if Space Edge
	SwordEdge(keyState);

	//Check for movement 
	if (mPlayer->GetState() != ActorState::Paused)
	{
		mStationary = false;
		//Movement
		if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT])
		{
			mPlayer->SetDirection(Direction::Left);
		}
		else if (keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT])
		{
			mPlayer->SetDirection(Direction::Right);
		}
		else if (keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN])
		{
			mPlayer->SetDirection(Direction::Up);
		}
		else if (keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP])
		{
			mPlayer->SetDirection(Direction::Down);
		}
		else
		{
			mStationary = true;
		}
	}
}

void PlayerMove::Update(float deltaTime)
{
	//Update Camera
	UpdateMovement(deltaTime);

	//Check for other actors
	CheckDoors();
	CheckColliders();
	CheckSecretBlocks();

	//Update sword 
	UpdateSword(deltaTime);

	//Update Camera
	UpdateCamera();
}


void PlayerMove::Teleport()
{
	Vector2 Teleport = mPlayer->GetdirVec() * TELEPORT;
	mPlayer->SetPosition(mPlayer->GetPosition() + Teleport);
}

void PlayerMove::UpdateCamera()
{
	//Move camera so that link is in center frame
	Vector2 offset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	Vector2 updateCamera(mOwner->GetPosition() - offset);
	mOwner->GetGame()->SetCamera(updateCamera);
}

void PlayerMove::UpdateMovement(float& deltaTime)
{
	if (!mStationary && mAttackTime <= 0.0f)
	{
		Vector2 newPosition = mPlayer->GetPosition() + (mPlayer->GetdirVec() * deltaTime * SPEED);
		mPlayer->SetPosition(newPosition);
	}

}

void PlayerMove::UpdateSword(float& deltatime)
{
	if (mAttackTime > 0.0f)
	{
		if (mSwordEdge) //Only set sprite for the first time
		{
			//Set the sprite and coordinates of sword
			SetSword();

			//Check For Collisions only once
			mSword->SwordCollision();
		}

		//Set the Animation to be visible 
		mSword->SetVisible(true);
		mAttackTime -= deltatime;
	}
	else
	{
		mSword->SetVisible(false);
	}
}

void PlayerMove::SwordEdge(const Uint8* key)
{
	if (key[SDL_SCANCODE_SPACE])
	{
		//Jump Edge 
		if (mSwordEdge == false && mSpacePressed == false)
		{
			mSwordEdge = true;
			if (mAttackTime <= 0.0f)
			{
				mAttackTime = 0.25f;
			}
		}
		else
		{
			mSwordEdge = false;
		}
		mSpacePressed = true;
	}
	else
	{
		mSpacePressed = false;
	}
}

void PlayerMove::SetSword()
{
	//Set right sprite for Sword
	std::string filename = "Assets/Sword/";
	Direction p = mOwner->GetGame()->GetPlayer()->GetDirection();
	switch (p)
	{
	case Direction::Down:
	{
		filename += "Down.png";
		mSword->SetSprite(filename);
		Vector2 offset(8.0f, 16.0f);
		Vector2 newPos = mPlayer->GetPosition() + offset;
		mSword->SetPosition(newPos);
		break;
	}
	case Direction::Up:
	{
		filename += "Up.png";
		mSword->SetSprite(filename);
		Vector2 offset(-8.0f, -16.0f);
		Vector2 newPos = mPlayer->GetPosition() + offset;
		mSword->SetPosition(newPos);
		break;
	}
	case Direction::Right:
	{
		filename += "Right.png";
		mSword->SetSprite(filename);
		Vector2 offset(16.0f, 4.0f);
		Vector2 newPos = mPlayer->GetPosition() + offset;
		mSword->SetPosition(newPos);
		break;
	}
	case Direction::Left:
	{
		filename += "Left.png";
		mSword->SetSprite(filename);
		Vector2 offset(-16.0f, -4.0f);
		Vector2 newPos = mPlayer->GetPosition() + offset;
		mSword->SetPosition(newPos);
		break;
	}
	}
}

bool PlayerMove::GetSpacePressed()
{
	return mSpacePressed;
}

void PlayerMove::CheckColliders()
{
	//Useful Variables 
	Vector2 offset;
	CollisionComponent* otherColl = nullptr;
	CollSide side = CollSide::None;

	//Iterate through blocks for collision detection
	for (Collider* col : mOwner->GetGame()->GetColliders())
	{
		otherColl = col->GetCollComp();
		side = mOwner->GetCollComp()->GetMinOverlap(otherColl, offset);
		Vector2 newPos = mOwner->GetPosition() + offset; //Position Correction
		mOwner->SetPosition(newPos);
	}
}

void PlayerMove::CheckDoors()
{
	//Useful Variables 
	Vector2 offset;
	CollisionComponent* otherColl = nullptr;
	CollSide side = CollSide::None;

	//Iterate through blocks for collision detection
	for (Door* DoorCol : mOwner->GetGame()->GetDoors())
	{
		otherColl = DoorCol->GetCollComp();
		side = mOwner->GetCollComp()->GetMinOverlap(otherColl, offset);
		if (side != CollSide::None)
		{
			//Check for propper entrance
			bool ProperDir = DoorCol->GetDirection() == mPlayer->GetDirection();
			if (ProperDir && DoorCol->GetDoorState() == DoorState::Open)
			{
				//Set New Room
				mPlayer->GetGame()->SetNewRoom(DoorCol->GetDestination());

				//Teleport to next room
				Teleport();
			}
			else if ((ProperDir && DoorCol->GetDoorState() == DoorState::Locked) && mPlayer->GetKeys() > 0)
			{
				//Open door and teleport
				DoorCol->SetDoorState(DoorState::Open);
				DoorCol->UpdateTexture();
				mPlayer->RemoveKey();
				mPlayer->GetGame()->SetNewRoom(DoorCol->GetDestination());
				Teleport();
			}
		}
	}
}

void PlayerMove::CheckSecretBlocks()
{
	//Useful Variables 
	Vector2 offset;
	CollisionComponent* otherColl = nullptr;
	CollSide side = CollSide::None;

	//Iterate through blocks for collision detection
	for (SecretBlock* SBlok : mOwner->GetGame()->GetSBlocks())
	{
		otherColl = SBlok->GetCollComp();
		side = mOwner->GetCollComp()->GetMinOverlap(otherColl, offset);
		if (side != CollSide::None)
		{
			Vector2 newPos = mPlayer->GetPosition() + offset;
			mPlayer->SetPosition(newPos);
			//Check for propper direction
			if (SBlok->GetDirection() == mPlayer->GetDirection() && SBlok->IsLocked())
			{
				SBlok->Contact(true);
			}
		}
	}
}
