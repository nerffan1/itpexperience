#include "Enemy.h"
#include "Game.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Renderer.h"
#include "CollisionComponent.h"
#include "EnemyMove.h"

Enemy::Enemy(Game* game) : Actor(game)
{
	mScale = 0.75f;
	Mesh* newmesh = mGame->GetRenderer()->GetMesh("Assets/Kart.gpmesh");
	mMesh = new MeshComponent(this);
	mMesh->SetMesh(newmesh);
	mMesh->SetTextureIndex(6);

	//Move Component
	EnemyMove* mMov = new EnemyMove(this);
}
