#include "SecurityCone.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Block.h"
#include "SDL/SDL_mixer.h"
#include "Player.h"

SecurityCone::SecurityCone(Game* game, Actor* parent) : Actor(game, parent)
{
	mMesh = mGame->GetRenderer()->GetMeshComp("Assets/Cone.gpmesh");
	mMeshComp = new MeshComponent(this, true);
	mMeshComp->SetMeshComp(mMesh);
	SetPosition(Vector3(30.0f, 0.0f, 0.0f));
	SetScale(3.0f);
}

void SecurityCone::OnUpdate(float deltaTime)
{
	CheckForPlayer();
	if (mConeState == Alert)
	{
		mAlertTimer -= deltaTime;
		if (mAlertTimer <= 0.0f)
		{
			SetState(InSight);
		}
	}
	else if (mConeState == InSight)
	{
		mInSightTimer -= deltaTime;
		if (mInSightTimer <= 0.0f)
		{
			mPlayer = mGame->GetPlayer();
			mPlayer->Respawn();
		}
	}
	else if (mConeState == Searching)
	{
		if (mAlertTimer < ALERT_TIMER)
		{
			mAlertTimer = ALERT_TIMER;
		}
		if (mInSightTimer < IN_SIGHT_TIMER)
		{
			mInSightTimer = IN_SIGHT_TIMER;
		}
	}
}

void SecurityCone::SetState(ConeState stat)
{
	switch (stat)
	{
	case ConeState::Alert:
	{
		if (mConeState != Alert)
		{
			mMeshComp->SetTextureIndex(1);
		}
		break;
	}
	case ConeState::InSight:
	{
		if (mConeState != InSight)
		{
			mMeshComp->SetTextureIndex(2);
		}
		break;
	}
	case ConeState::Searching:
	{
		if (mConeState != Searching)
		{
			mMeshComp->SetTextureIndex(0);
		}
		break;
	}
	}
	mConeState = stat;
}

void SecurityCone::CheckForPlayer()
{
	mPlayer = mGame->GetPlayer();
	Vector3 ToPlayer = mPlayer->GetPosition() - GetWorldPosition();
	float DistProj = Vector3::Dot(GetWorldForward(), ToPlayer);
	ToPlayer.Normalize();
	float CosAngle = Vector3::Dot(GetWorldForward(), ToPlayer);
	if (CosAngle >= sqrt(3) / 2.0f && DistProj <= CONEHEIGHT)
	{
		if (mConeState == Searching)
		{
			Mix_Chunk* AlertSound = mGame->GetSound("Assets/Sounds/SecurityDetected.wav");
			mAlertSFX = Mix_PlayChannel(Mix_GroupAvailable(1), AlertSound, 0);
			SetState(Alert);
		}
	}
	else
	{
		if (mConeState == Alert)
		{
			Mix_HaltChannel(mAlertSFX);
		}
		SetState(Searching);
	}
}
