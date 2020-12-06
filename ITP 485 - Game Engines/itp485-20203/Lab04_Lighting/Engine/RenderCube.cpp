#include "stdafx.h"
#include "RenderCube.h"
#include "VertexBuffer.h"
#include "VertexFormat.h"

RenderCube::RenderCube(Graphics* graphics, const Shader* shader) : RenderObj(graphics, shader)
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
	//Colors
	Graphics::Color4 red(1.0f, 0.0f, 0.0f, 1.0f);
	Graphics::Color4 green(0.0f, 1.0f, 0.0f, 1.0f);
	Graphics::Color4 blue(0.0f, 0.0f, 1.0f, 1.0f);
	Graphics::Color4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
	Graphics::Color4 cyan(0.0f, 1.0f, 1.0f, 1.0f);
	Graphics::Color4 magenta(1.0f, 0.0f, 1.0f, 1.0f);

	//Normals
	Vector3 norms[6] = {
		Vector3(1.0f,0.0f,0.0f),
		Vector3(-1.0f,0.0f,0.0f),
		Vector3(0.0f,1.0f,0.0f),
		Vector3(0.0f,-1.0f,0.0f),
		Vector3(0.0f,0.0f,1.0f),
		Vector3(0.0f,0.0f,-1.0f)
	};

	static VertexForTexture cubeVertex[] =
	{
		//1st face
		{ CubeVertices[0], red, Vector2(1.0f, 1.0f), norms[5]},
		{ CubeVertices[1], red, Vector2(1.0f, 0.0f), norms[5]},
		{ CubeVertices[2], red, Vector2(0.0f, 0.0f), norms[5]},
		{ CubeVertices[3], red, Vector2(0.0f, 1.0f), norms[5]},
		//2nd face
		{ CubeVertices[1], green, Vector2(0.0f, 0.0f), norms[0]},
		{ CubeVertices[2], green, Vector2(0.0f, 1.0f), norms[0]},
		{ CubeVertices[4], green, Vector2(1.0f, 0.0f), norms[0]},
		{ CubeVertices[5], green, Vector2(1.0f, 1.0f), norms[0]},
		//3rd Face
		{ CubeVertices[4], blue, Vector2(1.0f, 0.0f), norms[4]},
		{ CubeVertices[5], blue, Vector2(1.0f, 1.0f), norms[4]},
		{ CubeVertices[6], blue, Vector2(0.0f, 1.0f), norms[4]},
		{ CubeVertices[7], blue, Vector2(0.0f, 0.0f), norms[4]},
		//4th Face
		{ CubeVertices[7], yellow, Vector2(1.0f, 1.0f), norms[1]},
		{ CubeVertices[6], yellow, Vector2(0.0f, 1.0f), norms[1]},
		{ CubeVertices[0], yellow, Vector2(1.0f, 0.0f), norms[1]},
		{ CubeVertices[3], yellow, Vector2(0.0f, 0.0f), norms[1]},
		//5th Face
		{ CubeVertices[7], cyan, Vector2(0.0f, 1.0f), norms[2]},
		{ CubeVertices[4], cyan, Vector2(1.0f, 1.0f), norms[2]},
		{ CubeVertices[0], cyan, Vector2(0.0f, 0.0f), norms[2]},
		{ CubeVertices[1], cyan, Vector2(1.0f, 0.0f), norms[2]},
		//6th Face
		{ CubeVertices[6], magenta, Vector2(0.0f, 0.0f), norms[3]},
		{ CubeVertices[5], magenta, Vector2(1.0f, 0.0f), norms[3]},
		{ CubeVertices[2], magenta, Vector2(1.0f, 1.0f), norms[3]},
		{ CubeVertices[3], magenta, Vector2(0.0f, 1.0f), norms[3]},
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
		sizeof(VertexForTexture),
		cubeIndex,
		ARRAY_SIZE(cubeIndex),
		sizeof(uint16_t)
	);
}