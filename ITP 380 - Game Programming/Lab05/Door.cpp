#include "Door.h"
#include <string>
#include "CollisionComponent.h"
#include "Game.h"
#include "SpriteComponent.h"

Door::Door(Vector2 pos, std::string RoomName,std::string params[], Game* game) : 
	Actor(game)
{
	//Load Maps
	mStateMap["Open"] = DoorState::Open;
	mStateMap["Closed"] = DoorState::Closed;
	mStateMap["Locked"] = DoorState::Locked;

	//Save Parameters and load
	for (size_t i = 0; i < 3; i++)
	{
		parameters[i] = params[i];
	}
	SetUpDoor(params);
	SetPosition(pos);

	//Load mColl
	mColl = new CollisionComponent(this);
	mColl->SetSize(width, height);

	//Load Sprite
	mSprite = new SpriteComponent(this, 50);
	mSprite->SetIsVisible(true);
	std::string file = "Assets/Door/" + params[1] + params[2] + ".png";
	mSprite->SetTexture(mGame->GetTexture(file));

	//Add to game
	mGame->AddDoor(this,RoomName);
}

void Door::SetUpDoor(std::string params[])
{
	//Load 3 main parameters
	mDestination = params[0];
	mDirection = mDirectionMap[params[1]];
	mDoorState = mStateMap[params[2]];

	//Load the Proper Dimensions based on direction
	if (mDirection == Direction::Left || mDirection == Direction::Right)
	{
		width = d2;
		height= d1;
	}
	else
	{
		width = d1;
		height = d2;
	}
}



void Door::UpdateTexture()
{
	std::string filename = "Assets/Door/" + parameters[1];
	if (mDoorState == DoorState::Open)
	{
		filename += "Open.png";
	}
	mSprite->SetTexture(mGame->GetTexture(filename));
}

DoorState Door::GetDoorState()
{
	return mDoorState;
}

std::string Door::GetDestination()
{
	return mDestination;
}
