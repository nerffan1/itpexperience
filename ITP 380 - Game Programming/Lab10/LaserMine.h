#pragma once
#include "Actor.h"

class LaserMine : public Actor
{
public:
	LaserMine(class Game* game, Actor* parent);
private:
	class LaserComponent* mLaser = nullptr;
	
};