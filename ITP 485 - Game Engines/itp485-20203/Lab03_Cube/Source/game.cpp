#include "stdafx.h"
#include "Game.h"
#include "engineMath.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "RenderObj.h"
#include "VertexFormat.h"
#include "Shader.h"
#include "Camera.h"
#include "stringUtil.h"
#include "rapidjson\include\rapidjson\rapidjson.h"
#include "rapidjson\include\rapidjson\document.h"
#include <fstream>
#include <sstream>
#include "RenderCube.h"

float Theta = 0;

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init(HWND hWnd, float width, float height)
{
	mGraphics.InitD3D(hWnd, width, height);

	//Triangle
	uint16_t Indices[] = {0, 1, 2};
	VertexPosColor vert[] = { 
		{ Vector3(0.0f, 0.5f, 0.0f), Graphics::Color4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vector3(0.45f, -0.5f, 0.0f), Graphics::Color4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vector3(-0.45f, -0.5f, 0.0f), Graphics::Color4(0.0f, 0.0f, 1.0f, 1.0f) } 
	};
	VertexBuffer* vertB = new VertexBuffer( 
		&mGraphics,
		vert, 
		ARRAY_SIZE(vert),
		sizeof(VertexPosColor),
		Indices,
		ARRAY_SIZE(Indices),
		sizeof(uint16_t)
	);

	//Shader
	const D3D11_INPUT_ELEMENT_DESC inputElem[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPosColor, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexPosColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	Shad = new Shader(&mGraphics);
	Shad->Load(L"Shaders/Mesh.hlsl", inputElem, ARRAY_SIZE(inputElem) );

	//New
	mCam = new Camera(&mGraphics);
	mRenObj = new RenderObj(&mGraphics, vertB, Shad);
	mCube = new RenderCube(&mGraphics, Shad);
}

void Game::Shutdown()
{
	delete mRenObj;
	delete mCube;
	delete Shad;
	delete mCam;
	mGraphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
	Theta += 2 * deltaTime;
	Matrix4 Input = Matrix4::CreateScale(300.0f) * Matrix4::CreateRotationZ(Theta/2);
	mRenObj->mObjConsts.modelToWorld = Input;
	Input = Matrix4::CreateScale(100.0f)
		* Matrix4::CreateRotationY(Theta)
		* Matrix4::CreateRotationX(0.25f * Theta);
	mCube->mObjConsts.modelToWorld = Input;
}

void Game::RenderFrame()
{
	// Clear the screen to blue
	Graphics::Color4 clearColor(0.0f, 0.2f, 0.4f, 1.0f);
	mGraphics.BeginFrame(clearColor);

	//Set Active
	mCam->SetActive();

	//Draw
	mRenObj->Draw();
	mCube->Draw();

	mGraphics.EndFrame();
}

void Game::OnKeyDown(uint32_t key)
{
	m_keyIsHeld[key] = true;
}

void Game::OnKeyUp(uint32_t key)
{
	m_keyIsHeld[key] = false;
}

bool Game::IsKeyHeld(uint32_t key) const
{
	const auto find = m_keyIsHeld.find(key);
	if (find != m_keyIsHeld.end())
		return find->second;
	return false;
}

bool Game::LoadLevel(const WCHAR* fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		return false;
	}

	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);

	if (!doc.IsObject())
	{
		return false;
	}

	std::string str = doc["metadata"]["type"].GetString();
	int ver = doc["metadata"]["version"].GetInt();

	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itplevel" ||
		ver != 2)
	{
		return false;
	}

	return true;
}