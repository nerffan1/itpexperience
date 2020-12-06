#pragma once
#include "Actor.h"

class Coin : public Actor
{
public:
	Coin(class Game* game);
	~Coin();
	void OnUpdate(float deltaTime);

private:
};