#include "Spawner.h"
#include "Game.h"
#include "Player.h"
#include "Actor.h"
#include "Goomba.h"

Spawner::Spawner(Game* game, Vector2 iPos) : Actor(game)
{
	SetPosition(iPos);
}

void Spawner::OnUpdate(float deltaTime)
{
	if (GetPosition().x - mGame->GetMario()->GetPosition().x < 600)
	{
		//Spawn Goomba with Spawner's coordinates and destroy
		Goomba* Spawn = new Goomba(mGame, GetPosition());
		SetState(ActorState::Destroy);
	}

}
