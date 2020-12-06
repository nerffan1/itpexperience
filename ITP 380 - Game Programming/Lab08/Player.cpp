#include "Player.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Game.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "PlayerMove.h"
#include "CameraMove.h"
#include "CollisionComponent.h"
#include "PlayerUI.h"

Player::Player(Game* game) : Actor(game)
{
	mScale = 0.75f;
	//Set Mesh Component
	mMesh = new MeshComponent(this);
	Mesh* newMesh = mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh");
	mMesh->SetMesh(newMesh);
	
	//Move Component 
	PlayerMove* mMov = new PlayerMove(this);
	CameraMove* mCam = new CameraMove(this);
	mColl = new CollisionComponent(this);
	mColl->SetSize(40, 40, 40);

	//UI Component
	PlayerUI* mUI = new PlayerUI(this);
}
