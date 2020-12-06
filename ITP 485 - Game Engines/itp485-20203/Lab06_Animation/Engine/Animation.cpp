#include "stdafx.h"
#include "Animation.h"
#include "jsonUtil.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "BoneTransform.h"
#include "Skeleton.h"

Animation* Animation::StaticLoad(const WCHAR* fileName, AssetManager* pAssetManager)
{
	Animation* pAnim = new Animation();
	if (false == pAnim->Load(fileName))
	{
		delete pAnim;
		return nullptr;
	}
	return pAnim;
}

bool Animation::Load(const WCHAR* fileName)
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
		str != "itpanim" ||
		ver != 2)
	{
		return false;
	}

	// Load Animation
	const rapidjson::Value& sequence = doc["sequence"];
	if (!sequence.IsObject())
	{
		return false;
	}
	//Frame count
	GetUintFromJSON(sequence, "frames", mNumFrame);
	//Animation Length
	GetFloatFromJSON(sequence, "length", mLength);
	//BoneCount
	GetUintFromJSON(sequence, "bonecount", mNumBone);
	//Load Animation Transforms
	mAnimData.resize(static_cast<size_t>(mNumBone));
	const rapidjson::Value& tracks = sequence["tracks"];
	if (!tracks.IsArray())
	{
		return false;
	}
	auto AnimCount = tracks.Size();
	for (rapidjson::SizeType i = 0; i < AnimCount; i++)
	{
		const rapidjson::Value& track = tracks[i];
		if (!track.IsObject())
		{
			return false;
		}
		//Get Bone Index
		int boneIndex;
		GetIntFromJSON(track, "bone", boneIndex);
		//Set Animation Transforms for indexed bone
		const rapidjson::Value& transforms = track["transforms"];
		if (!transforms.IsArray())
		{
			return false;
		}
		auto tCount = transforms.Size();
		BoneTransform animTrans;
		for (rapidjson::SizeType j = 0; j < tCount; j++)
		{
			GetQuaternionFromJSON(transforms[j], "rot", animTrans.mRot);
			GetVectorFromJSON(transforms[j], "trans", animTrans.mTrans);
			mAnimData[boneIndex].emplace_back(animTrans);
		}
	}
	return true;
}

void Animation::GetGlobalPoseAtTime(std::vector<Matrix4>& outPoses, const Skeleton* inSkeleton, float inTime) const
{
	//Choose Keyframe
	float TimePerS = GetLength() / GetNumFrames();
	size_t frame = inTime / TimePerS;

	//Lerp Parameters
	float fLerp = inTime / TimePerS;
	fLerp -= frame;

	auto transforms = mAnimData;
	size_t boneNum = inSkeleton->GetNumBones();
	for (size_t i = 0; i < boneNum; i++)
	{
		if (transforms[i].size() == 0)
		{
			outPoses[i] = Matrix4::Identity;
		}
		else
		{
			//Lerp Transform
			BoneTransform T;
			if (frame == mNumFrame - 1)
			{
				T = BoneTransform::Interpolate(transforms[i][frame], transforms[i][0], fLerp);
			}
			else
			{
				T = BoneTransform::Interpolate(transforms[i][frame], transforms[i][frame + 1], fLerp);
			}
			outPoses[i] = T.ToMatrix();
			int parentIndex = inSkeleton->GetBones()[i].mParent;
			if (parentIndex != -1)
			{
				outPoses[i] *= outPoses[parentIndex];
			}

		}
	}
}

