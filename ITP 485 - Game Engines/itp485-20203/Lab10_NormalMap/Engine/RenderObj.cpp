#include "stdafx.h"
#include "RenderObj.h"
#include "VertexFormat.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Graphics.h"
#include "assetManager.h"
#include "mesh.h"
#include "Component.h"

RenderObj::RenderObj(Graphics* graphics, const Mesh* pMesh) :
	mGraphics(graphics),
	mMesh(pMesh)
{
	//Set Graphics Buffer with matrix
	mObjBuff = mGraphics->CreateGraphicsBuffer(
		static_cast<void*>(&mObjConsts),  //Pass Structure
		sizeof(mObjConsts),
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	//Define Matrix
	mObjConsts.modelToWorld = Matrix4::Identity;
	mGraphics->UploadBuffer(
		mObjBuff,
		static_cast<void*>(&mObjConsts),
		sizeof(mObjConsts)
	);
};

void RenderObj::Draw()
{
	mGraphics->UploadBuffer(mObjBuff,
		&mObjConsts,
		sizeof(mObjConsts)
	);
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(
		static_cast<UINT>(Graphics::CONSTANT_BUFFER_RENDEROBJ),
		static_cast<UINT>(1),
		&mObjBuff
	);
	mMesh->Draw();
}

void RenderObj::Update(float deltaTime)
{
	for (auto c : mComponents)
	{
		c->Update(deltaTime);
	}
}

void RenderObj::SetTexture(int slot, const Texture* texture)
{

}


RenderObj::~RenderObj()
{
	mObjBuff->Release();
}
