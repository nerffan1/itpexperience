#pragma once
#include "Actor.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	~Asteroid();
protected:
	void OnUpdate(float deltaTime) override;
	class SpriteComponent* mSprite;
	class MoveComponent* mMove = nullptr;
};