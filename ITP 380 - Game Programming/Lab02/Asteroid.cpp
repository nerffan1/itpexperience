#include "Asteroid.h"
#include "Actor.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"

Asteroid::Asteroid(Game* game):Actor(game)
{
	//Add to vector
	mGame->AddAsteroid(this);

	//Sprite Component
	mSprite = new SpriteComponent(this, 101);
	mSprite->SetTexture(game->GetTexture("Assets/Asteroid.png"));
	
	//Move Component
	mMove = new MoveComponent(this);
	mMove->SetForwardSpeed(150.0f);
	mMove->SetAngularSpeed(Random::GetFloatRange(0.0f,Math::TwoPi));
	Vector2 min(0, 0); Vector2 max(1024, 768);
	this->SetPosition(Random::GetVector(min,max));
}

Asteroid::~Asteroid()
{
}

void Asteroid::OnUpdate(float deltaTime)
{
	Vector2 min(0, 0), original(this->GetPosition());
	if (this->GetPosition().x < 0)
	{
		Vector2 newPos(1022, original.y);
		this->SetPosition(newPos);
	}
	if (this->GetPosition().x > 1020)
	{
		Vector2 newPos(0, original.y);
		this->SetPosition(newPos);
	}
	if (this->GetPosition().y < 0)
	{
		Vector2 newPos(original.x,762);
		this->SetPosition(newPos);
	}
	if (this->GetPosition().y > 768)
	{
		Vector2 newPos(original.x, 0);
		this->SetPosition(newPos);
	}
}
