#include "stdafx.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"

VertexBuffer::VertexBuffer(Graphics* graphics, const void* vertData, uint32_t vertCount, uint32_t vertStride):
	mVertices(vertCount),
	mGraphics(graphics),
	mStride(vertStride),
	mBuffer(nullptr)
{
	mBuffer = mGraphics->CreateGraphicsBuffer(
		vertData,
		mStride*mVertices,
		D3D11_BIND_VERTEX_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
}

VertexBuffer::~VertexBuffer()
{
	mBuffer->Release();
}

void VertexBuffer::Draw() const
{
	UINT stride[] = { mStride };
	UINT pOffset[] = { 0 };
	mGraphics->GetDeviceContext()->IASetVertexBuffers(
		0,
		1,
		&mBuffer,
		stride,
		pOffset
	);
	mGraphics->GetDeviceContext()->Draw(mVertices, 0);
}
