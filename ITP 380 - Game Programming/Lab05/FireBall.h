#pragma once
#include "Actor.h"

class FireBall: public Actor
{
public:
	FireBall(Vector2 iPos, class Game* game);
	void OnUpdate(float deltatime);
	void UpdateMovement(float& deltatime);

protected:
	class SpriteComponent* mSprite = nullptr;
	class MoveComponent* mMove = nullptr;
	
	//Useful Variables
	const float BALL_SPEED = 200.0f;
	float mLifeTime = 2.0f;
	float mAngle = 0.0f;
};