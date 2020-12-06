#include "EnemyComponent.h"
#include "Actor.h"
#include "Game.h"

EnemyComponent::EnemyComponent(Actor* owner) : Component(owner)
, HP(1)
, mDamage(1)
{
	//Add enemy to Map
	mOwner->GetGame()->AddEnemy(mOwner, mOwner->GetGame()->GetCurrentRoom());
}

EnemyComponent::~EnemyComponent()
{
	mOwner->GetGame()->RemoveEnemy(mOwner);
}

void EnemyComponent::TakeDamage(int dam)
{
	HP -= dam;

	//Destroy enemy if 0 HP
	if (HP <= 0)
	{
		if (mOnDeath)
		{
			mOnDeath();
		}
		mOwner->SetState(ActorState::Destroy);
	}
	else
	{
		if (mOnTakeDamage)
		{
			mOnTakeDamage();
		}
	}
}

