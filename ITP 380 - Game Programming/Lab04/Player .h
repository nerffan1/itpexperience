#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player();
protected:
	class SpriteComponent* idle = nullptr;
};