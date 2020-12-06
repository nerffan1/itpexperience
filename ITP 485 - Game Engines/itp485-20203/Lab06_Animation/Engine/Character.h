#pragma once
#include "Component.h"
#include "stdafx.h"

class SkinnedObj;
class Skeleton;
class Animation;

class Character : public Component
{
public:
	Character(SkinnedObj* pObj);
	
	//Functions
	void LoadProperties(const rapidjson::Value& properties) override;
	bool SetAnim(const std::string& animName);
	void UpdateAnim(float deltaTime);
	void Update(float deltaTime) override;

private:
	SkinnedObj* mSkinnedObj;
	Skeleton* mSkel;
	std::unordered_map<std::string, const Animation*> mAnimTable;
	const Animation* mCurrentAnim;
	float mAnimTime = 0.0f;
};