#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game, Actor* parent);
protected:
	class PlayerMove* mMov = nullptr;
};