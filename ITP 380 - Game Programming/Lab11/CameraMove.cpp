#include "CameraComponent.h"
#include "Math.h"
#include "Actor.h"
#include "Player.h"
#include "Game.h"
#include "Renderer.h"
#include "CameraMove.h"

CameraMove::CameraMove(Actor* owner) : Component(owner)
{
	mCamPos = GetIdealPos();
	player = mOwner->GetGame()->GetPlayer();
}

void CameraMove::Update(float deltaTime)
{
	Vector3 cameraTarget = mOwner->GetPosition();
	cameraTarget += PlayerNormal() * TargetOffset;

	//Set View Matrix
	Matrix4 ViewMatrix = Matrix4::CreateLookAt(
		GetCamPos(deltaTime),
		cameraTarget,
		Vector3::UnitZ
	);
	mOwner->GetGame()->GetRenderer()->SetViewMatrix(ViewMatrix);
}

Vector3 CameraMove::GetIdealPos()
{
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= PlayerNormal() * HDist;
	cameraPos.z = mOwner->GetPosition().z + VDist;
	return cameraPos;
}

Vector3 CameraMove::GetCamPos(float &deltatime)
{
	return Vector3::Zero;
}

Vector3 CameraMove::PlayerNormal()
{
	float KarAngle = mOwner->GetRotation();
	return Vector3(Math::Cos(KarAngle), Math::Sin(KarAngle), 0.0f);
}
