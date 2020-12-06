#include "SecurityCamera.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "SecurityCone.h"
#include "Player.h"

SecurityCamera::SecurityCamera(Game* game) : Actor(game, nullptr)
{
	//Mesh Component
	mMesh = mGame->GetRenderer()->GetMeshComp("Assets/Camera.gpmesh");
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMeshComp(mMesh);
	mCone = new SecurityCone(game, this);
	game->AddCamera(this);
}

SecurityCamera::~SecurityCamera()
{
	if (mRotSFX != -1)
	{
		Mix_Pause(mRotSFX);
	}
	mGame->RemoveCamera(this);
}

void SecurityCamera::OnUpdate(float deltaTime)
{
	if (mCone->GetConeState() == SecurityCone::Searching)
	{
		CameraRotation(deltaTime);
	}
	else if (InterpTimer <= INT_TIME)
	{
		Mix_Pause(mRotSFX);
	}
}

void SecurityCamera::CameraRotation(float& deltaTime)
{
	if (InterpTimer <= INT_TIME)
	{
		if (InterpTimer == 0.0f)
		{
			Mix_Chunk* rot = GetGame()->GetSound("Assets/Sounds/CameraMotor.wav");
			mRotSFX = Mix_PlayChannel(Mix_GroupAvailable(1), rot, 0);
		}
		else
		{
			Mix_Volume(mRotSFX, CameraVolume());
			Mix_Resume(mRotSFX);
		}
		mQuat = Quaternion::Slerp(mStartQ, mEndQ, InterpTimer / INT_TIME);
		InterpTimer += deltaTime;
	}
	else if (PauseTimer <= PAUSE_TIME)
	{
		if (PauseTimer == 0.0f)
		{
			Mix_HaltChannel(mRotSFX);
			mRotSFX = -1;
			Mix_Chunk* stop = GetGame()->GetSound("Assets/Sounds/CameraMotorStop.wav");
			mStopSFX = Mix_PlayChannel(Mix_GroupAvailable(1), stop, 0);
			Mix_Volume(mStopSFX, CameraVolume());
		}
		PauseTimer += deltaTime;
	}
	else
	{
		Quaternion holder = mEndQ;
		mEndQ = mStartQ;
		mStartQ = holder;
		PauseTimer = 0.0f;
		InterpTimer = 0.0f;
		Mix_Pause(mStopSFX);
	}
}

int SecurityCamera::CameraVolume()
{
	int Volume = 0;
	Vector3 dist = GetWorldPosition() - mGame->GetPlayer()->GetPosition();
	if (dist.Length() > 1500.0f)
	{
	}
	else if (dist.Length() < 1500.0f && dist.Length() > 500.0f)
	{
		float dis = dist.Length();
		float f = (-1.0f / 1000.0f) * (dis - 500.0f) + 1.0f;
		Volume = (int)Math::Lerp(0, 128, f);
	}
	else
	{
		Volume = 128;
	}
	return Volume;
}

