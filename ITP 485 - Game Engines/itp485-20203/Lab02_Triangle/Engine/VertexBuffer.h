#pragma once
#include <d3d11.h>
#include "Graphics.h"

class VertexBuffer
{
public:
	VertexBuffer(Graphics* graphics, const void* vertData, uint32_t vertCount, uint32_t vertStride);
	~VertexBuffer();
	void Draw() const;
	ID3D11Buffer* mBuffer = nullptr;
	uint32_t mVertices;
	uint32_t mStride;
	Graphics* mGraphics;
};