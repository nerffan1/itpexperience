#include "PlayerMove.h"
#include "Actor.h"
#include "Math.h"
#include "Player.h"
#include "SDL/SDL.h"
#include "Game.h"
#include "Renderer.h"
#include "Bullet.h"
#include "Block.h"
#include "CollisionComponent.h"

PlayerMove::PlayerMove(Player* owner) : MoveComponent(owner)
{
	mPlayer = owner;
	mForwardSpeed = 400.0f;
	mNormal = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 vel = mNormal * mForwardSpeed;
	mVelocity = vel;
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	mVelocity.y = 0.0f;
	mVelocity.z = 0.0f;
	if (mOwner->GetState() == ActorState::Active)
	{
		//Movement
		if (keyState[SDL_SCANCODE_A])
		{
			mVelocity.y -= PLANAR_VEL;
		}
		if (keyState[SDL_SCANCODE_D])
		{
			mVelocity.y += PLANAR_VEL;
		}
		if (keyState[SDL_SCANCODE_W])
		{
			mVelocity.z += PLANAR_VEL;
		}
		if (keyState[SDL_SCANCODE_S])
		{
			mVelocity.z -= PLANAR_VEL;
		}

		//Shoot Controls
		ShootControls(keyState);
	}
}

void PlayerMove::CheckBoundary()
{
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
}

void PlayerMove::UpdateCamera()
{
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos.x -= HDist;
	cameraPos.z += VDist;

	Vector3 cameraTarget = mOwner->GetPosition();
	cameraPos.x += TargetD;

	//Set View Matrix
	Matrix4 ViewMatrix = Matrix4::CreateLookAt(
		cameraPos,
		cameraTarget,
		Vector3::UnitZ
	);
	mPlayer->GetGame()->GetRenderer()->SetViewMatrix(ViewMatrix);
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

	//Check Lading Edge
	if (input && !LastInput)
	{
		Vector3 iPos = mPlayer->GetPosition();
		Vector3 norm = mPlayer->GetComponent<MoveComponent>()->GetNormal();
		Bullet* newBull = new Bullet(iPos, norm, mOwner->GetGame());
	}
	LastInput = input;
}

void PlayerMove::CheckCollisions()
{
	for (std::vector<Block*> layer : mOwner->GetGame()->GetBlockLayers())
	{
		for (Block* blok : layer)
		{
			CollisionComponent* PColl = mOwner->GetComponent<CollisionComponent>();
			if (PColl->Intersect(blok->GetCollComp()))
			{
				mOwner->SetState(ActorState::Paused);
				Mix_PlayChannel(-1, mOwner->GetGame()->GetSound("Assets/Sounds/ShipDie.wav"), 0);
			}
		}
	}
}

void PlayerMove::SpeedUpdate(float deltatime)
{
	SpeedUpTimer -= deltatime;
	if (SpeedUpTimer <= 0.0f)
	{
		SpeedMult += 0.15f;
		SpeedUpTimer = 10.0f;
	}
}

void PlayerMove::Update(float deltaTime)
{
	if (mOwner->GetState() == ActorState::Active)
	{
		//Update Position
		Vector3 dl = mVelocity * SpeedMult * deltaTime;
		Vector3 newPos = mOwner->GetPosition() + dl;
		mOwner->SetPosition(newPos);

		//Check Collision 
		CheckBoundary();
		UpdateCamera();
		CheckCollisions();
		SpeedUpdate(deltaTime);
	}
	else
	{
		Mix_HaltChannel(mOwner->GetGame()->GetMusicChannel());
	}
}
