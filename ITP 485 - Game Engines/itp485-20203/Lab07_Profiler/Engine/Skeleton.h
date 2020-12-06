#pragma once
#include "BoneTransform.h"
#include <string>
#include <vector>


struct Bone
{
	BoneTransform mTBone;
	std::string mName;
	int mParent;
};

class Skeleton
{
public:
	//Accessors
	size_t GetNumBones() const;
	const Bone& GetBone(size_t idx) const;
	const std::vector<Bone>& GetBones() const;
	const std::vector<Matrix4>& GetGlobalInvBindPoses() const;
	//Functions
	static Skeleton* StaticLoad(const WCHAR* fileName, class AssetManager* pAssetManager);
	bool Load(const WCHAR* fileName);

private:
	//Functions
	void ComputeGlobalInvBindPose();

	//Members
	std::vector<Bone> mBones;
	std::vector<Matrix4> mBindPoses;
};