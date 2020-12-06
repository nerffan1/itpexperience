#pragma once
#include "MoveComponent.h"
#include "Math.h"

class BallMove : public MoveComponent
{
public:
	BallMove(class Ball* owner);
	void Update(float deltaTime) override;
protected:
	Vector2 mVelocity;
	float mLeftBound = 43.0f, mRightBound = (float)(1024 - 32 - 11);
	float mTopBound = 43.0f;
	float mBottomBound = 800.0f;
};	