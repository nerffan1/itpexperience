#include "stdafx.h"
#include "RenderCube.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"

RenderCube::RenderCube(Graphics* graphics, const Shader* shader): RenderObj(graphics,shader)
{
	Vector3 CubeVertices[8] =
	{
		Vector3(-0.5f, 0.5f, -0.5f),
		Vector3(0.5f, 0.5f, -0.5f),
		Vector3(0.5f, -0.5f, -0.5f),
		Vector3(-0.5f, -0.5f, -0.5f),
		Vector3(0.5f, 0.5f, 0.5f),
		Vector3(0.5f, -0.5f, 0.5f),
		Vector3(-0.5f, -0.5f, 0.5f),
		Vector3(-0.5f, 0.5f, 0.5f)

	};
	static VertexPosColor cubeVertex[] =
	{
	//1st face
	{ CubeVertices[0], Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ CubeVertices[1], Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ CubeVertices[2], Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ CubeVertices[3], Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) },
	//2nd face
	{ CubeVertices[1], Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ CubeVertices[2], Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ CubeVertices[4], Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ CubeVertices[5], Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) },
	//3rd Face
	{ CubeVertices[4], Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ CubeVertices[5], Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ CubeVertices[6], Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ CubeVertices[7], Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) },
	//4th Face
	{ CubeVertices[7], Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) },
	{ CubeVertices[6], Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) },
	{ CubeVertices[0], Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) },
	{ CubeVertices[3], Graphics::Color4(1.0f, 1.0f, 0.0f, 1.0f) },
	//5th Face
	{ CubeVertices[7], Graphics::Color4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ CubeVertices[4], Graphics::Color4(0.0f,  1.0f, 1.0f, 1.0f) },
	{ CubeVertices[0], Graphics::Color4(0.0f,  1.0f, 1.0f, 1.0f) },
	{ CubeVertices[1], Graphics::Color4(0.0f,  1.0f, 1.0f, 1.0f) },
	//6th Face
	{ CubeVertices[6], Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ CubeVertices[5], Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ CubeVertices[2], Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ CubeVertices[3], Graphics::Color4(1.0f, 0.0f, 1.0f, 1.0f) },
	};
	static uint16_t cubeIndex[] =
	{
	2, 1, 0,
	3, 2, 0,
	4, 7, 6,
	4, 5, 7,
	8, 9, 10,
	11, 8, 10,
	12,13,15,
	12,15,14,
	16, 18, 19,
	16, 19, 17,
	20, 21, 22,
	20, 22, 23
	};
	mVerBuf = new VertexBuffer(
		graphics,
		cubeVertex,
		ARRAY_SIZE(cubeVertex),
		sizeof(VertexPosColor),
		cubeIndex,
		ARRAY_SIZE(cubeIndex),
		sizeof(uint16_t)
		);
}

