#pragma once
#include "Actor.h"

class Skeleton : Actor
{
public:
	Skeleton(Vector2 iPos, class Game* game);
	void LoadAnimation();
	void OnUpdate(float deltatime);
	void GenerateFireball(float& deltatime);
	void OnSight(bool inRoom);

protected:
	class AnimatedSprite* mSprite = nullptr;
	class EnemyComponent* mEnemyC = nullptr;
	
	//Fireball parameters
	float mShootInterval;
	float mCoolDown;
};