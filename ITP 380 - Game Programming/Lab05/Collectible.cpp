#include "Collectible.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Math.h"
#include "Game.h"
#include "Player.h"
#include "Key.h"

Collectible::Collectible(Actor* owner) : Component(owner)
{
}

void Collectible::SetOnCollect(std::function<void()> OnCollect)
{
	mOnCollect = OnCollect;
}

void Collectible::Update(float deltaTime)
{
	Vector2 offset;
	CollisionComponent* PlayerColl = mOwner->GetGame()->GetPlayer()->GetCollComp();
	if (mOwner->GetCollComp()->GetMinOverlap(PlayerColl, offset) != CollSide::None)
	{
		mOwner->SetState(ActorState::Destroy);
		if (mOnCollect)
		{
			mOnCollect();
		}
		else
		{
			SDL_Log("mOnCollect is not set. You cannot use Callback");
		}
	}
}
