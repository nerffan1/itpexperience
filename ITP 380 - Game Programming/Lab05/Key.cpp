#include "Key.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "CollisionComponent.h"
#include "Collectible.h"
#include <functional>

Key::Key(Vector2 iPos, Game* game) : Actor(game)
{
	SetPosition(iPos);

	//Collision Component 
	mColl = new CollisionComponent(this);
	mColl->SetSize(16.0f, 32.0f);

	//Sprite Component
	mSprite = new SpriteComponent(this);
	mSprite->SetIsVisible(true);
	mSprite->SetTexture(mGame->GetTexture("Assets/Key.png"));

	//Create Collectible
	Collectible* mCollect = new Collectible(this);
	mCollect->SetOnCollect([this] { 
		mGame->GetPlayer()->AddKey();
	});

}

