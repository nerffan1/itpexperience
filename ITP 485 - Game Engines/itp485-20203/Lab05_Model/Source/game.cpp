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
#include "assetManager.h"
#include  "mesh.h"
#include "jsonUtil.h"
#include "PointLight.h"

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
	mManager = new AssetManager(&mGraphics);

	//Phong
	const D3D11_INPUT_ELEMENT_DESC phongInput[] =
	{
	{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Phong, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Phong, uvpos) , D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Phong, normal) , D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mManager->SetShader(L"Phong", new Shader(&mGraphics));
	mManager->GetShader(L"Phong")->Load(L"Shaders/Phong.hlsl", phongInput, ARRAY_SIZE(phongInput));

	//Unlit
	const D3D11_INPUT_ELEMENT_DESC UnlitInput[] =
	{
	{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Unlit, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Unlit, uvpos) , D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	mManager->SetShader(L"Unlit", new Shader(&mGraphics));
	mManager->GetShader(L"Unlit")->Load(L"Shaders/Unlit.hlsl", UnlitInput, ARRAY_SIZE(UnlitInput));

	//LoadLevel
	LoadLevel(L"Assets/Levels/Level05.itplevel");

	//Add Lights
	mLightBuf = mGraphics.CreateGraphicsBuffer(
		&mlightConsts,
		sizeof(mlightConsts),
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);
}

void Game::Shutdown()
{
	delete mCam;

	for (auto o : mObjects)
	{
		delete o;
	}

	mLightBuf->Release();
	mManager->Clear();
	mGraphics.CleanD3D();
}

void Game::Update(float deltaTime)
{
	Theta += 3 * deltaTime;
	Matrix4 Input = Matrix4::CreateScale(300.0f) * Matrix4::CreateRotationZ(Theta / 2);

	Input = Matrix4::CreateScale(2.0f)
		* Matrix4::CreateRotationY(Theta)
		* Matrix4::CreateRotationX(0.25f * Theta);

	//Loop through all objects and update 
	for (auto o : mObjects)
	{
		o->Update(deltaTime);	
	}

}

void Game::RenderFrame()
{
	// Clear the screen to blue
	Graphics::Color4 clearColor(0.0f, 0.2f, 0.4f, 1.0f);
	mGraphics.BeginFrame(clearColor);

	//Set Active Cam
	mCam->SetActive();

	//Lighting
	mGraphics.UploadBuffer(mLightBuf, &mlightConsts, sizeof(mlightConsts));
	mGraphics.GetDeviceContext()->PSSetConstantBuffers(
		mGraphics.CONSTANT_BUFFER_LIGHTING,
		1,
		&mLightBuf);

	//Draw Objects
	for (auto o : mObjects)
	{
		o->Draw();
	}

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

	//Load Camera
	Vector3 pos;
	Quaternion rot;
	const rapidjson::Value& camObj = doc["camera"];
	//New
	mCam = new Camera(&mGraphics);
	if (camObj.IsObject())
	{
		GetVectorFromJSON(camObj, "position", pos);
		GetQuaternionFromJSON(camObj, "rotation", rot);
		Matrix4 posmat = Matrix4::CreateTranslation(pos);
		Matrix4 RotMatrix = Matrix4::CreateFromQuaternion(rot);
		mCam->mWorldToCam = RotMatrix * posmat;
		mCam->mWorldToCam.Invert();

	}
	//Load Ambient Light
	const rapidjson::Value& light = doc["lightingData"];
	if (camObj.IsObject())
	{
		Vector3 Ambient;
		GetVectorFromJSON(light, "ambient", Ambient);
		mlightConsts.c_ambient = Ambient;
	}
	//RenderFiles
	float scale;
	const rapidjson::Value& objs = doc["renderObjects"];
	for (unsigned int i = 0; i < objs.Size(); i++)
	{
		const rapidjson::Value& obj = objs[i];
		//Get Pos, Rot, Scale
		GetVectorFromJSON(obj, "position", pos);
		Matrix4 PosMat = Matrix4::CreateTranslation(pos);
		GetQuaternionFromJSON(obj, "rotation", rot);
		Matrix4 RotMat = Matrix4::CreateFromQuaternion(rot);
		GetFloatFromJSON(obj, "scale", scale);
		Matrix4 ScaleMat = Matrix4::CreateScale(scale);

		//Mesh 
		std::wstring MeshFile;
		GetWStringFromJSON(obj, "mesh", MeshFile);
		RenderObj* ObjectToRender = new RenderObj(&mGraphics, mManager->LoadMesh(MeshFile));
		ObjectToRender->mObjConsts.modelToWorld = ScaleMat * RotMat * PosMat;
		mObjects.push_back(ObjectToRender);

		//Lights
		const rapidjson::Value& ComponentArray = obj["components"]; //Perhaps will have to loop through in a near future
		for (rapidjson::SizeType i = 0; i < ComponentArray.Size(); i++)
		{
			const rapidjson::Value& Properties = obj["components"][i];
			std::string type = Properties["type"].GetString();
			if (type == "PointLight")
			{
				//Call PointLight Load Propoerties
				PointLight* AddLight = new PointLight(this, ObjectToRender);
				AddLight->LoadProperties(Properties);
				ObjectToRender->AddComponent(AddLight);
			}
		}

	}
	//LightBuffer
	mLightBuf = mGraphics.CreateGraphicsBuffer(
		&mlightConsts,
		sizeof(mlightConsts),
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		D3D11_USAGE_DYNAMIC);


	// Check the metadata
	if (!doc["metadata"].IsObject() ||
		str != "itplevel" ||
		ver != 2)
	{
		return false;
	}


	return true;
}