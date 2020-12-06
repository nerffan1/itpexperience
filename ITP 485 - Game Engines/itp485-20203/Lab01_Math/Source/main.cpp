// Vector_01.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "engineMath.h"
#include <assert.h>

int main()
{
    bool isOk = true;

    isOk &= Vector2::UnitTest();
    assert(isOk);
    isOk &= Vector3::UnitTest();
    assert(isOk);
    isOk &= Vector4::UnitTest();
    assert(isOk);
    isOk &= Matrix3::UnitTest();
    assert(isOk);
    isOk &= Matrix4::UnitTest();
    assert(isOk);
    isOk &= Quaternion::UnitTest();
    assert(isOk);

    return (int)(false == isOk);	// return 0 if everything is ok
}

