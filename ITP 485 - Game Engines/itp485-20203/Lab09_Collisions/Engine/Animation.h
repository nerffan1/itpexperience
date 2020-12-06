#pragma once
#include "stdafx.h"
#include "engineMath.h"
#include "BoneTransform.h"

class AssetManager;
class Skeleton;

class Animation
{
public:
	uint32_t GetNumBones() const { return mNumBone; }
	uint32_t GetNumFrames() const { return mNumFrame; }
	static Animation* StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager);
	bool Load(const WCHAR* fileName);

	//Function
	void GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const;

	//Getter
	float GetLength() const { return mLength; }
	const std::vector<std::vector<BoneTransform>> GetAnimData() const { return mAnimData; }

private:
	uint32_t mNumBone;
	uint32_t mNumFrame;
	float mLength;
	std::vector<std::vector<BoneTransform>> mAnimData;
};