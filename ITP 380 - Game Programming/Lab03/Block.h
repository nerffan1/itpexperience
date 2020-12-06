#pragma once

#include "Actor.h"
#include <string>

class Block : public Actor
{
public:
	Block(class Game* game,Vector2 pos, char c);
	~Block();
	void SetText(char c);
protected:
	class SpriteComponent* mSprite = nullptr;;
};