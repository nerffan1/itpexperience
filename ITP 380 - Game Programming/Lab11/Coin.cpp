#include "Coin.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "CollisionComponent.h"

Coin::Coin(Game* game) : Actor(game, nullptr)
{
	mMesh = mGame->GetRenderer()->GetMeshComp("Assets/Coin.gpmesh");
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMeshComp(mMesh);

	//Set Collision
	mColl = new CollisionComponent(this);
	mColl->SetSize(100.0f, 100.0f, 100.0f);

	mGame->AddCoin(this);
}

Coin::~Coin()
{
	mGame->RemoveCoin(this);
}

void Coin::OnUpdate(float deltaTime)
{
	SetRotation(GetRotation() + Math::Pi * deltaTime);
}
