#include "Block.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "Player.h"

Block::Block(Game* game, Vector3 pos, char type, int layer) : Actor(game)
{
	mLayer = layer;
	mPosition = pos;
	mScale = 25.0f;

	//Initiate Mesh Component
	mMesh = new MeshComponent(this);
	Mesh* newMesh = mGame->GetRenderer()->GetMesh("Assets/Cube.gpmesh");
	mMesh->SetMesh(newMesh);
	if (type == 'A')
	{
		mMesh->SetTextureIndex(2);
		mType = normal;
	}
	else if (type == 'B')
	{
		mMesh->SetTextureIndex(4);
		mType = exploding;
	}

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(1, 1, 1);

	//Add to vector 
	mGame->AddBlok(this);

}

Block::~Block()
{
	mGame->RemoveBlok(this);
}

void Block::OnUpdate(float deltaTime)
{
	//Create coordinate to delete blocks at
	float ByeBlok = mGame->GetPlayer()->GetPosition().x - DeleteCoor;
	if (GetPosition().x < ByeBlok)
	{
		SetState(ActorState::Destroy);
	}
}

void Block::Explode()
{
	Mix_PlayChannel(-1, GetGame()->GetSound("Assets/Sounds/BlockExplode.wav"), 0);
	mState = ActorState::Destroy;
	for (Block* explodeToo : mGame->GetBlockLayers()[mLayer])
	{
		float distance = (explodeToo->GetPosition() - mPosition).Length();
		if (explodeToo != this && distance <= 50.0f)
		{
			if (explodeToo->GetState() != ActorState::Destroy && explodeToo->mType == type::exploding)
			{
				explodeToo->Explode();
			}
			else
			{
				explodeToo->SetState(ActorState::Destroy);
			}
		}
	}
}
