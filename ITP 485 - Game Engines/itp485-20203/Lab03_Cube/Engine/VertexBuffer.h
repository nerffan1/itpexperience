#pragma once
#include <d3d11.h>
#include "Graphics.h"

class VertexBuffer
{
public:
	VertexBuffer(Graphics* graphics, 
		const void* vertexData, uint32_t vertexCount, uint32_t vertexStride, 
		const void* indexData, uint32_t indexCount, uint32_t indexStride);
	~VertexBuffer();
	void Draw() const;

	//Index Variables
	Graphics* mGraphics;
	ID3D11Buffer* mIndexBuff = nullptr;
	DXGI_FORMAT mIndexFormat;
	uint32_t mIndices;

	//Vertex Variables
	ID3D11Buffer* mBuffer = nullptr;
	uint32_t mVerStr;
};