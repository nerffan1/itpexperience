#include "stdafx.h"
#include "Skeleton.h"
#include <fstream>
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include "Graphics.h"
#include <sstream>
#include "jsonUtil.h"

size_t Skeleton::GetNumBones() const
{
	return mBones.size();
}

const Bone& Skeleton::GetBone(size_t idx) const
{
	return mBones[idx];
}

const std::vector<Bone>& Skeleton::GetBones() const
{
	// TODO: insert return statement here
	return mBones;
}

const std::vector<Matrix4>& Skeleton::GetGlobalInvBindPoses() const
{
	return mBindPoses;
}

Skeleton* Skeleton::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
	Skeleton* pMesh = new Skeleton();
	if (false == pMesh->Load(fileName))
	{
		delete pMesh;
		return nullptr;
	}
	return pMesh;
}

bool Skeleton::Load(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itpskel" ||
		ver != 1)
	{
		return false;
	}

	// Load Bones
	rapidjson::SizeType BoneCount = doc["bonecount"].GetInt();
	const rapidjson::Value& bones = doc["bones"];
	if (!bones.IsArray() || bones.Size() < 1 || bones.Size() != BoneCount)
	{
		return false;
	}
	Bone newBone;
	for (rapidjson::SizeType i = 0; i < BoneCount; i++)
	{
		const rapidjson::Value& bone = bones[i];
		if (!bone.IsObject())
		{
			return false;
		}
		//Get Name
		newBone.mName = bone["name"].GetString();
		//Get Parent
		newBone.mParent = bone["parent"].GetInt();
		//Get Bone Bind Pose: rot and trans
		const rapidjson::Value& bindpose = bone["bindpose"];
		if (!bindpose.IsObject())
		{
			return false;
		}
		GetQuaternionFromJSON(bindpose, "rot", newBone.mTBone.mRot);
		GetVectorFromJSON(bindpose, "trans", newBone.mTBone.mTrans);
		mBones.emplace_back(newBone);
	}
	ComputeGlobalInvBindPose();
	return true;
}

void Skeleton::ComputeGlobalInvBindPose()
{
	Matrix4 NewBindPose;
	for (auto bone : mBones)
	{
		NewBindPose = bone.mTBone.ToMatrix();
		auto parentIndex = bone.mParent;
		while (parentIndex != -1)
		{
			NewBindPose *= mBones[parentIndex].mTBone.ToMatrix();
			parentIndex = mBones[parentIndex].mParent;
		}
		NewBindPose.Invert();
		mBindPoses.emplace_back(NewBindPose);
	}
}
