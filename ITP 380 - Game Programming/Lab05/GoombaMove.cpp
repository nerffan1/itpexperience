#include "GoombaMove.h"
#include "MoveComponent.h"
#include "Actor.h"
#include "Game.h"
#include "CollisionComponent.h"

GoombaMove::GoombaMove(class Goomba* owner) : MoveComponent(owner)
{
	SetForwardSpeed(-SPEED);
	mGoombaOwner = owner;
}


void GoombaMove::Update(float deltaTime)
{
	if (mGoombaOwner->GetStompState())
	{
		mForwardSpeed = 0;
	}
	//Update Position 
	Vector2 dl(mForwardSpeed * deltaTime, mYSpeed * deltaTime);
	mOwner->SetPosition(mOwner->GetPosition() + dl);

	//Check Actors
	CheckActors(deltaTime);

	//Gravity 
	if (mInAir == true)
	{
		mYSpeed += ACCEL * deltaTime;
	}
}

void GoombaMove::CheckActors(float deltaTime)
{
	if (mOwner->GetState() != ActorState::Destroy)
	{
		//Useful Variables;
		CollisionComponent* otherColl = nullptr;
		Vector2 offset;
		CollSide side = CollSide::None;

		//Iterate through blocks and find Collisions
		for (Block* blok : mOwner->GetGame()->GetBlocks())
		{
			// Only find the Vertical offset if the blok is underneath Goomba and within a certain range
			if (VerticalGoombaBoundary(blok))
			{
				//Get Component if it criteria is met
				otherColl = blok->GetCollComp();
				side = mOwner->GetCollComp()->GetMinOverlap(otherColl, offset);

				//Stop looping if there's a bottom or top collision 
				if (side == CollSide::Top)
				{
					//Make sure they're not in air
					offset.y += 0.1f;
					Vector2 newPos = mOwner->GetPosition() + offset;
					mOwner->SetPosition(newPos);

					//Speed 
					mInAir = false;
					if (mYSpeed >= 0)
					{
						mYSpeed = 0.0f;
					}
					break;
				}
				else
				{
					mInAir = true;
				}
			}
			else if (HorizontalGoombaBoundary(blok))
			{
				//Get Component if it criteria is met
				otherColl = blok->GetCollComp();
				side = mOwner->GetCollComp()->GetMinOverlap(otherColl, offset);

				//Left and Right Side
				if (side == CollSide::Left || side == CollSide::Right)
				{
					//Position Correction
					Vector2 newPos = mOwner->GetPosition() + offset;
					mOwner->SetPosition(newPos);
					//Change of direction
					SetForwardSpeed(GetForwardSpeed() * -1);
					break;
				}
			}
		}

		//Iterate through other goombas
		for (Goomba* otherGoomba : mOwner->GetGame()->GetGoombas())
		{
			if (otherGoomba != mOwner && GoombaXNeighbourhood(otherGoomba) && GoombaYNeighbourhood(otherGoomba))
			{
				//Check collision side
				otherColl = otherGoomba->GetCollComp();
				side = mOwner->GetCollComp()->GetMinOverlap(otherColl, offset);

				//Left and Right Side
				if (side == CollSide::Left || side == CollSide::Right)
				{
					//Position Correction 
					Vector2 newPos = mOwner->GetPosition() + offset;
					mOwner->SetPosition(newPos);

					//Change of direction only if gomba is coming in opposite direction
					if (mForwardSpeed != otherGoomba->GetForwardSpeed())
					{
						//Change Both goombas' speed
						SetForwardSpeed(GetForwardSpeed() * -1);
						ChangeOtherGoombaSpeed(otherGoomba);
					}
					break;
				}
			}
		}

	}
}


bool GoombaMove::GoombaXNeighbourhood(Actor* otherActor)
{
	return (otherActor->GetPosition().x > mOwner->GetPosition().x - mXBoundary && otherActor->GetPosition().x < mOwner->GetPosition().x + mXBoundary) ? true : false;
}

bool GoombaMove::GoombaYNeighbourhood(Actor* otherActor)
{
	return (otherActor->GetPosition().y > mOwner->GetPosition().y - mYBoundary && otherActor->GetPosition().y < mOwner->GetPosition().y + mYBoundary) ? true : false;
}

bool GoombaMove::VerticalGoombaBoundary(Block* blok)
{
	return blok->GetPosition().y > mOwner->GetPosition().y && GoombaXNeighbourhood(blok);
}

bool GoombaMove::HorizontalGoombaBoundary(Block* blok)
{
	return (GoombaYNeighbourhood(blok) && GoombaXNeighbourhood(blok));
}

void GoombaMove::ChangeOtherGoombaSpeed(Goomba* other)
{
	other->SetForwardSpeed(other->GetForwardSpeed()*-1);
}
