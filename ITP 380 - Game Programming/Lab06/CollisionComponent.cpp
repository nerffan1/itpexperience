#include "CollisionComponent.h"
#include "Actor.h"
#include <algorithm>

CollisionComponent::CollisionComponent(class Actor* owner)
:Component(owner)
,mWidth(0.0f)
,mHeight(0.0f)
{
	
}

CollisionComponent::~CollisionComponent()
{
	
}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	bool LeftSide = GetMax().x <= other->GetMin().x;
	bool RightSide = GetMin().x >= other->GetMax().x;
	bool BottomSide = GetMin().y >= other->GetMax().y;
	bool TopSide = GetMax().y <= other->GetMin().y;

	if (!(LeftSide || RightSide || BottomSide || TopSide))
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector2 CollisionComponent::GetMin() const
{
	Vector2 min;
	min.x = mOwner->GetPosition().x - (mWidth * mOwner->GetScale() / 2.0f);
	min.y = mOwner->GetPosition().y - (mHeight * mOwner->GetScale() / 2.0f);
	return min;
}

Vector2 CollisionComponent::GetMax() const
{
	Vector2 max;
	max.x = mOwner->GetPosition().x + (mWidth * mOwner->GetScale() / 2.0f);
	max.y = mOwner->GetPosition().y + (mHeight * mOwner->GetScale() / 2.0f);
	return max;
}

const Vector2& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector2& offset)
{
	offset = Vector2::Zero;
	// TODO: Implement
	if (!(this->Intersect(other)))
	{
		return CollSide::None;
	}
	else
	{
		//Calculate the Absolute value difference between sides
		float dx1 = other->GetMin().x - this->GetMax().x;
		float dx2 = other->GetMax().x - this->GetMin().x;
		float dy1 = other->GetMin().y - this->GetMax().y;
		float dy2 = other->GetMax().y - this->GetMin().y;
		float FindSide[4] = { Math::Abs(dx1),Math::Abs(dx2),Math::Abs(dy1),Math::Abs(dy2) };

		//Sort and return a value
		std::sort(FindSide, FindSide + sizeof(FindSide) / sizeof(FindSide[0]));
		if (FindSide[0] == Math::Abs(dx1))
		{
			offset.x = dx1;
			return CollSide::Left;
		}
		else if (FindSide[0] == Math::Abs(dx2))
		{
			offset.x = dx2;
			return CollSide::Right;
		}
		else if (FindSide[0] == Math::Abs(dy1))
		{
			offset.y = dy1;
			return CollSide::Top;
		}
		else
		{
			offset.y = dy2;
			return CollSide::Bottom;
		}
	}
}
