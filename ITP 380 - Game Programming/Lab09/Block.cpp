#include "Block.h"
#include "Mesh.h"
#include"MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Game.h"

Block::Block(Game* game) : Actor(game)
{
	mGame->AddBlock(this);
	mScale = 46.0f;

	//Mesh Component 
	mMesh = this->GetGame()->GetRenderer()->GetMeshComp("Assets/Cube.gpmesh");
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMeshComp(mMesh);
	
	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(1.0f, 1.0f, 1.0f);
}

Block::~Block()
{
	mGame->RemoveBlock(this);
}
