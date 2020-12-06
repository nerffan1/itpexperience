#include "stdafx.h"
#include "RenderObj.h"
#include "VertexFormat.h"
#include "Shader.h"
#include "VertexBuffer.h"

void RenderObj::Draw()
{	
	mVerBuf->Draw();
}

RenderObj::RenderObj(Graphics* graphics, const VertexBuffer* vertexBuffer, const Shader* shader):
	mGraphics(graphics),
	mVerBuf(vertexBuffer),
	mShad(shader)
{
}

RenderObj::~RenderObj()
{
	delete mVerBuf;
}
