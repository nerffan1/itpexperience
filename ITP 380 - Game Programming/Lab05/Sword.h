#pragma once
#include "Actor.h"

class Sword : public Actor
{
public:
	Sword(class Game* game);
	void OnUpdate(float deltatime) override;
	void SetVisible(bool v);
	void SetSprite(std::string file);
	void SwordCollision();

protected:
	class SpriteComponent* mSprite = nullptr;
};