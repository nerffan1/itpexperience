#include "SideBlock.h"
#include "Game.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Renderer.h"
#include "Player.h"

SideBlock::SideBlock(Game* game, int ColorIndex) : Actor(game)
{
	mScale = 500.0f;
	//Create Mesh Component
	mMesh = new MeshComponent(this);
	Mesh* newMesh = mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh");
	mMesh->SetMesh(newMesh);
	mMesh->SetTextureIndex(ColorIndex);
	//Add to Game Vector
	mGame->AddSideBlok(this);
}

SideBlock::~SideBlock()
{
	mGame->RemoveSideBlok(this);
}

void SideBlock::OnUpdate(float deltaTime)
{
	//Create coordinate to delete blocks at
	float ByeBlok = mGame->GetPlayer()->GetPosition().x - DeleteCoor;
	if (GetPosition().x < ByeBlok)
	{
		SetState(ActorState::Destroy);
	}
}
