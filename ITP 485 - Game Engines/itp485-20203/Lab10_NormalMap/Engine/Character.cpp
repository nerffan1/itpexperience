#include "stdafx.h"
#include "Character.h"
#include "SkinnedObj.h"
#include "mesh.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "stringUtil.h"
#include "Skeleton.h"
#include "Profiler.h"
#include "JobManager.h"

Character::Character(Game* pGame, SkinnedObj* pObj) :
	Component(pObj),
	mSkinnedObj(pObj),
	mSkel(nullptr),
	mGame(pGame),
	mCurrentAnim(nullptr),
	mAnimJob(this)
{

}

void Character::LoadProperties(const rapidjson::Value& properties)
{
	//Get Manager for loading
	AssetManager* manager = mObj->getMesh()->getManager();

	//Load skeleton section
	std::wstring skelFile;
	GetWStringFromJSON(properties, "skeleton", skelFile);
	manager->SetSkel(L"MySkel", manager->LoadSkeleton(skelFile));
	mSkel = manager->GetSkel(L"MySkel");

	//Load Animations
	const rapidjson::Value& anims = properties["animations"];
	if (!anims.IsArray())
	{
		return;
	}
	//Loop through animations
	for (rapidjson::SizeType i = 0; i < anims.Size(); i++)
	{
		//Animation Name wstr
		std::string strName = anims[i][0].GetString();
		std::wstring wstrFile;
		//Animation File wstr
		std::string strFile = anims[i][1].GetString();
		StringUtil::String2WString(wstrFile, strFile);
		//Load Animations
		//manager->SetAnim(wstrName, manager->LoadAnimation(wstrFile)); ????????
		mAnimTable[strName] = manager->LoadAnimation(wstrFile);
	}

}

bool Character::SetAnim(const std::string& animName)
{
	if (mAnimTable.find(animName) != mAnimTable.end())
	{
		mCurrentAnim = mAnimTable[animName];
		mAnimTime = 0.0f;
	}
	return true;
}

void Character::UpdateAnim(float deltaTime)
{
	PROFILE_SCOPE(anim);
	mAnimTime += deltaTime;
	JobManager::GetMan()->AddJob(&mAnimJob);
}

void Character::Update(float deltaTime)
{
	if (mCurrentAnim == nullptr)
	{
		SetAnim("run");
	}
	UpdateAnim(deltaTime);
}
