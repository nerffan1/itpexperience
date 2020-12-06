#pragma once

#include "Actor.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);
protected:
	class SpriteComponent* mSprite = nullptr;
	class MoveComponent* mMove = nullptr;
	void OnProcessInput(const Uint8* keyState) override;
};