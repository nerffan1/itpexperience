#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game);
	void OnUpdate(float deltatime) override;
	
	//Player Death
	void KillPlayer();
	void Death(const float &deltatime);

	//Animation
	void LoadAnimations();
	void UpdateAnimation(float &deltaTime);
	void SetAttackAnimation();
	void SetMovementAnimation();

	void AddKey() { keys++; }
	int GetKeys() { return keys; }
	void RemoveKey() { keys--; }


protected:
	class AnimatedSprite* mSprite = nullptr;
	class PlayerMove* mMov = nullptr;
	bool mDead = false;
	float mDeathTime = 2.9f;
	int keys = 0;
};