#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	void OnUpdate(float deltatime) override;
	void LoadAnimations();
	void PlayerDeath();
	void UpdateAnimation(float &deltaTime);
protected:
	class AnimatedSprite* mSprite = nullptr;
	class PlayerMove* mMov = nullptr;
	bool mDead = false;
	float mDeathTime = 2.9f;
};