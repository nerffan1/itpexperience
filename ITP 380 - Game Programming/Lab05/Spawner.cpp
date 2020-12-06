#include "Spawner.h"
#include "Game.h"
#include "Player.h"
#include "Actor.h"
#include "Bat.h"
#include "Key.h"
#include "Skeleton.h"


Spawner::Spawner(Game* game, Vector2 iPos, std::string type, std::string room) : 
	Actor(game)
	,mType(type)
	,mRoomName(room)
{
	SetPosition(iPos);

	//Add to Game
	mGame->AddSpawner(this, room);
}

Spawner::~Spawner()
{
	mGame->RemoveSpawner(this);
}

void Spawner::SpawnObject()
{
	if (mType == "Key")
	{
		Key* newKey = new Key(mPosition, mGame);
	}
	else if (mType == "Bat")
	{
		Bat* newBat = new Bat(mPosition, mGame);
	}
	else if (mType == "Skeleton")
	{
		Skeleton* newSkel = new Skeleton(mPosition, mGame);
	}
	SetState(ActorState::Destroy);
}

