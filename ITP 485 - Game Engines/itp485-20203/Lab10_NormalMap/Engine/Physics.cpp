#include "Physics.h"
#include "stdafx.h"
#include "Physics.h"
#include "CollisionComponent.h"

using AABB = Physics::AABB;
using LineSegment = Physics::LineSegment;
std::vector<CollisionComponent*> Physics::mColVec = std::vector<CollisionComponent*>();
Physics Physics::mPhx = Physics();

bool Physics::Intersect(const AABB& a, const AABB& b, AABB* pOverlap)
{
	//Note: Perhaps could be calculated more eficiently if these booleans were counted.
	//I think you need always 2 dimensions to intersect in order to have a physical intersection
	bool Col = a.colMax.x < b.colMin.x ||
		a.colMax.y < b.colMin.y ||
		a.colMax.z < b.colMin.z ||
		b.colMax.x < a.colMin.x ||
		b.colMax.y < a.colMin.y ||
		b.colMax.z < a.colMin.z;

	if (Col)
	{
		return false;
	}
	else
	{
		Vector3 diffmai = a.colMax - b.colMin;
		Vector3 diffmii = a.colMin - b.colMin;
		Vector3 diffmaa = a.colMax - b.colMax;

		//Make diff zero if needed
		{
			if (Math::IsZero(diffmai.x))
			{
				diffmai.x = 0.0f;
			}
			if (Math::IsZero(diffmai.y))
			{
				diffmai.y = 0.0f;
			}
			if (Math::IsZero(diffmai.z))
			{
				diffmai.z = 0.0f;
			}
			if (Math::IsZero(diffmii.x))
			{
				diffmii.x = 0.0f;
			}
			if (Math::IsZero(diffmii.y))
			{
				diffmii.y = 0.0f;
			}
			if (Math::IsZero(diffmii.z))
			{
				diffmii.z = 0.0f;
			}
			if (Math::IsZero(diffmaa.x))
			{
				diffmaa.x = 0.0f;
			}
			if (Math::IsZero(diffmaa.y))
			{
				diffmaa.y = 0.0f;
			}
			if (Math::IsZero(diffmaa.z))
			{
				diffmaa.z = 0.0f;
			}
		}

		//Check signs to determine proper intersection
		//X dimension
		if (diffmai.x > 0.0f) //Is max a above min b, in x
		{
			if (diffmaa.x <= 0.0f) // Is max b above max a, in x
			{
				pOverlap->colMax.x = a.colMax.x;
			}
			else // max a is above max b
			{
				pOverlap->colMax.x = b.colMax.x;
			}

			if (diffmii.x >= 0.0f) // If min a is greater than min b 
			{
				pOverlap->colMin.x = a.colMin.x;
			}
			else
			{
				pOverlap->colMin.x = b.colMin.x;
			}
		}

		//Y Dimension
		if (diffmai.y > 0.0f) //Is max a above min b, in x
		{
			if (diffmaa.y <= 0.0f) // Is max b above max a, in x
			{
				pOverlap->colMax.y = a.colMax.y;
			}
			else // max a is above max b
			{
				pOverlap->colMax.y = b.colMax.y;
			}

			if (diffmii.y >= 0.0f) // If min a is greater than min b 
			{
				pOverlap->colMin.y = a.colMin.y;
			}
			else
			{
				pOverlap->colMin.y = b.colMin.y;
			}
		}

		//Z Dimension
		if (diffmai.z > 0.0f) //Is max a above min b, in x
		{
			if (diffmaa.z <= 0.0f) // Is max b above max a, in x
			{
				pOverlap->colMax.z = a.colMax.z;
			}
			else // max a is above max b
			{
				pOverlap->colMax.z = b.colMax.z;
			}

			if (diffmii.z >= 0.0f) // If min a is greater than min b 
			{
				pOverlap->colMin.z = a.colMin.z;
			}
			else
			{
				pOverlap->colMin.z = b.colMin.z;
			}
		}
		return true;
	}
}

void Physics::RemoveObj(CollisionComponent* pObj)
{
	auto i = std::find(mColVec.begin(), mColVec.end(), pObj);
	mColVec.erase(i);
}

bool Physics::RayCast(const LineSegment& segment, Vector3* pHitPoint)
{
	Vector3 tmp1;
	float tmp2;
	float d = 0.0f;
	for (auto col : mColVec)
	{
		if (Intersect(segment, col->GetAABB(), &tmp1))
		{
			tmp2 = (segment.start - tmp1).Length();
			if (tmp2 < d || d == 0.0f)
			{
				d = tmp2;
			}
		}
	}
	return true;
}

bool Physics::Intersect(const LineSegment& seg, const AABB& box, Vector3* pHitPoint)
{

	// Intersect ray R(t) = p + t*d against AABB a. When intersecting,
	// return intersection distance tmin and point q of intersection
	Vector3 direction = (seg.end - seg.start);
	Vector3 d = direction / direction.Length();
	float tmin = 0.0f; // set to -FLT_MAX to get first hit on line
	float tmax = direction.Length(); // set to max distance ray can travel (for segment)
	float ood;
	float t1;
	float t2;

	if (Math::IsZero(d.x)) {
		// Ray is parallel to slab. No hit if origin not within slab
		if (seg.start.x < box.colMin.x || seg.start.x > box.colMax.x) { return false; }
	}
	else {
		// Compute intersection t value of ray with near and far plane of slab
		ood = 1.0f / d.x;
		t1 = (box.colMin.x - seg.start.x) * ood;
		t2 = (box.colMax.x - seg.start.x) * ood;
		// Make t1 be intersection with near plane, t2 with far plane
		if (t1 > t2)
		{
			float tmp = t1;
			t1 = t2;
			t2 = tmp;
		}
		// Compute the intersection of slab intersection intervals
		tmin = Math::Max(tmin, t1);
		tmax = Math::Min(tmax, t2);
		// Exit with no collision as soon as slab intersection becomes empty
		if (tmin > tmax)
		{
			*pHitPoint = Vector3::Zero;
			return false;
		}
	}
	if (Math::IsZero(d.y)) {
		// Ray is parallel to slab. No hit if origin not within slab
		if (seg.start.y < box.colMin.y || seg.start.y > box.colMax.y) { return false; }
	}
	else {
		// Compute intersection t value of ray with near and far plane of slab
		ood = 1.0f / d.y;
		t1 = (box.colMin.y - seg.start.y) * ood;
		t2 = (box.colMax.y - seg.start.y) * ood;
		// Make t1 be intersection with near plane, t2 with far plane
		if (t1 > t2)
		{
			float tmp = t1;
			t1 = t2;
			t2 = tmp;
		}
		// Compute the intersection of slab intersection intervals
		tmin = Math::Max(tmin, t1);
		tmax = Math::Min(tmax, t2);
		// Exit with no collision as soon as slab intersection becomes empty
		if (tmin > tmax)
		{
			*pHitPoint = Vector3::Zero;
			return false;
		}
	}
	if (Math::IsZero(d.z)) {
		// Ray is parallel to slab. No hit if origin not within slab
		if (seg.start.z < box.colMin.z || seg.start.z > box.colMax.z) { return false; }
	}
	else {
		// Compute intersection t value of ray with near and far plane of slab
		ood = 1.0f / d.z;
		t1 = (box.colMin.z - seg.start.z) * ood;
		t2 = (box.colMax.z - seg.start.z) * ood;
		// Make t1 be intersection with near plane, t2 with far plane
		if (t1 > t2)
		{
			float tmp = t1;
			t1 = t2;
			t2 = tmp;
		}
		// Compute the intersection of slab intersection intervals
		tmin = Math::Max(tmin, t1);
		tmax = Math::Min(tmax, t2);
		// Exit with no collision as soon as slab intersection becomes empty
		if (tmin > tmax)
		{
			*pHitPoint = Vector3::Zero;
			return false;
		}
	}

	// Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
	*pHitPoint = seg.start + d * tmin;
	return true;
}

bool Physics::UnitTest()
{
	struct TestAABB
	{
		AABB a;
		AABB b;
		AABB overlap;
	};
	const TestAABB testAABB[] =
	{
	{
	AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f)),
	AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(10.0f, 10.0f, 10.0f))
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-110.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f)),
	AABB(Vector3(-100.0f, -10.0f, -10.0f), Vector3(-90.0f, 10.0f, 10.0f))
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(110.0f, 10.0f, 10.0f)),
	AABB(Vector3(90.0f, -10.0f, -10.0f), Vector3(100.0f, 10.0f, 10.0f))
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-10.0f, -110.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f)),
	AABB(Vector3(-10.0f, -100.0f, -10.0f), Vector3(10.0f, -90.0f, 10.0f))
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 110.0f, 10.0f)),
	AABB(Vector3(-10.0f, 90.0f, -10.0f), Vector3(10.0f, 100.0f, 10.0f))
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-10.0f, -10.0f, -110.0f), Vector3(10.0f, 10.0f, -90.0f)),
	AABB(Vector3(-10.0f, -10.0f, -100.0f), Vector3(10.0f, 10.0f, -90.0f))
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 110.0f)),
	AABB(Vector3(-10.0f, -10.0f, 90.0f), Vector3(10.0f, 10.0f, 100.0f))
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-120.0f, -10.0f, -10.0f), Vector3(-110.0f, 10.0f, 10.0f)),
	AABB(Vector3::One, Vector3::Zero)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(110.0f, -10.0f, -10.0f), Vector3(120.0f, 10.0f, 10.0f)),
	AABB(Vector3::One, Vector3::Zero)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-10.0f, -120.0f, -10.0f), Vector3(10.0f, -110.0f, 10.0f)),
	AABB(Vector3::One, Vector3::Zero)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-10.0f, 110.0f, -10.0f), Vector3(10.0f, 120.0f, 10.0f)),
	AABB(Vector3::One, Vector3::Zero)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-10.0f, -10.0f, -120.0f), Vector3(10.0f, 10.0f, -110.0f)),
	AABB(Vector3::One, Vector3::Zero)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	AABB(Vector3(-10.0f, -10.0f, 110.0f), Vector3(10.0f, 10.0f, 120.0f)),
	AABB(Vector3::One, Vector3::Zero)
	},
	};

	//Temp AABB for comparisons
	AABB  tmp(Vector3::Zero, Vector3::Zero);

	//Loop through all the AABB tests
	size_t tests = ARRAYSIZE(testAABB);
	for (size_t i = 0; i < tests; i++)
	{
		if (Intersect(testAABB[i].a, testAABB[i].b, &tmp))
		{
			if (tmp.colMax.x != testAABB[i].overlap.colMax.x
				|| tmp.colMax.y != testAABB[i].overlap.colMax.y
				|| tmp.colMax.z != testAABB[i].overlap.colMax.z)
			{
				return false;
			}
		}
	}

	//
	struct TestSegment
	{
		AABB box;
		LineSegment segment;
		bool hit;
		Vector3 point;
	};
	const TestSegment testSegment[] =
	{
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(-110.0f, 0.0f, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)),
	true, Vector3(-100.0f, 0.0f, 0.0f)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(0.0f, -110.0f, 0.0f), Vector3(0.0f, -90.0f, 0.0f)),
	true, Vector3(0.0f, -100.0f, 0.0f)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(0.0f, 0.0f, -110.0f), Vector3(0.0f, 0.0f, -90.0f)),
	true, Vector3(0.0f, 0.0f, -100.0f)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(110.0f, 0.0f, 0.0f), Vector3(90.0f, 0.0f, 0.0f)),
	true, Vector3(100.0f, 0.0f, 0.0f)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(0.0f, 110.0f, 0.0f), Vector3(0.0f, 90.0f, 0.0f)),
	true, Vector3(0.0f, 100.0f, 0.0f)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(0.0f, 0.0f, 110.0f), Vector3(0.0f, 0.0f, 90.0f)),
	true, Vector3(0.0f, 0.0f, 100.0f)
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(-120.0f, 0.0f, 0.0f), Vector3(-110.0f, 0.0f, 0.0f)),
	false, Vector3::Zero
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(0.0f, -120.0f, 0.0f), Vector3(0.0f, -110.0f, 0.0f)),
	false, Vector3::Zero
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(0.0f, 0.0f, -120.0f), Vector3(0.0f, 0.0f, -110.0f)),
	false, Vector3::Zero
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(120.0f, 0.0f, 0.0f), Vector3(110.0f, 0.0f, 0.0f)),
	false, Vector3::Zero
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(0.0f, 120.0f, 0.0f), Vector3(0.0f, 110.0f, 0.0f)),
	false, Vector3::Zero
	},
	{
	AABB(Vector3(-100.0f, -100.0f, -100.0f), Vector3(100.0f, 100.0f, 100.0f)),
	LineSegment(Vector3(0.0f, 0.0f, 120.0f), Vector3(0.0f, 0.0f, 110.0f)),
	false, Vector3::Zero
	},
	};

	Vector3* hitPoint = new Vector3();

	//Loop through all the tests
	tests = ARRAYSIZE(testSegment);
	for (size_t i = 0; i < tests; i++)
	{
		if (testSegment[i].hit == Intersect(testSegment[i].segment, testSegment[i].box, hitPoint))
		{
			if (hitPoint->x != testSegment[i].point.x ||
				hitPoint->y != testSegment[i].point.y ||
				hitPoint->z != testSegment[i].point.z)
			{
				return false;
			}
		}
	}
	return true;
}

Physics::AABB::AABB()
{

}
