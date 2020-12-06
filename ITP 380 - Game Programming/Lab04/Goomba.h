#pragma once
#include "Actor.h"
#include "CollisionComponent.h"

class Goomba : public Actor
{
public:
	Goomba(class Game* game, Vector2 iPos);
	~Goomba();
	void OnUpdate(float deltaTime) override;
	void Stompage();
	bool GetStompState();
	void LoadAnimation();
	void DeathAnimation();
	void ReverseDirection();
	float GetForwardSpeed();
	void SetForwardSpeed(float speed);

protected:
	class AnimatedSprite* mSprite = nullptr;
	class GoombaMove* mMov = nullptr;
	bool mStomped = false;
	float mDeathTime = 0.25f;
	std::vector<struct SDL_Texture*> mWalk, mDeath;

	//Constants
	const float DEATH_ANIM = 0.25f;
	const float FALL_DEATH = 460.0f;
};