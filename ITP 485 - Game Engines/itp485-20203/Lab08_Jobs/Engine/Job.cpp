#include "stdafx.h"
#include "Job.h"
#include "Character.h"
#include "Animation.h"
#include "Skeleton.h"
#include "SkinnedObj.h"
#include "Profiler.h"

AnimateUp::AnimateUp(Character* inChar) : Job(), mChar(inChar)
{

}

void AnimateUp::DoIt()
{
	if (mChar->mAnimTime > mChar->mCurrentAnim->GetLength())
	{
		mChar->mAnimTime = 0.0f;
	}
	std::vector<Matrix4> outPoses(mChar->mSkel->GetNumBones());
	mChar->mCurrentAnim->GetGlobalPoseAtTime(outPoses, mChar->mSkel, mChar->mAnimTime);
	std::vector<Matrix4> bindPoses = mChar->mSkel->GetGlobalInvBindPoses();

	//Combine matrices 
	size_t boneSize = mChar->mSkel->GetNumBones();
	for (size_t i = 0; i < boneSize; i++)
	{
		mChar->mSkinnedObj->mSkinConsts.c_skinMatrix[i] = bindPoses[i] * outPoses[i];
	}
}
