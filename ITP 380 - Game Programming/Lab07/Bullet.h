#pragma once
#include "Actor.h"

class Bullet : public Actor
{
public:
	Bullet(Vector3 pos,Vector3 normal, class Game* game);
	void OnUpdate(float deltaTime) override;
	void CheckCollision();
	void CheckLife(float &deltatime);

protected:
	class MeshComponent* mMesh = nullptr;
	float mLifeTime = 1.0f;
};
