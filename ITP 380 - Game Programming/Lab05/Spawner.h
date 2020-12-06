#pragma once
#include "Actor.h"


class Spawner : public Actor
{
public:
	Spawner(class Game* game, Vector2 iPos, std::string type, std::string room);
	~Spawner();
	void SpawnObject();
protected:
	std::string mType;
	std::string mRoomName;
};