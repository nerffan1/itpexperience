#include "stdafx.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"

VertexBuffer::VertexBuffer(Graphics* graphics,
	const void* vertexData, uint32_t vertexCount, uint32_t vertexStride,
	const void* indexData, uint32_t indexCount, uint32_t indexStride):
	mGraphics(graphics),
	mIndices(indexCount),
	mVerStr(vertexStride),
	mBuffer(nullptr),
	mIndexFormat(DXGI_FORMAT_R16_UINT)
{
	mBuffer = mGraphics->CreateGraphicsBuffer(
		vertexData,
		mVerStr*vertexCount,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);

	mIndexBuff = mGraphics->CreateGraphicsBuffer(
		indexData,
		indexStride * mIndices,
		D3D11_BIND_INDEX_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
	
}

VertexBuffer::~VertexBuffer()
{
	mBuffer->Release();
	mIndexBuff->Release();
}

void VertexBuffer::Draw() const
{
	UINT stride[] = { mVerStr };
	UINT pOffset[] = { 0 };
	mGraphics->GetDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&mBuffer,
		stride,
		pOffset
	);

	mGraphics->GetDeviceContext()->IASetIndexBuffer(
		mIndexBuff,
		mIndexFormat,
		*pOffset
	);

	mGraphics->GetDeviceContext()->DrawIndexed(
		mIndices,
		0,
		0
	);
}
