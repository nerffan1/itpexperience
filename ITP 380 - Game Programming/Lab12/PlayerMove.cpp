#include "PlayerMove.h"
#include "Actor.h"
#include "Math.h"
#include "Player.h"
#include "SDL/SDL.h"
#include "Coin.h"
#include "HUD.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "SDL/SDL_mixer.h"
#include "CameraComponent.h"
#include <string>
#include "Checkpoint.h"
#include "SDL/SDL_mixer.h"

PlayerMove::PlayerMove(Player* owner) : MoveComponent(owner)
{
	mPlayer = owner;
	SetState(Falling);
	Mix_Chunk* steps = mOwner->GetGame()->GetSound("Assets/Sounds/Running.wav");
	mRunningSFX = Mix_PlayChannel(-1, steps, -1);
	Mix_Pause(mRunningSFX);
}

PlayerMove::~PlayerMove()
{
	Mix_HaltChannel(mRunningSFX);
}

//Control
void PlayerMove::ProcessInput(const Uint8* keyState)
{
	CameraControl();
	WASDMovement(keyState);
}
void PlayerMove::FallRespawn()
{
	if (mOwner->GetPosition().z < -750.0f)
	{
		mPlayer->SetPosition(mPlayer->GetRespawn());
		mPlayer->SetRotation(0.0f);
		mVel = Vector3::Zero;
		mState = Falling;
	}
}
void PlayerMove::CameraControl()
{
	int x, y;
	//For x, change the Rotation through the angular speed
	SDL_GetRelativeMouseState(&x, &y);
	SetAngularSpeed((((float)x) / 350.0f) * Math::Pi * 10.0f);

	//For y, change through camera 
	CameraComponent* myCam = mOwner->GetComponent<CameraComponent>();
	myCam->SetPitchSpeed((((float)y) / 350.0f) * Math::Pi * 10.0f);
}
void PlayerMove::WASDMovement(const Uint8* keyState)
{
	if (mOwner->GetState() == ActorState::Active && (mState == OnGround || mState == WallClimb))
	{
		//Movement
		if (keyState[SDL_SCANCODE_A])
		{
			AddForce(mOwner->GetRight() * -700.0f);
		}
		if (keyState[SDL_SCANCODE_D])
		{
			AddForce(mOwner->GetRight() * 700.0f);
		}
		if (keyState[SDL_SCANCODE_W])
		{
			AddForce(mOwner->GetForward() * 700.0f);
		}
		if (keyState[SDL_SCANCODE_S])
		{
			AddForce(mOwner->GetForward() * -700.0f);
		}

		if (keyState[SDL_SCANCODE_SPACE] && !LastInput)
		{
			//Play Sound
			Mix_Chunk* CoinSound = mOwner->GetGame()->GetSound("Assets/Sounds/Jump.wav");
			Mix_PlayChannel(-1, CoinSound, 0);

			AddForce(JUMP_FORCE);
			SetState(Jump);
		}
		LastInput = keyState[SDL_SCANCODE_SPACE];
	}
}

//Movement
void PlayerMove::FixXYVelocity()
{
	Vector2 planarVel(mVel.x, mVel.y);
	Vector2 planarAcc(mAccel.x, mAccel.y);
	//Limit Velocity
	if (planarVel.Length() >= MAX_VEL)
	{
		planarVel = Vector2::Normalize(planarVel) * MAX_VEL;
	}

	//Apply friction on Ground
	if ((mState == OnGround || mState == WallClimb) && mVel.Length() != 0.0f)
	{
		if (Math::NearZero(mAccel.x))
		{
			planarVel.x *= BRAKING_FACTOR;
		}
		if (Math::NearZero(mAccel.y))
		{
			planarVel.y *= BRAKING_FACTOR;
		}

		//Make opposite movement more responsive
		if (mAccel.Length() > 0.0f) //Apply only when there's a force is being applied (i.e. the keys are pressed)
		{
			bool a = planarVel.x > 0 && planarAcc.x < 0;
			bool b = planarVel.x < 0 && planarAcc.x > 0;
			bool c = planarVel.y > 0 && planarAcc.y < 0;
			bool d = planarVel.y < 0 && planarAcc.y > 0;

			if (a || b)
			{
				planarVel.x *= BRAKING_FACTOR;
			}
			if (c || d)
			{
				planarVel.y *= BRAKING_FACTOR;
			}
		}
	}

	//If planarVel is too close to 0, make it zero
	if (Math::NearZero(planarVel.Length()))
	{
		planarVel = Vector2::Zero;

	}
	//Update mVel
	mVel.x = planarVel.x;
	mVel.y = planarVel.y;
}
void PlayerMove::AddForce(const Vector3& force)
{
	mPendingForces += force;
}

//Collision
CollSide PlayerMove::FixCollision(CollisionComponent* self, CollisionComponent* block)
{
	Vector3 offset;
	CollSide side = mOwner->GetCollComp()->GetMinOverlap(block, offset);
	if (side == CollSide::None)
	{
		return side;
	}
	else
	{
		Vector3 fixPos = mOwner->GetPosition();
		fixPos += offset;
		if (side == CollSide::Top)
		{
			offset.z -= 0.9f;
		}
		mOwner->SetPosition(fixPos);
		return side;
	}
}
void PlayerMove::CheckCollision()
{
	//Check Block Collisions
	std::vector<Actor*> blocks = mOwner->GetGame()->GetBlocks();
	bool Check = true;
	CollSide HelperVariable = CollSide::None; //This helps us know if there's no collision at all

	//Loop Through all the blocks
	for (Actor* i : blocks)
	{
		CollisionComponent* blokCol = i->GetCollComp();
		CollSide side = FixCollision(mOwner->GetCollComp(), blokCol);
		bool OnSideX = (side == CollSide::SideX1) || (side == CollSide::SideX2);
		bool OnSideY = (side == CollSide::SideY1) || (side == CollSide::SideY2);

		//For collisions with objects during movement
		if (side == CollSide::Top && Check)
		{
			if (mState == Falling)
			{
				//Play Sound
				Mix_Chunk* LandSound = mOwner->GetGame()->GetSound("Assets/Sounds/Land.wav");
				Mix_PlayChannel(-1, LandSound, 0);
			}
			mVel.z = 0.0f;
			SetState(OnGround);
			Check = false; //Don't check other cases since they're for player on air
			HelperVariable = side;
		}
		else if (side == CollSide::Bottom && mState == Jump && Check)
		{
			mVel.z = 0.0f;
			Check = false;
			HelperVariable = side;
		}

		//For climbing
		if ((OnSideX || OnSideY))
		{
			AddForce(SideNormal(side) * 700.0f); //Normal Force
			SideOfCollision = side;

			if (CanWallClimb(side) && (mState == OnGround || mState == Jump))
			{
				mClimbingTimer = 0.0f;
				SetState(WallClimb);
				Check = false;
			}
			else if (CanWallRun(side) && mState == Jump)
			{
				mWallRunTimer = 0.0f;
				SetState(WallRun);
				Check = false;
			}
		}

		//Climbing State
		if (mState == WallClimb)
		{
			//The conditionals for this state is slightly more complex becasue I must wait for all
			//Blocks to be checked to make sure that no side is being touched
			if (side == CollSide::Top || side == CollSide::Bottom)
			{
				SetState(Falling);
				mVel.z = 0.0f;
			}
			else if ((OnSideX || OnSideY))
			{
				HelperVariable = side;
			}
		}

	}
	if (HelperVariable == CollSide::None && (mState == WallClimb || mState == OnGround) && Check)
	{
		SetState(Falling);
		mVel.z = 0.0f;
	}

	//Check Checkpoint collision
	CheckCP();
	CheckCoins();
}
void PlayerMove::CheckCP()
{
	Checkpoint* CurrentCP = mOwner->GetGame()->GetCheckpoint();
	if (CurrentCP != nullptr)
	{
		CollisionComponent* mCPColl = CurrentCP->GetCollComp();
		if (mOwner->GetCollComp()->Intersect(mCPColl))
		{
			//Play Sound
			Mix_Chunk* CPSound = mOwner->GetGame()->GetSound("Assets/Sounds/Checkpoint.wav");
			Mix_PlayChannel(-1, CPSound, 0);

			//Change Respawn point and Destroy
			mPlayer->SetRespawn(CurrentCP->GetPosition());
			CurrentCP->SetState(ActorState::Destroy);

			//Display Text
			mOwner->GetGame()->GetHUD()->SetCPColl(true);
			mOwner->GetGame()->GetHUD()->SetCPString(CurrentCP->GetTextString());

			//Change level if mCPColl is the last in the level
			if (!CurrentCP->GetLevelString().empty())
			{
				mOwner->GetGame()->SetNextLevel(CurrentCP->GetLevelString());
			}
		}
	}
}
void PlayerMove::CheckCoins()
{
	std::vector<Coin*> Coins = mOwner->GetGame()->GetCoins();

	if (!Coins.empty())
	{
		for (Coin* cns : Coins)
		{
			if (mOwner->GetCollComp()->Intersect(cns->GetCollComp()))
			{
				//Play Sound
				Mix_Chunk* CoinSound = mOwner->GetGame()->GetSound("Assets/Sounds/Coin.wav");
				Mix_PlayChannel(-1, CoinSound, 0);
				mOwner->GetGame()->GetHUD()->AddCoin(); //Update HUD 

				//Destroy
				cns->SetState(ActorState::Destroy);
			}
		}
	}
}

//Climbing
bool PlayerMove::CanWallClimb(CollSide wallside)
{
	//The player must face with 30 degrees of the wall's normal
	float DotProd = Vector3::Dot(mOwner->GetForward(), SideNormal(wallside));
	bool FacingBlock = DotProd >= -1 && DotProd <= (-sqrt(3) / 2);

	//The player's velocity is within 30 degrees of the wall's normal
	Vector3 VelNormal = Vector3::Normalize(mVel);
	DotProd = Vector3::Dot(VelNormal, SideNormal(wallside));
	bool VelToBlock = DotProd >= -1 && DotProd <= (-sqrt(3) / 2);

	//Velocity is enough if it's 350.0f
	bool EnoughVel = mVel.Length() > 350.0f;

	return FacingBlock && VelToBlock && EnoughVel;
}
Vector3 PlayerMove::SideNormal(CollSide wallside)
{
	if (wallside == CollSide::SideX1)
	{
		return -1 * Vector3::UnitX;
	}
	else if (wallside == CollSide::SideX2)
	{
		return Vector3::UnitX;
	}
	else if (wallside == CollSide::SideY1)
	{
		return -1 * Vector3::UnitY;
	}
	else if (wallside == CollSide::SideY2)
	{
		return Vector3::UnitY;
	}
	return Vector3();
}

//Wall Running
bool PlayerMove::CanWallRun(CollSide wallside)
{
	//The player must face with 30 degrees of the wall's normal
	float DotProd = Vector3::Dot(mOwner->GetForward(), SideNormal(wallside));
	bool FacingBlock = DotProd <= 0.0f && DotProd >= (-sqrt(2) / 2);

	//The player's velocity is within 30 degrees of the wall's normal
	Vector3 VelNormal = Vector3::Normalize(mVel);
	DotProd = Vector3::Dot(VelNormal, SideNormal(wallside));
	bool VelToBlock = DotProd <= 0.0f && DotProd >= (-sqrt(2) / 2);

	//Velocity is enough if it's 350.0f
	bool EnoughVel = mVel.Length() > 350.0f;

	return FacingBlock && VelToBlock && EnoughVel;
}

//Update
void PlayerMove::Update(float deltaTime)
{
	switch (mState)
	{
	case Falling:
		UpdateFalling(deltaTime);
		break;
	case Jump:
		UpdateJump(deltaTime);
		break;
	case OnGround:
		UpdateOnGround(deltaTime);
		break;
	case WallClimb:
		UpdateWallClimb(deltaTime);
		break;
	case WallRun:
		UpdateWallRun(deltaTime);
		break;
	}
	PhysicsUpdate(deltaTime);
	CheckCollision();
	UpdateSFX();
	FallRespawn();
}
void PlayerMove::UpdateOnGround(float& deltaTime)
{
}
void PlayerMove::UpdateJump(float& deltaTime)
{
	AddForce(GRAVITY);
	if (mVel.z < 0.0f)
	{
		SetState(Falling);
	}
}
void PlayerMove::UpdateFalling(float& deltaTime)
{
	AddForce(GRAVITY);
}
void PlayerMove::UpdateWallClimb(float& deltaTime)
{
	//Update Timer - AddForces - Conditionals
	mClimbingTimer += deltaTime;
	AddForce(GRAVITY);
	if (mClimbingTimer < 0.4)
	{
		AddForce(CLIMB);
	}
	if (mVel.z < 0.0f)
	{
		SetState(Falling);
		mVel.z = 0.0f;
	}
}
void PlayerMove::UpdateWallRun(float& deltaTime)
{
	//Update Timer - AddForces - Conditionals
	mWallRunTimer += deltaTime;
	AddForce(GRAVITY);
	if (mWallRunTimer < 0.4)
	{
		AddForce(WALLRUN_FORCE);
	}
	if (mVel.z < 0.0f)
	{
		SetState(Falling);
		mVel.z = 0.0f;
	}
}
void PlayerMove::UpdateSFX()
{
	if (mState == OnGround)
	{
		if (mVel.Length() > 50.0f)
		{
			Mix_Resume(mRunningSFX);
		}
		else
		{
			Mix_Pause(mRunningSFX);
		}
	}
	else if (mState == WallClimb || mState == WallRun)
	{
		Mix_Resume(mRunningSFX);
	}
	else
	{
		Mix_Pause(mRunningSFX);
	}
}
void PlayerMove::PhysicsUpdate(float deltaTime)
{
	//Update Accel and Vel
	mAccel = mPendingForces * (1.0f / mMass);
	mVel += mAccel * deltaTime;
	FixXYVelocity();

	//Update Linear Motion
	Vector3 dl = mVel * deltaTime;
	mOwner->SetPosition(mOwner->GetPosition() + dl);

	//Update Rotation
	float rot = mAngularSpeed * deltaTime;
	mOwner->SetRotation(mOwner->GetRotation() + rot);

	//Make all forces 0 
	mPendingForces = Vector3::Zero;
}
