#include "stdafx.h"
#include "CollisionComponent.h"
#include "jsonUtil.h"

CollisionComponent::CollisionComponent(RenderObj* pRender) : Component(pRender)
{

}

void CollisionComponent::LoadProperties(const rapidjson::Value& prop)
{
	if (prop.IsObject())
	{
		Vector3 loadV;
		
		//Load min
		GetVectorFromJSON(prop, "min", loadV);
		aabb.colMin = loadV;
		
		//Load max
		GetVectorFromJSON(prop, "max", loadV);
		aabb.colMax = loadV;
	}
}

const Physics::AABB CollisionComponent::GetAABB() const
{
	//Scale the AABB
	Physics::AABB newab = aabb; 
	newab.colMax *= mObj->mObjConsts.modelToWorld.GetScale().x;
	newab.colMin *= mObj->mObjConsts.modelToWorld.GetScale().x;

	//Translate the AABB
	newab.colMax += mObj->mObjConsts.modelToWorld.GetTranslation();
	newab.colMin += mObj->mObjConsts.modelToWorld.GetTranslation();

	return newab;
}
