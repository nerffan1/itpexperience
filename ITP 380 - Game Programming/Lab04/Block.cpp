#include "Block.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"

Block::Block(Game* game,Vector2 pos, char c): Actor(game)
{
	//Sprite Component
	mSprite = new SpriteComponent(this, 100);
	SetText(c);
	mSprite->SetIsVisible(true);
	this->SetPosition(pos);
	this->SetScale(1);
	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(32, 32);
	//Add to Game's block vector
	mGame->AddBlock(this);
}

Block::~Block() //Does this have to delete something else?
{
	//Delete Block from Vector
	mGame->RemoveBlock(this);
}

void Block::SetText(char c)
{
	std::string filename = "Assets/Block";
	filename += c;
	filename += ".png";
	mSprite->SetTexture(mGame->GetTexture(filename));
}

