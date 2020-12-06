#include "Ship.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Laser.h"

Ship::Ship(Game* game) :Actor(game)
{
	mSprite = new SpriteComponent(this, 100);
	mSprite->SetTexture(game->GetTexture("Assets/Ship.png"));
	mSprite->SetIsVisible(true);
	Vector2 center(1024 / 2, 768 / 2);
	this->SetPosition(center);
	this->SetScale(1);
	mMove = new MoveComponent(this);
}

void Ship::OnUpdate(float deltaTime)
{
	mlaserBuffer += deltaTime;
}

void Ship::OnProcessInput(const Uint8* keyState)
{
	//Linear Movement
	if (keyState[SDL_SCANCODE_UP] && !keyState[SDL_SCANCODE_DOWN])
	{
		mMove->SetForwardSpeed(SHIP_SPEED);
		mSprite->SetTexture(this->GetGame()->GetTexture("Assets/ShipThrust.png")); 
	}
	else if (keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP])
	{
		mMove->SetForwardSpeed(-SHIP_SPEED);
		mSprite->SetTexture(this->GetGame()->GetTexture("Assets/ShipThrust.png")); 
	}
	else
	{
		mMove->SetForwardSpeed(0.0f);
		mSprite->SetTexture(this->GetGame()->GetTexture("Assets/Ship.png"));
	}

	//Rotational Movement
	if (keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT])
	{
		mMove->SetAngularSpeed(-ANGULAR_SPEED);
	}
	else if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT])
	{
		mMove->SetAngularSpeed(ANGULAR_SPEED);
	}
	else
	{
		mMove->SetAngularSpeed(0);
	}
	if (keyState[SDL_SCANCODE_SPACE])
	{
		if (mlaserBuffer >= 1)
		{
			Laser* newLaser = new Laser(mGame, this->GetPosition(), this->GetRotation());
			mlaserBuffer = 0;
		}
	}
}