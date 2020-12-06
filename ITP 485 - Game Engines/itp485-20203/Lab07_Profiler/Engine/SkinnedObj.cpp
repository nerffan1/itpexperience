#include "stdafx.h"
#include "SkinnedObj.h"

SkinnedObj::SkinnedObj(Graphics* graphics, const Mesh* pMesh) :
	RenderObj(graphics, pMesh)
{
	//Create Graphics Buffer
	mSkinBuff = mGraphics->CreateGraphicsBuffer(
		&mSkinConsts,  //Pass Structure
		sizeof(mSkinConsts),
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	//Define Matrix
	for (size_t i = 0; i < MAX_SKELETON_BONES; i++)
	{
		mSkinConsts.c_skinMatrix[i] = Matrix4::Identity;
	}

	//Upload Buffer
	mGraphics->UploadBuffer(
		mSkinBuff,
		&mSkinConsts,
		sizeof(mSkinConsts)
	);
}

SkinnedObj::~SkinnedObj()
{
	mSkinBuff->Release();
}

void SkinnedObj::Draw()
{
	mGraphics->UploadBuffer(mSkinBuff,
		&mSkinConsts,
		sizeof(mSkinConsts)
	);
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(
		static_cast<UINT>(Graphics::CONSTANT_BUFFER_SKINNING),
		static_cast<UINT>(1),
		&mSkinBuff
	);
	RenderObj::Draw();
}
