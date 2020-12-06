#include "Checkpoint.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"

Checkpoint::Checkpoint(Game* game, Actor* parent) : Actor(game, parent)
{
	mMesh = game->GetRenderer()->GetMeshComp("Assets/Checkpoint.gpmesh");
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMeshComp(mMesh);
	mMeshComp->SetTextureIndex(1);

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(25.0f, 25.0f, 25.0f);

	//Add to queue
	mGame->AddCP(this);	
}

Checkpoint::~Checkpoint()
{
	mGame->RemoveCP(); 
}

void Checkpoint::OnUpdate(float deltaTime)
{
	if (this == mGame->GetCheckpoint())
	{
		mMeshComp->SetTextureIndex(0);
	}
	else
	{
		mMeshComp->SetTextureIndex(1);
	}
}
