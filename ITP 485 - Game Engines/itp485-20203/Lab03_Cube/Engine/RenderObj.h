#pragma once
#include "engineMath.h"
class RenderObj
{
public:

	struct PerObjConstants {
		Matrix4 modelToWorld;
	} mObjConsts;

	class Graphics* mGraphics;
	const class VertexBuffer* mVerBuf = nullptr;
	const class Shader* mShad;
	ID3D11Buffer* mObjBuff;

	//Functions
	void Draw();
	RenderObj(Graphics* graphics, const Shader* shader);
	RenderObj(Graphics* graphics, const VertexBuffer* vertexBuffer, const Shader* shader);
	virtual ~RenderObj();
};