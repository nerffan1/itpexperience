#include "stdafx.h"
#include "RenderObj.h"
#include "VertexFormat.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Graphics.h"

void RenderObj::Draw()
{
	mGraphics->UploadBuffer(mObjBuff,
		&mObjConsts,
		sizeof(mObjConsts)
	);
	mGraphics->GetDeviceContext()->VSSetConstantBuffers(
		static_cast<UINT>(mGraphics->CONSTANT_BUFFER_RENDEROBJ),
		static_cast<UINT>(1),
		&mObjBuff
	); 
	mShad->SetActive();
	mVerBuf->Draw();
}

RenderObj::RenderObj(Graphics* graphics, const Shader* shader) : 
	mGraphics(graphics),
	mShad(shader),
	mVerBuf(nullptr)
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
}

RenderObj::RenderObj(Graphics* graphics, const VertexBuffer* vertexBuffer, const Shader* shader):
	mGraphics(graphics),
	mVerBuf(vertexBuffer),
	mShad(shader)

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
}

RenderObj::~RenderObj()
{
	delete mVerBuf;
	mObjBuff->Release();
}
