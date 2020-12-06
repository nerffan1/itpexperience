#pragma once
#include "Actor.h"
#include <string>
#include <unordered_map>

enum class DoorState
{
	Open,
	Closed,
	Locked
};



class Door : public Actor
{
public:
	Door(Vector2 pos, std::string RoomName, std::string param[], Game* game);
	void SetUpDoor(std::string params[]);
	void UpdateTexture();
	DoorState GetDoorState();
	void SetDoorState(DoorState newState) { mDoorState = newState;  }
	std::string GetDestination();

protected:
	//Main Parameters 
	class SpriteComponent* mSprite = nullptr;
	DoorState mDoorState;
	std::string mDestination;
	std::string parameters[3];

	//Tools 
	std::unordered_map<std::string,DoorState> mStateMap;

	//Dimensions of Door
	const float d1 = 30.0f;
	const float d2 = 64.0f;
	float width;
	float height;
};