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
	mColl->SetSize(64, 32);
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
	std::string filename;
	switch (c) 
	{
	case 'A':
		filename = "Assets/BlockA.png";
		break;
	case 'B':
		filename = "Assets/BlockB.png";
		break;
	case 'C':
		filename = "Assets/BlockC.png";
		break;
	case 'D':
		filename = "Assets/BlockD.png";
		break;
	case 'E':
		filename = "Assets/BlockE.png";
		break;
	}
	mSprite->SetTexture(mGame->GetTexture(filename));
}

