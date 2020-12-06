#include "stdafx.h"
#include "BoneTransform.h"

BoneTransform BoneTransform::Interpolate(const BoneTransform& a, const BoneTransform& b, float f)
{
    Vector3 ltrans = Lerp(a.mTrans, b.mTrans, f);
    Quaternion lrot = Lerp(a.mRot, b.mRot, f);
    BoneTransform lerpTrans;
    lerpTrans.mRot = lrot;
    lerpTrans.mTrans = ltrans;

    return lerpTrans;
}
