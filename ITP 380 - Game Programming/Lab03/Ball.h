#pragma once
#include "Actor.h"

class Ball : public Actor
{
public:
	Ball(class Game* game);
	class CollisionComponent* GetCollComp();
protected:
	class SpriteComponent* mSprite = nullptr;
	class BallMove* mMove = nullptr;
};