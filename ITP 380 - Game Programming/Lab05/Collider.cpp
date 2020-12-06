#include "Collider.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Actor.h"

Collider::Collider(float width, float height, float x, float y, Game* game) : Actor(game)
{
	//Declare Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(width, height);
	
	//Set Position of Colliders
	Vector2 InitialPos(x, y);
	SetPosition(InitialPos);

	//Add to Vector
	mGame->AddCollider(this);

}
