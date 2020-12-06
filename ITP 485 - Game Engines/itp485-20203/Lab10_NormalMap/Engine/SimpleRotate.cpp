#include "SimpleRotate.h"
#include "stdafx.h"
#include "SimpleRotate.h"

#include "jsonUtil.h"

void SimpleRotate::LoadProperties(const rapidjson::Value& property)
{
	GetFloatFromJSON(property, "speed", mSpeed);
}

void SimpleRotate::Update(float deltaTime)
{
	mObj->mObjConsts.modelToWorld *= Matrix4::CreateRotationZ(mSpeed * deltaTime);
}
