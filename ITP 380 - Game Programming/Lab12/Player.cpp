#include "Player.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "CameraComponent.h"
#include "CollisionComponent.h"

Player::Player(Game* game, Actor* parent) : Actor(game, parent)
{

	//Cam Component 
	CameraComponent* mCam = new CameraComponent(this);

	//mMov Component
	mMov = new PlayerMove(this);

	//Coll Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(50, 175, 50);
}

void Player::Respawn()
{
	SetPosition(mRespawn);
	SetRotation(0.0f);
}
