#include "stdafx.h"
#include "PointLight.h"
#include "jsonUtil.h"

PointLight::PointLight(Game* pGame, RenderObj* pObj) :
	Component(pObj),
	mGame(pGame),
	mLData(pGame->AllocateLight())
{

}

PointLight::~PointLight()
{
	mGame->FreeLight(mLData);
}

void PointLight::LoadProperties(const rapidjson::Value& properties)
{
	Vector3 temp;
	GetVectorFromJSON(properties, "diffuseColor", mLData->diffuseColor);
	GetVectorFromJSON(properties, "specularColor", mLData->specularColor);
	GetFloatFromJSON(properties, "specularPower", mLData->specularPower);
	GetFloatFromJSON(properties, "innerRadius", mLData->innerRadius);
	GetFloatFromJSON(properties, "outerRadius", mLData->outerRadius);
}

void PointLight::Update(float deltaTime)
{ 
	mLData->position = mObj->mObjConsts.modelToWorld.GetTranslation();
}
