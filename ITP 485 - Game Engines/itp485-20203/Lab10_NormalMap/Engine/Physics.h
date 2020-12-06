#pragma once
#include "engineMath.h"
#include <vector>

class CollisionComponent;

class Physics
{
public:
	class AABB
	{
	public:
		AABB();
		AABB(Vector3 pMin, Vector3 pMax) : colMin(pMin), colMax(pMax) {};
		Vector3 colMin;
		Vector3 colMax;
	};
	class LineSegment
	{
	public: 
		LineSegment(Vector3 pSt, Vector3 pEnd) : start(pSt), end (pEnd) {}
		//Members
		Vector3 start;
		Vector3 end;
	};
	
	//Physics Methods
	static void AddObj(CollisionComponent* pObj) { mColVec.push_back(pObj); }
	static void RemoveObj(CollisionComponent* pObj);
	static bool RayCast(const LineSegment& segment, Vector3* pHitPoint = nullptr);
	static const std::vector<CollisionComponent*> getPlatforms() { return mColVec; } 
	//UnitTests 
	static bool Intersect(const LineSegment& segment, const AABB& box, Vector3* pHitPoint = nullptr);
	static bool Intersect(const AABB& a, const AABB& b, AABB* pOverlap = nullptr);
	static bool UnitTest();

	//Members 
	static Physics* Get() { return &mPhx;  }
private: 
	static std::vector<CollisionComponent*> mColVec;
	static Physics mPhx;
};