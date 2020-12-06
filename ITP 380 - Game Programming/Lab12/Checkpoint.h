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
	void SetTextString(std::string name) { mText = name; }
	std::string GetTextString() { return mText; }

protected:
	std::string mLevelString = "";
	std::string mText = "";
};