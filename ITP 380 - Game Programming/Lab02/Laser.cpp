#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Random.h"
#include "Game.h"
#include "Actor.h"
#include "Asteroid.h"

Laser::Laser(Game* game, Vector2 shipPos, float rotation) :Actor(game)
{
	mSprite = new SpriteComponent(this, 99);
	mSprite->SetTexture(game->GetTexture("Assets/Laser.png"));
	mSprite->SetIsVisible(true);
	mMove = new MoveComponent(this);
	mMove->SetForwardSpeed(400.0f);
	this->SetScale(1);
	this->SetPosition(shipPos);
	this->SetRotation(rotation);
}

void Laser::OnUpdate(float deltaTime)
{
	lifeTime += deltaTime;
	if (lifeTime >= 1)
	{
		this->SetState(ActorState::Destroy);
	}
	else
	{
		for (Asteroid* aster : mGame->GetAsteroid())
		{
			Vector2 distance(aster->GetPosition() - GetPosition());
			if (distance.Length() <= 70)
			{
				aster->SetState(ActorState::Destroy);
				SetState(ActorState::Destroy);
			}
		}
	}
}
