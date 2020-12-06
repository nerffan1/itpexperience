#pragma once
#include "Component.h"
#include "Physics.h"

class CollisionComponent : public Component
{
public:
	CollisionComponent(RenderObj* pRender);
	void LoadProperties(const rapidjson::Value& properties) override;
	const Physics::AABB GetAABB() const;

private:
	Physics::AABB aabb;
};