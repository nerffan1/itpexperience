#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
protected:
	class PlayerMove* mMov = nullptr;
};