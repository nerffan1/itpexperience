#include "Paddle.h"
#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "CollisionComponent.h"

Paddle::Paddle(Game* game):Actor(game)
{
	//Collision Component
	mColl = new CollisionComponent(this);
	mColl->SetSize(104.0f, 24.0f);
	//Sprite Component 
	mSprite = new SpriteComponent(this,103);
	mSprite->SetTexture(game->GetTexture("Assets/Paddle.png"));
	mSprite->SetIsVisible(true);
	Vector2 iPos(1024 / 2, 725);
	this->SetPosition(iPos);
	this->SetScale(1);
	mMov = new MoveComponent(this);
	//Set Normal
	mNormal1.x = (-1/Math::Sqrt(2.0f)); mNormal1.y = (-1 / Math::Sqrt(2.0f));
	mNormal2.x = 0; mNormal2.y = -1;
	mNormal3.x = (1 / Math::Sqrt(2.0f)); mNormal3.y = (-1 / Math::Sqrt(2.0f));
}

void Paddle::OnProcessInput(const Uint8* keyState)
{
	//Movement
	if (keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT])
	{
		mMov->SetForwardSpeed(-400);
	}
	else if (keyState[SDL_SCANCODE_RIGHT] && !keyState[SDL_SCANCODE_LEFT])
	{
		mMov->SetForwardSpeed(400);
	}
	else
	{
		mMov->SetForwardSpeed(0);
	}
}

void Paddle::OnUpdate(float deltaTime)
{
	//Prevent Paddle from exceeding margins of map
	if (mPosition.x <= 84)
	{
		Vector2 newPos(84, mPosition.y);
		mPosition = newPos;
	}
	else if (mPosition.x >= (1024 - 84))
	{
		Vector2 newPos(1024 - 84, mPosition.y);
		mPosition = newPos;
	}
}

CollisionComponent* Paddle::GetCollComp()
{
	return mColl;
}

std::vector<Vector2> Paddle::GetNormals()
{
	std::vector<Vector2> normals;
	normals.push_back(mNormal1); 
	normals.push_back(mNormal2); 
	normals.push_back(mNormal3);
	return normals;
}
