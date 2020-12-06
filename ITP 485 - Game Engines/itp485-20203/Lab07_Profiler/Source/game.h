#pragma once
#include "Graphics.h"
#include "engineMath.h"
class Shader;

const int MAX_LIGHTS = 8;

struct PointLightData {
	Vector3 diffuseColor;
	float specularPower;
	Vector3 specularColor;
	float innerRadius;
	Vector3 position;
	float outerRadius;
	bool isEnabled;
	Vector3 pad;
};

struct LightConstants
{
	Vector3 c_ambient;
	float pad;
	PointLightData Lights[MAX_LIGHTS];
};

class Game
{
public:
	Game();
	~Game();

	void Init(HWND hWnd, float width, float height);
	void Shutdown();
	void Update(float deltaTime);
	void RenderFrame();

	void OnKeyDown(uint32_t key);
	void OnKeyUp(uint32_t key);
	bool IsKeyHeld(uint32_t key) const;

	//Lights
	PointLightData* AllocateLight();
	void FreeLight(PointLightData* pLight);
	void SetAmbientLight(const Vector3& color);
	const Vector3& GetAmbientLight();

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;

	Graphics mGraphics;

	class Camera* mCam;

	//Lights
	ID3D11Buffer* mLightBuf = nullptr;
	LightConstants mlightConsts;

	//Asset Manager 
	class AssetManager* mManager;
	std::vector<class RenderObj*> mObjects;

	bool LoadLevel(const WCHAR* fileName);
};