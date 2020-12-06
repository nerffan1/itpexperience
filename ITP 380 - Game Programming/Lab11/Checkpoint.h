#pragma once
#include "Actor.h"
#include <string>

class Checkpoint : public Actor
{
public:
	Checkpoint(class Game* game, Actor* parent);
	~Checkpoint();
	void OnUpdate(float deltaTime);

	//Setter/Getters
	void SetLevelString(std::string name) { mLevelString = name; }
	std::string GetLevelString() { return mLevelString; }

protected:
	std::string mLevelString = "";
};