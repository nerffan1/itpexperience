#include "Player.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"

Player::Player(Game* game) : Actor(game)
{
	//Set Mesh Component
	mMesh = new MeshComponent(this);
	Mesh* newMesh = mGame->GetRenderer()->GetMesh("Assets/Ship.gpmesh");
	mMesh->SetMesh(newMesh);

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(100, 40, 60);

	//Move Component 
	mMov = new PlayerMove(this);
}
