#include "BallMove.h"
#include "Actor.h"
#include "CollisionComponent.h"
#include "Game.h"
#include "Ball.h"
#include "Paddle.h"
#include "Block.h"

BallMove::BallMove(Ball* owner) : MoveComponent(owner), mVelocity(100.0f, -130.0f)
{

}

void BallMove::Update(float deltaTime)
{
	//Update ball coordinates
	Vector2 dl(mVelocity.x * deltaTime, mVelocity.y * deltaTime);
	mOwner->SetPosition(mOwner->GetPosition() + dl);

	//Check for Wall Boundaries
	bool LeftWall = (mOwner->GetPosition().x < mLeftBound);
	bool RightWall = (mOwner->GetPosition().x > mRightBound);
	bool TopWall = (mOwner->GetPosition().y < mTopBound);
	bool BottomWall = (mOwner->GetPosition().y > mBottomBound);

	if (LeftWall)
	{
		Vector2 setPos(mLeftBound, mOwner->GetPosition().y);
		mOwner->SetPosition(setPos);
		mVelocity.x *= -1;
	}
	else if (RightWall)
	{
		Vector2 setPos(mRightBound, mOwner->GetPosition().y);
		mOwner->SetPosition(setPos);
		mVelocity.x *= -1;
	}
	if (TopWall)
	{
		Vector2 setPos(mOwner->GetPosition().x, mTopBound);
		mOwner->SetPosition(setPos);
		mVelocity.y *= -1;
	}
	else if (BottomWall)
	{
		Vector2 setPos(1024 / 2, 700), setVel(245, -230);
		mOwner->SetPosition(setPos);
		mVelocity = setVel;
	}

	//Bouncing off Block 
	for (Block* blok : mOwner->GetGame()->GetBlocks())
	{	
		Vector2 offset;
		CollisionComponent* blocko = blok->GetCollComp();
		CollSide side = mOwner->GetCollComp()->GetMinOverlap(blocko, offset);
		
		if (side == CollSide::Left || side == CollSide::Right)
		{
			//Velocity Bounce
			mVelocity.x *= -1;

			//Destroy Block
			blok->SetState(ActorState::Destroy);

			//Break Loop
			break;
		}
		else if (side == CollSide::Top || side == CollSide::Bottom)
		{
			//Velocity
			mVelocity.y *= -1;

			//Destroy Block
			blok->SetState(ActorState::Destroy);
			
			//Break Loop
			break;
		}

		//Position Correction
		mOwner->SetPosition(mOwner->GetPosition() + offset);
	}

	//Bouncing off Paddle
	Paddle* myPaddle = mOwner->GetGame()->GetPaddle();
	CollisionComponent* PaddleColl = mOwner->GetGame()->GetPaddle()->GetCollComp();
	Vector2 offset;
	CollSide side = mOwner->GetCollComp()->GetMinOverlap(PaddleColl, offset);

	if (side == CollSide::Top)
	{
		mOwner->SetPosition(mOwner->GetPosition() + offset);
		//Useful Variables
		float third1 = PaddleColl->GetMin().x + (PaddleColl->GetWidth() / 3);
		float third2 = PaddleColl->GetMin().x + (PaddleColl->GetWidth() * 2 / 3);
		float third3 = PaddleColl->GetMin().x + PaddleColl->GetWidth();

		//CorrectPosition
		
		if (mOwner->GetCollComp()->GetMax().x >= PaddleColl->GetMin().x && mOwner->GetPosition().x < third1)
		{
			mVelocity = Vector2::Reflect(mVelocity, myPaddle->GetNormals()[0]);
		}
		else if (mOwner->GetPosition().x >= third1 && mOwner->GetPosition().x <= third2)
		{
			mVelocity = Vector2::Reflect(mVelocity, myPaddle->GetNormals()[1]);
		}
		else if (mOwner->GetPosition().x > third2 && mOwner->GetCollComp()->GetMin().x <= third3)
		{
			mVelocity = Vector2::Reflect(mVelocity, myPaddle->GetNormals()[2]);
		}
	}

}
