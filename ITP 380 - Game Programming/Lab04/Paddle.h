#pragma once
#include "Actor.h"
#include "Math.h"

class Paddle : public Actor
{
public:
	Paddle(class Game* game);
	class CollisionComponent* GetCollComp();
	std::vector<Vector2> GetNormals();
protected:
	void OnProcessInput(const Uint8* keyState) override;
	class SpriteComponent* mSprite = nullptr;
	class MoveComponent* mMov = nullptr;
	void OnUpdate(float deltaTime) override;
	Vector2 mNormal1, mNormal2, mNormal3;

};