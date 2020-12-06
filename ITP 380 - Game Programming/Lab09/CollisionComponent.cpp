#include "CollisionComponent.h"
#include "Actor.h"

CollisionComponent::CollisionComponent(class Actor* owner)
	:Component(owner)
	, mWidth(0.0f)
	, mHeight(0.0f)
	, mDepth(0.0f)
{

}

CollisionComponent::~CollisionComponent()
{

}

bool CollisionComponent::Intersect(const CollisionComponent* other)
{
	Vector3 aMin = GetMin();
	Vector3 aMax = GetMax();
	Vector3 bMin = other->GetMin();
	Vector3 bMax = other->GetMax();

	bool no = aMax.x < bMin.x ||
		aMax.y < bMin.y ||
		aMax.z < bMin.z ||
		bMax.x < aMin.x ||
		bMax.y < aMin.y ||
		bMax.z < aMin.z;

	return !no;
}

Vector3 CollisionComponent::GetMin() const
{
	Vector3 v = mOwner->GetPosition();
	v.x -= mDepth * mOwner->GetScale() / 2.0f;
	v.y -= mWidth * mOwner->GetScale() / 2.0f;
	v.z -= mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

Vector3 CollisionComponent::GetMax() const
{
	Vector3 v = mOwner->GetPosition();
	v.x += mDepth * mOwner->GetScale() / 2.0f;
	v.y += mWidth * mOwner->GetScale() / 2.0f;
	v.z += mHeight * mOwner->GetScale() / 2.0f;
	return v;
}

const Vector3& CollisionComponent::GetCenter() const
{
	return mOwner->GetPosition();
}

CollSide CollisionComponent::GetMinOverlap(
	const CollisionComponent* other, Vector3& offset)
{
	offset = Vector3::Zero;
	// TODO: Implement
	if (!Intersect(other))
	{
		return CollSide::None;
	}
	else
	{
		//Z-axis
		float dz1 = (other->GetMin().z - this->GetMax().z); //Bottom of Object
		float dz2 = other->GetMax().z - this->GetMin().z; //Top of Object
		float MinZ = Math::Min(Math::Abs(dz1), Math::Abs(dz2));

		//X-axis
		float dx1 = other->GetMin().x - this->GetMax().x;
		float dx2 = other->GetMax().x - this->GetMin().x;
		float MinX = Math::Min(Math::Abs(dx1), Math::Abs(dx2));

		//Y-axis
		float dy1 = other->GetMin().y - this->GetMax().y;
		float dy2 = other->GetMax().y - this->GetMin().y;
		float MinY = Math::Min(Math::Abs(dy1), Math::Abs(dy2));


		//Get the smallest number
		float FinalMin = Math::Min(Math::Min(MinX, MinY), MinZ);

		//Due to Absolute values and signs, setting the offset is more complicated this time
		offset = GetOffSet(
			dz1, dz2, dx1, dx2, dy1,
			dy2, MinZ, MinX, MinY, FinalMin
		);

		//Return the side
		if (FinalMin == Math::Abs(dz1))
		{
			return CollSide::Bottom;
		}
		else if (FinalMin == Math::Abs(dz2))
		{
			return CollSide::Top;
		}
		else
		{
			return CollSide::Side;
		}

	}
}

Vector3 CollisionComponent::GetOffSet(float& dz1, float& dz2, float& dx1, float& dx2, float& dy1, float& dy2, float& MinZ, float& MinX, float& MinY,
	float& FinalMin)
{
	//Z-offset
	if (FinalMin == MinZ)
	{
		if (MinZ == Math::Abs(dz1))
		{
			return Vector3(0.0f, 0.0f, dz1);
		}
		else
		{
			return Vector3(0.0f, 0.0f, dz2);
		}
	}
	//X-Offset
	else if (FinalMin == MinX)
	{
		if (MinX == Math::Abs(dx1))
		{
			return Vector3(dx1, 0.0f, 0.0f);
		}
		else
		{
			return Vector3(dx2, 0.0f, 0.0f);
		}
	}
	//Y-Offset
	else
	{
		if (MinY == Math::Abs(dy1))
		{
			return Vector3(0.0f, dy1, 0.0f);
		}
		else
		{
			return Vector3(0.0f, dy2, 0.0f);
		}
	}
}
