#pragma once
#include "RenderObj.h"
#include "stdafx.h"
#include "Graphics.h"

#define MAX_SKELETON_BONES 80
const int maxbone = 80;
struct skinning
{
	Vector3 pos;
	Vector3 normal;
	UINT8 bones[4];
	UINT8 weights[4];
	Vector2 uvpos;
};
struct SkinConsts
{
	Matrix4 c_skinMatrix[MAX_SKELETON_BONES];
};

class SkinnedObj : public RenderObj
{
public:
	//Constructor
	SkinnedObj(Graphics* graphics, const class Mesh* pMesh);
	~SkinnedObj() { mSkinBuff->Release(); }

	//Functions
	void Draw() override;


	//Variables
	skinning mSkinVertex;
	SkinConsts mSkinConsts;	

private:
	ID3D11Buffer* mSkinBuff;
};
