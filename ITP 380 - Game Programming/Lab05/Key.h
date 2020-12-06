#pragma once
#include "Actor.h"
#include "Player.h"

class Key : public Actor
{
public:
	Key(Vector2 iPos, Game* game);
protected:
	class SpriteComponent* mSprite = nullptr;
	class Collectible* collect = nullptr;
};