#include "Sword.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"

Sword::Sword(Game* game) : Actor(game)
{
	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(25.0f, 25.0f);

	//Sprite Component
	mSprite = new SpriteComponent(this);
	mSprite->SetTexture(mGame->GetTexture("Assets/Sword/Up.png"));

}

void Sword::OnUpdate(float deltatime)
{

}

void  Sword::SetVisible(bool v)
{
	mSprite->SetIsVisible(v);
}

void Sword::SetSprite(std::string file)
{
	mSprite->SetTexture(mGame->GetTexture(file));
}

void Sword::SwordCollision()
{
	//Useful Variables 
	Vector2 offset;
	CollisionComponent* otherColl = nullptr;
	CollSide side = CollSide::None;

	//We must make a copy so that the vector doesn't suddenly chhange
	//during our loop
	std::vector<Actor*> killCopy;
	std::vector<Actor*> Problem = GetGame()->GetEnemies();
	for (Actor* enem : Problem)
	{
		otherColl = enem->GetCollComp();
		side = GetCollComp()->GetMinOverlap(otherColl, offset);
		if (side != CollSide::None)
		{
			killCopy.push_back(enem);
		}
	}
	for (Actor* enem : killCopy)
	{
		//Destroy enemy
		EnemyComponent* EnemyC = enem->GetComponent<EnemyComponent>();
		EnemyC->TakeDamage(1);
	}
}

