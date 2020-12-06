#pragma once
#include "Actor.h"

class Bat : public Actor 
{
public:
	Bat(Vector2 iPos,class Game* game);
	void OnUpdate(float deltatime) override;
	void LoadAnimation();
	void OnSight(bool inRoom);

	//Movement 
	void Movement(float deltatime);
	void CheckColliders();

protected:
	class EnemyComponent* mEnemyC = nullptr;
	class AnimatedSprite* mSprite = nullptr;

	//Animation Time
	float mFlyTime = 3.0f;

	//Useful variables
	 float BAT_SPEED = 50.0f;
	 const float FLY_TIME = 3.0f;

	 //Flying Direction
	 Vector2 min;
	 Vector2 max;
	 Vector2 mFlyDir;
};