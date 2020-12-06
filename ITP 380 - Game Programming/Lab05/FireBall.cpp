#include "FireBall.h"
#include "CollisionComponent.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Player.h"

FireBall::FireBall(Vector2 iPos, Game* game) : Actor(game)
{
	SetPosition(iPos);

	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(25.0f, 25.0f);

	//Sprite Component
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/FireballRed.png"));
	mSprite->SetIsVisible(true);

	//Get Angle
	Vector2 dif = mGame->GetPlayer()->GetPosition() - mPosition;
	mAngle = atan2(dif.y, dif.x);
}

void FireBall::OnUpdate(float deltatime)
{
	if (mLifeTime > 0)
	{
		UpdateMovement(deltatime);
		mLifeTime -= deltatime;
	}
	else
	{
		SetState(ActorState::Destroy);
	}
}

void FireBall::UpdateMovement(float& deltatime)
{
	//Calculate vector 
	Vector2 follow(Math::Cos(mAngle) * BALL_SPEED * deltatime, Math::Sin(mAngle) * BALL_SPEED * deltatime);
	Vector2 newPos = GetPosition() + follow;
	SetPosition(newPos);
}
