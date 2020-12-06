#pragma once

#include "Actor.h"

class Ship : public Actor
{
public:
	float SHIP_SPEED = 200.0f;
	float ANGULAR_SPEED = 2.71f;
	Ship(class Game* game);
protected:
	float mlaserBuffer = 1;
	virtual void OnUpdate(float deltaTime);
	class SpriteComponent* mSprite = nullptr;
	class MoveComponent* mMove = nullptr;
	void OnProcessInput(const Uint8* keyState) override;
};