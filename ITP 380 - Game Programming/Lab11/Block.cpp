#include "Block.h"
#include "Mesh.h"
#include"MeshComponent.h"
#include "CollisionComponent.h"
#include "Renderer.h"
#include "Game.h"

Block::Block(Game* game, Actor* parent) : Actor(game, parent)
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

void Block::OnUpdate(float deltaTime)
{
	if (mRot)
	{
		float CurRot = GetRotation();
		SetRotation(CurRot + (Math::Pi*deltaTime/4));
	}
}

Block::~Block()
{
	mGame->RemoveBlock(this);
}
