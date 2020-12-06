#include "Ball.h"
#include "SpriteComponent.h"
#include "BallMove.h"
#include "Game.h"
#include "CollisionComponent.h"

Ball::Ball(Game* game) : Actor(game)
{
	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(20.0f, 20.0f);
	//Sprite Component
	mSprite = new SpriteComponent(this, 100);
	mSprite->SetTexture(game->GetTexture("Assets/Ball.png"));
	mSprite->SetIsVisible(true);
	Vector2 iPos(1024 / 2, 700);
	this->SetPosition(iPos);
	this->SetScale(1);
	mMove = new BallMove(this);
}

CollisionComponent* Ball::GetCollComp()
{
	return mColl;
}
