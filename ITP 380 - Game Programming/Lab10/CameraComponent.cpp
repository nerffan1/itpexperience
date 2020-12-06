#include "CameraComponent.h"
#include "Math.h"
#include "Actor.h"
#include "Player.h"
#include "PlayerMove.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Renderer.h"

CameraComponent::CameraComponent(Actor* owner) : Component(owner)
{
	player = mOwner->GetGame()->GetPlayer();
}

void CameraComponent::Update(float deltaTime)
{
	//Set View Matrix
	Matrix4 ViewMatrix = Matrix4::CreateLookAt(
		mOwner->GetPosition(),
		UpdateTarget(deltaTime),
		CameraUp(deltaTime)
	);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(ViewMatrix);
}

Vector3 CameraComponent::GetIdealPos()
{
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= PlayerNormal() * HDist;
	cameraPos.z = mOwner->GetPosition().z + VDist;
	return cameraPos;
}

Vector3 CameraComponent::GetCamPos(float& deltatime)
{
	return GetIdealPos();
}

Vector3 CameraComponent::PlayerNormal()
{
	return mOwner->GetForward();
}

Vector3 CameraComponent::UpdateTarget(float& deltatime)
{
	//Update Pitch
	mPitchAngle += mPitchSpeed * deltatime;
	if (mPitchAngle >= Math::Pi / 4)
	{
		mPitchAngle = Math::Pi / 4;
	}
	else if (mPitchAngle <= -Math::Pi / 4)
	{
		mPitchAngle = -Math::Pi / 4;
	}
	Matrix4 Yaw = Matrix4::CreateRotationZ(mOwner->GetRotation());
	Matrix4 Pitch = Matrix4::CreateRotationY(mPitchAngle); //This is the axis of pitch
	Matrix4 Roll = Matrix4::CreateRotationX(mPitchAngle);
	Matrix4 transform = Pitch * Yaw;
	Vector3 ToTrans(1.0f, 0.0f, 0.0f);
	Vector3 transformed = Vector3::Transform(ToTrans, transform);
	return mOwner->GetPosition() + transformed;
}

Vector3 CameraComponent::CameraUp(float deltatime)
{
	//Check if player is performing a wall run
	PlayerMove* PlayersMov = mOwner->GetComponent<PlayerMove>();
	if (PlayersMov->GetState() == PlayerMove::WallRun)
	{
		WallRunRot(PlayersMov, deltatime);
	}
	else
	{	
		FixUp(deltatime);
	}

	return mCamNorm;
}

void CameraComponent::WallRunRot(PlayerMove* PlayersMov, float& deltatime)
{
	//Get Wall normal and simply rotate it1
	Vector3 WallNormal = PlayersMov->SideNormal(PlayersMov->SideOfCollision);
	float RotationAngle;
	RotationAngle = Math::Lerp(0.0f, Math::Pi / 15.0f, mRotationTimer / 0.4f);
	Matrix4 rotation;
	if (WallNormal.x == -1.0f)
	{
		rotation = Matrix4::CreateRotationY(-RotationAngle);
	}
	else if (WallNormal.x == 1.0f)
	{
		rotation = Matrix4::CreateRotationY(RotationAngle);
	}
	else if (WallNormal.y == -1.0f)
	{
		rotation = Matrix4::CreateRotationX(RotationAngle);
	}
	else if (WallNormal.y == 1.0f)
	{
		rotation = Matrix4::CreateRotationX(-RotationAngle);
	}
	mCamNorm = Vector3::Transform(Vector3::UnitZ, rotation);
	mRotationTimer += deltatime;
}

void CameraComponent::FixUp(float& deltatime)
{
	//Reset Timer for another wall run
	mRotationTimer = 0.0f;

	//Lerp from current Normal to new Normal
	mCamNorm = Vector3::Lerp(mCamNorm, Vector3::UnitZ, mFixNormalTimer / 2.2f);

	//Idealize values if close to UnitZ
	if (Math::NearZero(mCamNorm.x))
	{
		mCamNorm.x = 0.0f;
	}
	if (Math::NearZero(mCamNorm.y))
	{
		mCamNorm.y = 0.0f;
	}
	if (Math::NearZero(1.0f - mCamNorm.z))
	{
		mCamNorm.z = 1.0f;
	}

	bool FixedX = mCamNorm.x == 0.0f;
	bool FixedY = mCamNorm.y == 0.0f;
	bool FixedZ = mCamNorm.z == 1.0f;
	if (FixedX && FixedY && FixedZ)
	{
		mFixNormalTimer = 0.0f;
		LastStateWallRun = false;
	}
	else
	{
		mFixNormalTimer += deltatime;
	}
}
