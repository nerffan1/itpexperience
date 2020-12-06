#pragma once
#include "engineMath.h"

class BoneTransform
{
public:
	//Function
	Matrix4 ToMatrix() const
	{
		Matrix4 bindpose = Matrix4::CreateFromQuaternion(mRot);
		bindpose *= Matrix4::CreateTranslation(mTrans);
		return bindpose;
	}

	//Variables 
	Quaternion mRot;
	Vector3 mTrans;

	static BoneTransform Interpolate(const BoneTransform& a, const BoneTransform& b, float f);
};