#pragma once
#include "Actor.h"

class Player : public Actor
{
public:
	Player(class Game* game, Actor* parent);
	void SetRespawn(Vector3 pos) { mRespawn = pos; }
	Vector3 GetRespawn() { return mRespawn; }
	void Respawn();
protected:
	class PlayerMove* mMov = nullptr;
	Vector3 mRespawn;
};