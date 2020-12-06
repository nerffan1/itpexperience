#pragma once
#include "Actor.h"

class SecretBlock : public Actor
{
public:
	SecretBlock(Vector2 pos, std::string dir, std::string room, float width, float height, class Game* game);
	void OnUpdate(float deltatime);
	void Contact(bool cont) { mContact = cont; }
	void ContactAnimation(float deltatime);
	void Unlock();
	bool IsLocked() { return mLocked; }
	void InvertDirection();

protected:
	class SpriteComponent* mSprite = nullptr;
	const float UNLOCK_SPEED = 80.0f;
	float MAX_DISPL = 32.0f;
	bool mLocked = true;
	bool mContact = false;
};
