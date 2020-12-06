#include "CameraComponent.h"
#include "Math.h"
#include "Actor.h"
#include "Player.h"
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
		Vector3::UnitZ
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

Vector3 CameraComponent::GetCamPos(float &deltatime)
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
	Matrix4 transform = Pitch * Yaw;
	Vector3 ToTrans(1.0f, 0.0f, 0.0f);
	Vector3 transformed = Vector3::Transform(ToTrans, transform);
	return mOwner->GetPosition() + transformed;
}
