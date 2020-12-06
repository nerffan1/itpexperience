#include "stdafx.h"
#include "Character.h"
#include "SkinnedObj.h"
#include "mesh.h"
#include "assetManager.h"
#include "jsonUtil.h"
#include "stringUtil.h"
#include "Skeleton.h"

Character::Character(SkinnedObj* pObj) :
	Component(pObj),
	mSkinnedObj(pObj),
	mCurrentAnim(nullptr)
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
	mAnimTime += deltaTime;
	if (mAnimTime > mCurrentAnim->GetLength())
	{
		mAnimTime = 0.0f;
	}
	std::vector<Matrix4> outPoses(mSkel->GetNumBones());
	mCurrentAnim->GetGlobalPoseAtTime(outPoses, mSkel, mAnimTime);
	std::vector<Matrix4> bindPoses = mSkel->GetGlobalInvBindPoses();

	//Combine matrices 
	size_t boneSize = mSkel->GetNumBones();
	for (size_t i = 0; i < boneSize; i++)
	{
		mSkinnedObj->mSkinConsts.c_skinMatrix[i] = bindPoses[i] * outPoses[i];;
	}
}

void Character::Update(float deltaTime)
{
	if (mCurrentAnim == nullptr)
	{
		SetAnim("run");
	}
	UpdateAnim(deltaTime);
}
