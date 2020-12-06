#pragma once
#include "Graphics.h"

class Shader;

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

private:
	std::unordered_map<uint32_t, bool> m_keyIsHeld;
	
	Graphics mGraphics;
	class RenderObj* mRenObj;
	class RenderObj* mCube;
	Shader* Shad = nullptr;
	class Camera* mCam;

	bool LoadLevel(const WCHAR* fileName);
};