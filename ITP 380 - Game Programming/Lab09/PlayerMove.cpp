#include "PlayerMove.h"
#include "Actor.h"
#include "Math.h"
#include "Player.h"
#include "SDL/SDL.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "SDL/SDL_mixer.h"
#include "CameraComponent.h"

PlayerMove::PlayerMove(Player* owner) : MoveComponent(owner)
{
	mPlayer = owner;
	SetState(Falling);
}

void PlayerMove::ProcessInput(const Uint8* keyState)
{
	CameraControl();
	WASDMovement(keyState);
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
	if (mOwner->GetState() == ActorState::Active && mState == OnGround)
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
			AddForce(JUMP_FORCE);
			SetState(Jump);
		}
		LastInput = keyState[SDL_SCANCODE_SPACE];
	}
}

void PlayerMove::FixXYVelocity()
{
	Vector2 planarVel(mVel.x, mVel.y);
	Vector2 planarAcc(mAccel.x, mAccel.y);
	//Limit Velocity
	if (planarVel.Length() >= MAX_VEL)
	{
		Vector2 planarVel = Vector2::Normalize(planarVel) * MAX_VEL;
	}

	//Apply friction on Ground
	if (mState == OnGround && mVel.Length() != 0.0f)
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

void PlayerMove::UpdateOnGround(float& deltaTime)
{
}

void PlayerMove::UpdateJump(float& deltaTime)
{
	AddForce(GRAVITY);
	if (mVel.z <= 0.0f)
	{
		SetState(Falling);
	}
}

void PlayerMove::UpdateFalling(float& deltaTime)
{
	AddForce(GRAVITY);
}

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
		mOwner->SetPosition(fixPos);
		return side;
	}
}

void PlayerMove::CheckCollision()
{
	//Check Block Collisions
	std::vector<Actor*> blocks = mOwner->GetGame()->GetBlocks();
	bool Check = true;
	for (Actor* i : blocks)
	{
		CollisionComponent* blokCol = i->GetCollComp();
		CollSide side = FixCollision(mOwner->GetCollComp(), blokCol);
		if (side == CollSide::Top && Check)
		{
			mVel.z = 0.0f;
			SetState(OnGround);
			Check = false;
		}
		else if (side == CollSide::Bottom && mState == Jump && Check)
		{
			mVel.z = 0.0f;
			Check = false;
		}
		else if (Check && side == CollSide::None)
		{
			SetState(Falling);
		}

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

void PlayerMove::AddForce(const Vector3& force)
{
	mPendingForces += force;
}

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
	}
	PhysicsUpdate(deltaTime);
	CheckCollision();

}
