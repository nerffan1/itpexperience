#pragma once
#include "Actor.h"

class Arrow : public Actor
{
public:
	Arrow(class Game* game);
	~Arrow();
	void OnUpdate(float deltaTime) override;
	void UpdateQuat(float deltaTime);
protected:
};