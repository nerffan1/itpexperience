#include "PlayerMove.h"
#include "Actor.h"
#include "Math.h"
#include "Player.h"
#include "SDL/SDL.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "HeightMap.h"
#include "PlayerUI.h"
#include "EnemyMove.h"
#include "Enemy.h"
#include "SDL/SDL_mixer.h"

PlayerMove::PlayerMove(Player* owner) : VehicleMove(owner)
{
	mPlayer = owner;
	Vector3 iPos = mOwner->GetGame()->GetHeightMap()->CellToWorld(39, 58);
	mOwner->SetPosition(iPos);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{

	if (mOwner->GetState() == ActorState::Active)
	{
		//Acceleration
		if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])
		{
			mAccel = true;
		}
		else
		{
			mAccel = false;
		}

		//Steering
		int steer = 0;
		if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])
		{
			steer--;
		}
		if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT])
		{
			steer++;
		}
		mDir = (steerDir)steer;
	}
}

void PlayerMove::CheckBoundary()
{
	/*
	bool RightBound = mOwner->GetPosition().y > BoundY;
	bool LeftBound = mOwner->GetPosition().y < -BoundY;
	bool TopBound = mOwner->GetPosition().z > BoundZ;
	bool BottomBound = mOwner->GetPosition().z < -BoundZ;
	Vector3 newPos = mOwner->GetPosition();

	if (TopBound)
	{
		newPos.z = BoundZ;
		mOwner->SetPosition(newPos);
	}
	if (BottomBound)
	{
		newPos.z = -BoundZ;
		mOwner->SetPosition(newPos);
	}
	if (LeftBound)
	{
		newPos.y = -BoundY;
		mOwner->SetPosition(newPos);
	}
	if (RightBound)
	{
		newPos.y = BoundY;
		mOwner->SetPosition(newPos);
	}
	*/
}

void PlayerMove::ShootControls(const Uint8* keyState)
{
	bool input;
	if (keyState[SDL_SCANCODE_SPACE])
	{
		input = true;
	}
	else
	{
		input = false;
	}

	LastInput = input;
}

void PlayerMove::CheckCollisions()
{
	//COLLISION
	CollisionComponent* enemyCol = mOwner->GetGame()->GetEnemyColl();
	if (mOwner->GetCollComp()->Intersect(enemyCol))
	{
		mVelocity = Vector3::Zero;
		mAccel = false;
	}
}

void PlayerMove::SpeedUpdate(float deltatime)
{
}

void PlayerMove::OnLapChange(int newLap)
{
	PlayerUI* UI = mPlayer->GetComponent<PlayerUI>();
	if (newLap == 5)
	{
		Enemy* rival = mOwner->GetGame()->GetEnemy();
		EnemyMove* enemyMov = rival->GetComponent<EnemyMove>();
		int enemyLap = enemyMov->mCurrentLap;
		if (newLap < enemyLap)
		{
			Mix_FadeOutChannel(mfastMusic, 250);
			UI->SetRaceState(UI->Lost);
		}
		else
		{
			Mix_FadeOutChannel(mfastMusic, 250);
			UI->SetRaceState(UI->Won);
		}
		Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/Won.wav"), 0);
		mPlayer->SetState(ActorState::Paused);
		rival->SetState(ActorState::Paused);
	}
	else
	{
		UI->OnLapChange(newLap);
		if (newLap == 4)
		{
			Mix_FadeOutChannel(mOwner->GetGame()->GetMusicChannel(), 250);
			Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/FinalLap.wav"), 0);
			mfastMusic = Mix_FadeInChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/MusicFast.ogg"), -1, 4000);
		}
	}
}

void PlayerMove::Update(float deltaTime)
{
	if (mOwner->GetState() == ActorState::Active)
	{
		//CheckCollisions();  //A very primitive collision prototype
		VehicleMove::Update(deltaTime);
	}
}
