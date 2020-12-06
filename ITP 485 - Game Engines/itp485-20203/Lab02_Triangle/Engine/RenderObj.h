#pragma once

class RenderObj
{
public:
	class Graphics* mGraphics = nullptr;
	const class VertexBuffer* mVerBuf = nullptr;
	const class Shader* mShad = nullptr;
	virtual void Draw();
	RenderObj(Graphics* graphics, const VertexBuffer* vertexBuffer, const Shader* shader);
	virtual ~RenderObj();
};