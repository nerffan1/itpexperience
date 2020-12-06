#pragma once
#include "RenderObj.h"
#include "stdafx.h"
#include "Graphics.h"

#define MAX_SKELETON_BONES 80
const int maxbone = 80;

struct SkinConsts
{
	Matrix4 c_skinMatrix[MAX_SKELETON_BONES];
};

class SkinnedObj : public RenderObj
{
public:
	//Constructor
	SkinnedObj(Graphics* graphics, const class Mesh* pMesh);
	~SkinnedObj();

	//Functions
	void Draw() override;


	//Variables
	SkinConsts mSkinConsts;	

private:
	ID3D11Buffer* mSkinBuff;
};
