#pragma once
#include "Actor.h"
class Laser : public Actor
{
public:
	Laser(class Game* game, Vector2 shipPos, float rotation);
protected:
	float lifeTime = 0;
	void OnUpdate(float deltaTime) override;
	class SpriteComponent* mSprite = nullptr;
	class MoveComponent* mMove = nullptr;
};