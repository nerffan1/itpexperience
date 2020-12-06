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
#include "texture.h"

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
	uint16_t Indices[] = { 0, 1, 2 };
	Graphics::Color4 red(1.0f, 0.0f, 0.0f, 1.0f);
	Graphics::Color4 green(0.0f, 1.0f, 0.0f, 1.0f);
	Graphics::Color4 blue(0.0f, 0.0f, 1.0f, 1.0f);
	VertexPosColor vert[] = {
		{ Vector3(0.0f, 0.5f, 0.0f), red },
		{ Vector3(0.45f, -0.5f, 0.0f), green },
		{ Vector3(-0.45f, -0.5f, 0.0f), blue }
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
	Shad->Load(L"Shaders/Mesh.hlsl", inputElem, ARRAY_SIZE(inputElem));

	//New Shader
	const D3D11_INPUT_ELEMENT_DESC inputElem2[] =
	{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexForTexture, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(VertexForTexture, color), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexForTexture, uvpos) , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexForTexture, normal) , D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	Shad2 = new Shader(&mGraphics);
	Shad2->Load(L"Shaders/BasicMesh.hlsl", inputElem2, ARRAY_SIZE(inputElem2));

	//Load Texture
	mTexture = new Texture(&mGraphics);
	mTexture->Load(L"Assets/Textures/Cube.png");

	//New
	mCam = new Camera(&mGraphics);
	mRenObj = new RenderObj(&mGraphics, vertB, Shad);
	mCube = new RenderCube(&mGraphics, Shad2);

	//Add Lights
	mLightBuf = mGraphics.CreateGraphicsBuffer(
		&mlightConsts,
		sizeof(mlightConsts),
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
	SetAmbientLight(Vector3(0.1f, 0.1f, 0.1f));
	PointLightData* pLight1 = AllocateLight();
	pLight1->diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	pLight1->specularColor = Vector3(1.0f, 1.0f, 1.0f);
	pLight1->position = Vector3(-100.0f, 0.0f, 50.0f);
	pLight1->specularPower = 10.0f;
	pLight1->innerRadius = 20.0f;
	pLight1->outerRadius = 200.0f;
	PointLightData* pLight2 = AllocateLight();
	pLight2->diffuseColor = Vector3(1.0f, 1.0f, 1.0f);
	pLight2->specularColor = Vector3(1.0f, 1.0f, 1.0f);
	pLight2->position = Vector3(75.0f, 150.0f, 100.0f);
	pLight2->specularPower = 10.0f;
	pLight2->innerRadius = 20.0f;
	pLight2->outerRadius = 200.0f;
}

void Game::Shutdown()
{
	delete mRenObj;
	delete mTexture;
	delete mCube;
	delete Shad;
	delete Shad2;
	delete mCam;
	mLightBuf->Release();
	mGraphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
	Theta += 3 * deltaTime;
	Matrix4 Input = Matrix4::CreateScale(300.0f) * Matrix4::CreateRotationZ(Theta / 2);
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

	//Set Active Cam
	mCam->SetActive();
	mTexture->SetActive(Graphics::TEXTURE_SLOT_DIFFUSE);

	//Lighting
	mGraphics.UploadBuffer(mLightBuf, &mlightConsts, sizeof(mlightConsts));
	mGraphics.GetDeviceContext()->PSSetConstantBuffers(
		mGraphics.CONSTANT_BUFFER_LIGHTING,
		1,
		&mLightBuf);

	//Draw triangle and cube
	mRenObj->Draw();

	//Set Active Texture
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

//Lights

PointLightData* Game::AllocateLight()
{
	for (size_t i = 0; i < MAX_LIGHTS; i++)
	{
		if (!mlightConsts.Lights[i].isEnabled)
		{
			mlightConsts.Lights[i].isEnabled = true;
			return &mlightConsts.Lights[i];
		}
	}
	return nullptr;
}

void Game::FreeLight(PointLightData* pLight)
{
	pLight->isEnabled = false;
}

void Game::SetAmbientLight(const Vector3& color)
{
	mlightConsts.c_ambient = color;
}

const Vector3& Game::GetAmbientLight()
{
	// TODO: insert return statement here
	return mlightConsts.c_ambient;
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