#include "Arrow.h"
#include "Checkpoint.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "Player.h"
#include "MeshComponent.h"

Arrow::Arrow(Game* game) : Actor(game, nullptr)
{
	mMesh = mGame->GetRenderer()->GetMeshComp("Assets/Arrow.gpmesh");
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMeshComp(mMesh);
	mScale = 0.15f;
}

Arrow::~Arrow()
{
}

void Arrow::OnUpdate(float deltaTime)
{
	UpdateQuat(deltaTime);
}

void Arrow::UpdateQuat(float deltaTime)
{
	//Find angle of rotation
	if (mGame->GetCheckpoint() != nullptr)
	{
		Vector3 ToCP = mGame->GetCheckpoint()->GetPosition() - mGame->GetPlayer()->GetPosition();
		ToCP.Normalize();
		float RotAng = Math::Acos(Vector3::Dot(ToCP, Vector3::UnitX));
		Vector3 RotAxis = Vector3::Cross(Vector3::UnitX, ToCP);
		RotAxis.Normalize();
		SetQuaternion(Quaternion(RotAxis, RotAng));
	}
	else
	{
		SetQuaternion(Quaternion::Identity);
	}
	SetPosition(mGame->GetRenderer()->Unproject(Vector3(0.0f, 250.0f, 0.1f)));
}