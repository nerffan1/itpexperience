#pragma once
#include "Actor.h"

class Spawner : public Actor
{
public:
	Spawner(class Game* game, Vector2 iPos);
protected:
	void OnUpdate(float deltaTime) override;
};